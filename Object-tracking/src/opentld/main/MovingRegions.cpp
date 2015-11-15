#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <math.h>
#include "Main.h"
#include "ImAcq.h"
using namespace cv;
using namespace std;
/**
 * @author Philip Korobko
 */

#define IMG_RELEASE(x) do { if (x) {cvReleaseImage(&(x)); (x) = NULL; } } while(0)

bool Main::isMotionInBox(IplImage frame1, IplImage frame2) {
    IplImage* mask = cvCreateImage(Size(frame1.width, frame1.height), frame1.depth, frame1.nChannels);
	IplImage* frame1_blurred = cvCreateImage(Size(frame1.width, frame1.height), frame1.depth, frame1.nChannels);
	IplImage* frame2_blurred = cvCreateImage(Size(frame2.width, frame2.height), frame2.depth, frame2.nChannels);

	CvMat gaussianKernel = getGaussianKernel(3, -1);
	cvFilter2D(&frame1, frame1_blurred, &gaussianKernel);
	gaussianKernel = getGaussianKernel(3, -1);
	cvFilter2D(&frame2, frame2_blurred, &gaussianKernel);

	cvAbsDiff(frame1_blurred, frame2_blurred, mask);
	cvThreshold(mask, mask, MOTION_THRESHOLD, 255, THRESH_BINARY);
	cvDilate(mask, mask);
	IplConvKernel* kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT);
	cvErode(mask, mask, kernel);

	bool result = cvCountNonZero(mask) > 0;

	cvReleaseImage(&mask);
	cvReleaseImage(&frame1_blurred);
	cvReleaseImage(&frame2_blurred);

	return result;
}

CvRect maxBoundingRect(CvRect a, CvRect b) {
    int left = min(a.x, b.x),
        top = min(a.y, b.y),
        right = max(a.x + a.width, b.x + b.width),
        bottom = max(a.y + a.height, b.y + b.height);
    a.x = left;
    a.width = right - left;
    a.y = top;
    a.height = bottom - top;
    return a;
}

cv::Rect Main::maxMovingRegion(cv::Mat& last_gray) {
   bool isRegionFound = false;
   bool isMotionFound = false;
   Rect maxRegion = Rect(0, 0, 0, 0);
   vector<Rect>::iterator endIt;
    IplImage* motionHistoryImage = cvCreateImage(cvSize(last_gray.cols, last_gray.rows), 32, 1);
   Mat segmask = Mat( last_gray.size(), CV_32FC1 );
   IplImage* mask = cvCreateImage(last_gray.size(), 8, 1);
   double fps;
   if (imAcq->method == IMACQ_CAM) {
       imAcq->fps=30;
       //printf("Camera\n");
   } else {
       //fps = cvGetCaptureProperty(imAcq->capture , CV_CAP_PROP_FPS);
       //printf("Video %f\n", fps);
       //fps = imAcq->fps;
       imAcq->fps = 30;
   }
   fps = imAcq->fps;
   double motionHistoryDuration = 7 / fps;
   double maxMotionGradient = 1.5 / fps;
   IplImage* current_gray = cvCreateImage(last_gray.size(), 8, 1);
   IplImage* frame;
   IplImage* current_blurred = cvCreateImage(last_gray.size(), 8, 1);
   IplImage* last_blurred = cvCreateImage(last_gray.size(), 8, 1);
   IplImage c_last_gray = last_gray;
   IplImage c_segmask;
   CvSeq* seq;
   Rect comp_rect;
   CvMemStorage* storage = cvCreateMemStorage(0);

do {
      if(!imAcqHasMoreFrames(imAcq)) {
          cvReleaseMemStorage(&storage);
          return Rect(0, 0, 0, 0);
      }
   frame = imAcqGetImg(imAcq);
   gui->showImage(frame);
   cvWaitKey(1);
   cvCvtColor(frame, current_gray, CV_BGR2GRAY);
   IMG_RELEASE(frame);
   if (Main::isMotionInBox(c_last_gray,*current_gray)) {
      isMotionFound = true;
   }
    cvCopy(current_gray, &c_last_gray);
} while (!isMotionFound);

    CvMat gaussianKernel = getGaussianKernel(3, -1);
    cvFilter2D(&c_last_gray, last_blurred, &gaussianKernel);

do {
   cvSetZero(motionHistoryImage);
   maxRegion = Rect(0, 0, 0, 0);
   for (int i=0; i < motionHistoryDuration*fps; i++) {
       if(!imAcqHasMoreFrames(imAcq)) {
           cvReleaseMemStorage(&storage );
           IMG_RELEASE(current_blurred);
           IMG_RELEASE(current_gray);
           IMG_RELEASE(last_blurred);
           IMG_RELEASE(mask);
           return Rect(0, 0, 0, 0);
       }
       frame = imAcqGetImg(imAcq);
       cvCvtColor(frame, current_gray, CV_BGR2GRAY);
      gui->showImage(frame);
      IMG_RELEASE(frame);
      cvWaitKey(1);
      //gui->writeImage(img);
      //writer.write(frame);
       gaussianKernel = getGaussianKernel(3, -1);
       cvFilter2D(current_gray, current_blurred, &gaussianKernel);
       cvAbsDiff(current_blurred, last_blurred, mask);
       cvThreshold(mask, mask, MOTION_THRESHOLD, 255, THRESH_BINARY);
       IplConvKernel* kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT);
       cvDilate(mask, mask);
       cvErode(mask, mask, kernel);

       cvUpdateMotionHistory(mask, motionHistoryImage, (i+1) / fps, motionHistoryDuration);
       cvCopy(current_blurred, last_blurred);
   }
    cvCopy(current_gray, &c_last_gray);

   c_segmask = segmask;
   seq = cvSegmentMotion( motionHistoryImage, &c_segmask,
                                   storage,
                                   motionHistoryDuration, maxMotionGradient );

    vector <Rect> target(0);
    for( int i = 0; i < seq->total; i++ ) {
        comp_rect = ((CvConnectedComp*)cvGetSeqElem( seq, i ))->rect;
        if (std::min(comp_rect.width, comp_rect.height) >= MIN_BOX_SIDE) {
            target.push_back(comp_rect);
        }
    }
    if (target.size()) {
       target = Main::unionRectangls(target);
       for( vector<Rect>::iterator it = target.begin( ); it != target.end(); it++ ) {
           if((* it).area() > maxRegion.area()
               && (* it).x > 5
               && (* it).y > 5
               && (* it).x + (* it).width < last_gray.cols - 5
               && (* it).y + (* it).height < last_gray.rows - 5) {
                maxRegion = (* it);
                isRegionFound = true;
           }
       }
   }
   cvClearMemStorage(storage);
   cvClearSeq(seq);
} while (!isRegionFound);

