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
//M*/

#include "test_precomp.hpp"

namespace opencv_test { namespace {

TEST(Imgproc_PhaseCongruencyEdges, BasicTest)
{
    // Create a simple test image with a square
    Mat src = Mat::zeros(100, 100, CV_8UC1);
    rectangle(src, Point(30, 30), Point(70, 70), Scalar(255), 2);
    
    Mat edges;
    phaseCongruencyEdges(src, edges);
    
    // Check that output has correct size and type
    ASSERT_EQ(edges.size(), src.size());
    ASSERT_EQ(edges.type(), CV_8UC1);
    
    // Check that some edges were detected
    int edgeCount = countNonZero(edges);
    EXPECT_GT(edgeCount, 0) << "No edges detected in test image";
}

TEST(Imgproc_PhaseCongruencyEdges, ParameterTest)
{
    Mat src = Mat::zeros(100, 100, CV_8UC1);
    rectangle(src, Point(30, 30), Point(70, 70), Scalar(255), 2);
    
    Mat edges1, edges2;
    
    // Test with different parameters
    phaseCongruencyEdges(src, edges1, 3, 4, 0.05);
    phaseCongruencyEdges(src, edges2, 5, 8, 0.2);
    
    ASSERT_EQ(edges1.size(), src.size());
    ASSERT_EQ(edges2.size(), src.size());
    
    // Different parameters should produce different results
    // (though both should detect edges)
    int count1 = countNonZero(edges1);
    int count2 = countNonZero(edges2);
    
    EXPECT_GT(count1, 0);
    EXPECT_GT(count2, 0);
}

TEST(Imgproc_PhaseCongruencyEdges, OrientationTest)
{
    Mat src = Mat::zeros(100, 100, CV_8UC1);
    // Create a diagonal line
    line(src, Point(10, 10), Point(90, 90), Scalar(255), 2);
    
    Mat edges, orientation;
    phaseCongruencyEdges(src, edges, 4, 6, 0.1, 0.01, 2.0, 0.5, 3.0, 2.1, orientation);
    
    ASSERT_EQ(edges.size(), src.size());
    ASSERT_EQ(orientation.size(), src.size());
    ASSERT_EQ(orientation.type(), CV_32FC1);
    
    // Check that orientation is computed for edge pixels
    Mat edgeMask = (edges > 0);
    Mat orientationAtEdges;
    orientation.copyTo(orientationAtEdges, edgeMask);
    
    int validOrientations = countNonZero(orientationAtEdges > 0);
    EXPECT_GT(validOrientations, 0) << "No orientation information at edge pixels";
}

TEST(Imgproc_PhaseCongruencyEdges, EmptyImage)
{
    Mat src = Mat::zeros(100, 100, CV_8UC1);
    Mat edges;
    
    phaseCongruencyEdges(src, edges);
    
    // Empty image should produce empty edge map
    int edgeCount = countNonZero(edges);
    EXPECT_EQ(edgeCount, 0) << "Edges detected in empty image";
}

TEST(Imgproc_PhaseCongruencyEdges, FloatInput)
{
    Mat src = Mat::zeros(100, 100, CV_32FC1);
    rectangle(src, Point(30, 30), Point(70, 70), Scalar(1.0f), 2);
    
    Mat edges;
    phaseCongruencyEdges(src, edges);
    
    ASSERT_EQ(edges.size(), src.size());
    ASSERT_EQ(edges.type(), CV_8UC1);
    
    int edgeCount = countNonZero(edges);
    EXPECT_GT(edgeCount, 0);
}

TEST(Imgproc_PhaseCongruencyEdges, CompareWithCanny)
{
    Mat src = Mat::zeros(200, 200, CV_8UC1);
    circle(src, Point(100, 100), 50, Scalar(255), 2);
    
    Mat edgesPC, edgesCanny;
    phaseCongruencyEdges(src, edgesPC, 4, 6, 0.1);
    Canny(src, edgesCanny, 50, 150);
    
    // Both should detect edges
    int countPC = countNonZero(edgesPC);
    int countCanny = countNonZero(edgesCanny);
    
    EXPECT_GT(countPC, 0);
    EXPECT_GT(countCanny, 0);
    
    // Phase Congruency might detect more edges due to its robustness
    // but exact counts depend on parameters
}

}} // namespace
