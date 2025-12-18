#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace cv;
using namespace std;

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

    int rows = 10000;
    int cols = 2000;
    
    std::cout << "Benchmarking cv::sort with " << rows << "x" << cols << " matrix..." << std::endl;

    // Initialize with random data
    Mat src(rows, cols, CV_32F);
    randu(src, Scalar(0), Scalar(1000));
    Mat dst;

    // Warmup
    cv::sort(src, dst, SORT_EVERY_ROW + SORT_ASCENDING);

    // Measure time
    auto start = std::chrono::high_resolution_clock::now();
    
    int iterations = 10;
    for(int i=0; i<iterations; ++i) {
        cv::sort(src, dst, SORT_EVERY_ROW + SORT_ASCENDING);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    
    double avg_time = diff.count() / iterations;
    
    std::cout << "Average time per sort: " << avg_time * 1000.0 << " ms" << std::endl;
    std::cout << "Total rows processed: " << rows * iterations << std::endl;

    return 0;
}