IMG_RELEASE(current_blurred);
IMG_RELEASE(current_gray);
IMG_RELEASE(last_blurred);
cvReleaseMemStorage(&storage );

CvMemStorage *mem = cvCreateMemStorage(0);
CvSeq *contours = 0;
cvSetImageROI(mask, maxRegion);
int n = cvFindContours(mask, mem, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
IMG_RELEASE(mask);
cvReleaseMemStorage(&mem);
if (!n) {
    return maxRegion;
}
CvRect result;
for (result = cvBoundingRect(contours, 0); contours != NULL; contours = contours->h_next) {
    result = maxBoundingRect(result, cvBoundingRect(contours, 0));
}
result.x += maxRegion.x;
result.y += maxRegion.y;

if (min(result.width, result.height) >= MIN_BOX_SIDE) {
    return result;
} else {
    return Main::maxMovingRegion(last_gray);
}
}

std::vector<cv::Rect> Main::unionRectangls(std::vector<cv::Rect> src_rect) {
    int min_x, max_x, min_y, max_y;
    Rect rect1, rect2;
    bool isMarked[src_rect.size()];
    vector <int> forLook(0);
    vector <Rect> union_result(0);

    isMarked[0]=true;
    for( int i = 1; i < src_rect.size(); i++) {
        isMarked[i]=false;
    }
    forLook.push_back(0);
    union_result.push_back(src_rect.front());
    do {
        for( int i = 0; i < forLook.size(); i++) {
           rect1 = *(src_rect.begin() + forLook.at(i));
           for( int j = 1; j < src_rect.size(); j++) {
                if (!isMarked[j]) {
                    rect2 = *(src_rect.begin() + j);
                    if((((rect1.x<=rect2.x  && rect2.x<=rect1.x+rect1.width) ||
                         (rect1.x<=rect2.x+rect2.width  && rect2.x+rect2.width<=rect1.x+rect1.width) ||
                         (rect2.x<=rect1.x  && rect1.x<=rect2.x+rect2.width) ||
                         (rect2.x<=rect1.x+rect1.width  && rect1.x+rect1.width<=rect2.x+rect2.width)) &&
                        ((rect1.y<=rect2.y  && rect2.y<=rect1.y+rect1.height) ||
                         (rect1.y<=rect2.y+rect2.height  && rect2.y+rect2.height<=rect1.y+rect1.height) ||
                         (rect2.y<=rect1.y  && rect1.y<=rect2.y+rect2.height) ||
                         (rect2.y<=rect1.y+rect1.height  && rect1.y+rect1.height<=rect2.y+rect2.height)))) {
                             min_x = std::min(union_result.back().x, rect2.x);
                             max_x = std::max(union_result.back().x + union_result.back().width, rect2.x + rect2.width);
                             min_y = std::min(union_result.back().y, rect2.y);
                             max_y = std::max(union_result.back().y + union_result.back().height, rect2.y + rect2.height);
                             isMarked[j]=true;
                             forLook.push_back(j);
                             union_result.back().x = min_x;
                             union_result.back().y = min_y;
                             union_result.back().width = max_x - min_x;
                             union_result.back().height = max_y - min_y;
                    }
                }
           }
       }
       forLook.clear();
       for( int k = 1; k < src_rect.size(); k++) {
           if(!isMarked[k]) {
                isMarked[k]=true;
                forLook.push_back(k);
                union_result.push_back(*(src_rect.begin() + k));
                break;
           }
       }
    } while(!forLook.empty());
    return union_result;
}
