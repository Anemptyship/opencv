/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2024, OpenCV Contributors, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the copyright holders or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

/* ////////////////////////////////////////////////////////////////////
//
//  Phase Congruency based Edge Detection
//
//  This implementation is based on the work of Peter Kovesi:
//  - "Image Features From Phase Congruency" (1999)
//  - "Phase Congruency Detects Corners and Edges" (2003)
//
// */

#include "precomp.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/hal/intrin.hpp"
#include <vector>
#include <cmath>

namespace cv
{

// Helper function to compute Gabor filter responses (even and odd parts)
static void computeGaborResponse(const Mat& src, const Mat& gaborEven, const Mat& gaborOdd,
                                 Mat& responseEven, Mat& responseOdd)
{
    filter2D(src, responseEven, CV_32F, gaborEven, Point(-1, -1), 0, BORDER_REPLICATE);
    filter2D(src, responseOdd, CV_32F, gaborOdd, Point(-1, -1), 0, BORDER_REPLICATE);
}

// Compute amplitude and phase from even and odd Gabor responses
static void computeAmplitudePhase(const Mat& even, const Mat& odd, Mat& amplitude, Mat& phase)
{
    amplitude.create(even.size(), CV_32F);
    phase.create(even.size(), CV_32F);

    const float* evenPtr = even.ptr<float>();
    const float* oddPtr = odd.ptr<float>();
    float* ampPtr = amplitude.ptr<float>();
    float* phasePtr = phase.ptr<float>();
    size_t total = even.total();

    for (size_t i = 0; i < total; i++)
    {
        float e = evenPtr[i];
        float o = oddPtr[i];
        ampPtr[i] = std::sqrt(e * e + o * o);
        phasePtr[i] = std::atan2(o, e);
    }
}

// Weight function for phase congruency (suppresses noise in uniform regions)
static float computeWeight(float amplitude, float maxAmplitude, double noiseThreshold)
{
    if (maxAmplitude < 1e-6f)
        return 0.0f;
    
    float weight = 1.0f / (1.0f + std::exp((noiseThreshold - amplitude / maxAmplitude) * 10.0f));
    return weight;
}

void phaseCongruencyEdges(InputArray _src, OutputArray _dst,
                          int numScales, int numOrientations,
                          double threshold, double noiseThreshold,
                          double k, double sigma, double minWaveLength,
                          double mult, OutputArray _orientationEdges)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert(!src.empty());
    CV_Assert(src.channels() == 1);
    CV_Assert(numScales > 0 && numOrientations > 0);
    CV_Assert(threshold >= 0.0 && threshold <= 1.0);
    CV_Assert(minWaveLength >= 3.0);
    CV_Assert(mult > 1.0);

    // Convert input to float if needed
    Mat srcFloat;
    if (src.depth() != CV_32F)
    {
        src.convertTo(srcFloat, CV_32F, 1.0 / 255.0);
    }
    else
    {
        srcFloat = src.clone();
    }

    Size imgSize = srcFloat.size();
    
    // Storage for filter responses
    std::vector<Mat> amplitudes(numScales * numOrientations);
    std::vector<Mat> phases(numScales * numOrientations);
    std::vector<Mat> evenResponses(numScales * numOrientations);
    std::vector<Mat> oddResponses(numScales * numOrientations);

    // Compute filter bank responses
    double gamma = 1.0; // Aspect ratio for Gabor filters
    double psiEven = 0.0; // Phase offset for even-symmetric filter
    double psiOdd = CV_PI / 2.0; // Phase offset for odd-symmetric filter

    // Determine kernel size based on maximum wavelength
    double maxWaveLength = minWaveLength * std::pow(mult, numScales - 1);
    int kernelSize = cvRound(maxWaveLength * 3) * 2 + 1; // Ensure odd size
    if (kernelSize < 5) kernelSize = 5;
    if (kernelSize % 2 == 0) kernelSize += 1;

    // Generate Gabor filter bank and compute responses
    for (int s = 0; s < numScales; s++)
    {
        double wavelength = minWaveLength * std::pow(mult, s);
        double currentSigma = sigma * wavelength;
        
        for (int o = 0; o < numOrientations; o++)
        {
            double theta = CV_PI * o / numOrientations;
            int idx = s * numOrientations + o;

            // Create even and odd Gabor filters
            Mat gaborEven = getGaborKernel(Size(kernelSize, kernelSize),
                                          currentSigma, theta, wavelength,
                                          gamma, psiEven, CV_32F);
            Mat gaborOdd = getGaborKernel(Size(kernelSize, kernelSize),
                                         currentSigma, theta, wavelength,
                                         gamma, psiOdd, CV_32F);

            // Normalize filters to have zero mean
            Scalar meanEven = mean(gaborEven);
            gaborEven -= meanEven[0];
            Scalar meanOdd = mean(gaborOdd);
            gaborOdd -= meanOdd[0];

            // Compute filter responses
            computeGaborResponse(srcFloat, gaborEven, gaborOdd,
                                evenResponses[idx], oddResponses[idx]);

            // Compute amplitude and phase
            computeAmplitudePhase(evenResponses[idx], oddResponses[idx],
                                 amplitudes[idx], phases[idx]);
        }
    }

    // Compute phase congruency
    Mat phaseCongruency = Mat::zeros(imgSize, CV_32F);
    Mat orientationMap;
    bool computeOrientation = !_orientationEdges.empty();
    if (computeOrientation)
    {
        orientationMap = Mat::zeros(imgSize, CV_32F);
    }

