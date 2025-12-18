#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>

using namespace cv;
using namespace std;

// ==========================================================
//  My Parallel Sort Implementation (Proposed)
// ==========================================================

template<typename T>
class Sort_Invoker : public ParallelLoopBody
{
public:
    Sort_Invoker( const Mat& _src, Mat& _dst, int _flags )
        : src(_src), dst(_dst), flags(_flags)
    {
        sortRows = (flags & 1) == SORT_EVERY_ROW;
        sortDescending = (flags & SORT_DESCENDING) != 0;
        inplace = (src.data == dst.data);
        if( sortRows )
            len = src.cols;
        else
            len = src.rows;
    }

    void operator()(const Range& range) const CV_OVERRIDE
    {
        AutoBuffer<T> buf;
        if( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        for( int i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            if( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                if( !inplace )
                {
                    const T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, sizeof(T) * len);
                }
                ptr = dptr;
            }
            else
            {
                for( int j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            if( sortDescending )
            {
                for( int j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            if( !sortRows )
                for( int j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    }

private:
    const Mat& src;
    Mat& dst;
    int flags;
    bool sortRows;
    bool sortDescending;
    bool inplace;
    int len;
};

template<typename T> 
static void my_sort_( const Mat& src, Mat& dst, int flags )
{
    int n = (flags & 1) == SORT_EVERY_ROW ? src.rows : src.cols;
    Sort_Invoker<T> body(src, dst, flags);
    // Use cv::parallel_for_
    parallel_for_(Range(0, n), body);
}

void my_sort( InputArray _src, OutputArray _dst, int flags )
{
    Mat src = _src.getMat();
    _dst.create( src.size(), src.type() );
    Mat dst = _dst.getMat();

    if( src.depth() == CV_32F )
        my_sort_<float>( src, dst, flags );
    else if( src.depth() == CV_8U )
        my_sort_<uchar>( src, dst, flags );
    else if( src.depth() == CV_32S )
        my_sort_<int>( src, dst, flags );
    // Add other types if needed
}

// ==========================================================
//  Benchmark Logic
// ==========================================================

int main() {
    // 1. Setup Data
    // Large matrix: 10,000 rows x 2,000 cols (Float)
    int rows = 10000;
    int cols = 2000;
    std::cout << ">>> Benchmarking Sort on " << rows << "x" << cols << " matrix (Float32) <<<" << std::endl;
    std::cout << ">>> CPU Cores: " << getNumberOfCPUs() << " <<<" << std::endl;

    Mat src(rows, cols, CV_32F);
    randu(src, Scalar(0), Scalar(1000));
    
    Mat dst_orig, dst_new;

    // 2. Warmup & Check Correctness
    cv::sort(src, dst_orig, SORT_EVERY_ROW + SORT_ASCENDING);
    my_sort(src, dst_new, SORT_EVERY_ROW + SORT_ASCENDING);

    double diff = cv::norm(dst_orig, dst_new, NORM_L1);
    if(diff > 0.0001) {
        std::cerr << "!!! Error: Results mismatch! Diff: " << diff << std::endl;
        return -1;
    } else {
        std::cout << "[Correctness Checked] Results match perfectly." << std::endl;
    }

    // 3. Measure Original
    double t_orig = 0;
    int iterations = 10;
    
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<iterations; ++i) {
        cv::sort(src, dst_orig, SORT_EVERY_ROW + SORT_ASCENDING);
    }
    auto end = std::chrono::high_resolution_clock::now();
    t_orig = std::chrono::duration<double>(end - start).count() / iterations;
    
    std::cout << "Original cv::sort: " << t_orig * 1000.0 << " ms" << std::endl;

    // 4. Measure New Parallel
    double t_new = 0;
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<iterations; ++i) {
        my_sort(src, dst_new, SORT_EVERY_ROW + SORT_ASCENDING);
    }
    end = std::chrono::high_resolution_clock::now();
    t_new = std::chrono::duration<double>(end - start).count() / iterations;

    std::cout << "Proposed my_sort : " << t_new * 1000.0 << " ms" << std::endl;

    // 5. Result
    std::cout << ">>> Speedup: " << t_orig / t_new << "x <<<" << std::endl;

    return 0;
}
