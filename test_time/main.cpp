#include <iostream>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "IntegralImage.h"

using namespace std;
using namespace cv;
using namespace tld;

#define WITH_TLD 1


#define N 100
// 512 512 grayscale lena
#define filename "lena512.bmp"
// nsec += all_times_rt[i].tv_nsec + all_times_rt[i].tv_sec * (1000000000UL);
unsigned long long cv_integral[N];
#if WITH_TLD
unsigned long long tld_integral[N];
#endif


void get_time_diff (bool clock_round, const struct timespec *start,
               struct timespec *res)
{
        if (clock_round && clock_gettime (CLOCK_REALTIME, res) == 0) {
                if (res->tv_nsec < start->tv_nsec) {
                        res->tv_nsec = 1000000000 + res->tv_nsec - start->tv_nsec;
                        res->tv_sec -= 1 + start->tv_sec;
                } else {
                        res->tv_nsec -= start->tv_nsec;
                        res->tv_sec -= start->tv_sec;
                }


        } else {
                res->tv_nsec -= start->tv_nsec;
                res->tv_sec -= start->tv_sec;
        }
}

int time_measurement(unsigned long long time_seq[], string st)
{
    unsigned long long total;
    unsigned long long min, max;

    int i;

    total = 0;
    min = time_seq[0];
    max = time_seq[0];

    for(i = 0; i < N; i++) {
        total+=time_seq[i];
        if(time_seq[i] < min)
            min = time_seq[i];
        if(time_seq[i] > max)
            max = time_seq[i];
    }

    cout<< CV;
    cout<< ": total = " << total;
    cout<< " mean = " << (unsigned long long)(total/N);
    cout<< " min = " << min;
    cout<< " max = " << max << endl;

    return 0;
}

int result()
{

    time_measurement(cv_integral, "CV");
#if WITH_TLD
    time_measurement(tld_integral, "tld");
#endif


    return 0;
}

int test()
{

    int i;

    struct timespec start, end;
    IplImage *src_img=0, *cv_integral_img = 0, *cv_sqintegral_img = 0;
    cout << "read file " << filename << endl;


    src_img = cvLoadImage(filename, 0);

    Mat src_mat(src_img, true);

    printf("image parameters: depth = %d, w = %d, h = %d, channels = %d\n", src_img->depth, src_img->width, src_img->height, src_img->nChannels );


    cvNamedWindow( "original", 1 );
    cvShowImage( "original", src_img );


    waitKey(0);

    cout << "cv integral "<< endl;

    for(i = 0; i < N; i++) {

        cv_integral_img = cvCreateImage( cvSize(src_img->width+1, src_img->height+1), IPL_DEPTH_64F, 1);
        cv_sqintegral_img = cvCreateImage( cvSize(src_img->width+1, src_img->height+1), IPL_DEPTH_64F, 1);
        clock_gettime (CLOCK_REALTIME, &start);
        cvIntegral(src_img, cv_integral_img, cv_sqintegral_img);
        get_time_diff(true, &start,&end);
        //printf("nano seconds = %lu, seconds = %lu\n", end.tv_nsec, end.tv_sec);
        cv_integral[i] = (unsigned long long )end.tv_nsec + (unsigned long long)(end.tv_sec * 1000000000);
        cvReleaseImage(&cv_integral_img);
        cvReleaseImage(&cv_sqintegral_img);

    }
#if WITH_TLD
    cout << "tld integral "<< endl;

    for(i = 0; i < N; i++) {
        IntegralImage<int>* integral;
        IntegralImage<long long>* sq_integral;
        integral = new IntegralImage<int>(cv::Size(src_img->width, src_img->height));
        sq_integral = new IntegralImage<long long>(cv::Size(src_img->width, src_img->height));

        clock_gettime (CLOCK_REALTIME, &start);

        integral->calcIntImg (src_mat);
        sq_integral->calcIntImg (src_mat, true);

        get_time_diff(true, &start,&end);
        tld_integral[i] = (unsigned long long )end.tv_nsec + (unsigned long long)(end.tv_sec * 1000000000);
        delete integral;
        delete sq_integral;
    }
#endif
    cout << "end of calc integral matrix: please enter any key " << endl;
    waitKey(0);
    cvReleaseImage(&src_img);
    cvReleaseImage(&cv_integral_img);
    cvReleaseImage(&cv_sqintegral_img);
    cvDestroyAllWindows();

    return 0;
}

int main()
{
    test();
    result();
    return 0;
}