    // Find maximum amplitude at each pixel across all scales/orientations
    Mat maxAmplitude = Mat::zeros(imgSize, CV_32F);
    for (size_t i = 0; i < amplitudes.size(); i++)
    {
        max(maxAmplitude, amplitudes[i], maxAmplitude);
    }

    // Add small epsilon to avoid division by zero
    maxAmplitude += 1e-6f;

    // Compute phase congruency using the formula:
    // PC = Σ W_n [A_n ΔΦ_n - T] / (ε + Σ A_n)
    // where ΔΦ_n is the phase deviation, A_n is amplitude, W_n is weight
    // 
    // For each orientation, we compute phase congruency across scales
    // and then take the maximum across orientations

    // Pre-compute mean phase for each orientation (across scales)
    std::vector<Mat> meanPhases(numOrientations);
    for (int o = 0; o < numOrientations; o++)
    {
        meanPhases[o] = Mat::zeros(imgSize, CV_32F);
        Mat sumCos = Mat::zeros(imgSize, CV_32F);
        Mat sumSin = Mat::zeros(imgSize, CV_32F);
        Mat count = Mat::zeros(imgSize, CV_32F);

        for (int s = 0; s < numScales; s++)
        {
            int idx = s * numOrientations + o;
            const Mat& amp = amplitudes[idx];
            const Mat& phase = phases[idx];

            for (int y = 0; y < imgSize.height; y++)
            {
                const float* ampPtr = amp.ptr<float>(y);
                const float* phasePtr = phase.ptr<float>(y);
                float* cosPtr = sumCos.ptr<float>(y);
                float* sinPtr = sumSin.ptr<float>(y);
                float* countPtr = count.ptr<float>(y);

                for (int x = 0; x < imgSize.width; x++)
                {
                    if (ampPtr[x] > 1e-6f) // Only consider significant amplitudes
                    {
                        cosPtr[x] += ampPtr[x] * std::cos(phasePtr[x]);
                        sinPtr[x] += ampPtr[x] * std::sin(phasePtr[x]);
                        countPtr[x] += ampPtr[x];
                    }
                }
            }
        }

        // Compute mean phase
        for (int y = 0; y < imgSize.height; y++)
        {
            const float* cosPtr = sumCos.ptr<float>(y);
            const float* sinPtr = sumSin.ptr<float>(y);
            const float* countPtr = count.ptr<float>(y);
            float* meanPhasePtr = meanPhases[o].ptr<float>(y);

            for (int x = 0; x < imgSize.width; x++)
            {
                if (countPtr[x] > 1e-6f)
                {
                    meanPhasePtr[x] = std::atan2(sinPtr[x], cosPtr[x]);
                }
            }
        }
    }

    // Compute phase congruency for each pixel
    for (int y = 0; y < imgSize.height; y++)
    {
        for (int x = 0; x < imgSize.width; x++)
        {
            float maxPC = 0.0f;
            float bestOrientation = 0.0f;
            float maxAmp = maxAmplitude.at<float>(y, x);

            // Compute PC for each orientation and take maximum
            for (int o = 0; o < numOrientations; o++)
            {
                float sumWeightedPhaseDev = 0.0f;
                float sumAmplitude = 0.0f;
                float meanPhase = meanPhases[o].at<float>(y, x);

                // Sum over all scales for this orientation
                for (int s = 0; s < numScales; s++)
                {
                    int idx = s * numOrientations + o;
                    float amp = amplitudes[idx].at<float>(y, x);
                    float phase = phases[idx].at<float>(y, x);

                    if (amp > 1e-6f)
                    {
                        // Compute phase deviation from mean phase
                        float phaseDiff = phase - meanPhase;
                        // Normalize to [-pi, pi]
                        while (phaseDiff > CV_PI) phaseDiff -= 2.0f * CV_PI;
                        while (phaseDiff < -CV_PI) phaseDiff += 2.0f * CV_PI;
                        float phaseDev = std::abs(std::cos(phaseDiff));

                        // Compute weight
                        float weight = computeWeight(amp, maxAmp, noiseThreshold);

                        // Accumulate weighted phase deviation
                        sumWeightedPhaseDev += weight * amp * phaseDev;
                        sumAmplitude += amp;
                    }
                }

                // Compute phase congruency value for this orientation
                float epsilon = 1e-6f;
                float pcValue = (sumWeightedPhaseDev - noiseThreshold * sumAmplitude) / 
                               (epsilon + sumAmplitude);
                pcValue = std::max(0.0f, pcValue); // Ensure non-negative

                // Track maximum PC and corresponding orientation
                if (pcValue > maxPC)
                {
                    maxPC = pcValue;
                    bestOrientation = CV_PI * o / numOrientations;
                }
            }

            phaseCongruency.at<float>(y, x) = maxPC;

            if (computeOrientation && maxPC > threshold)
            {
                orientationMap.at<float>(y, x) = bestOrientation;
            }
        }
    }

    // Apply threshold and convert to 8-bit
    Mat thresholded;
    threshold(phaseCongruency, thresholded, threshold, 1.0, THRESH_BINARY);

    // Convert to 8-bit output
    thresholded.convertTo(_dst, CV_8U, 255.0);

    // Output orientation map if requested
    if (computeOrientation)
    {
        _orientationEdges.assign(orientationMap);
    }
}

} // namespace cv
