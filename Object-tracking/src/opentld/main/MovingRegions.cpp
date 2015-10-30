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

bool Main::isMotionInBox(cv::Mat frame1, cv::Mat frame2) {
  Mat mask;
  Mat frame1_blurred;
  Mat frame2_blurred;

  GaussianBlur( frame1, frame1_blurred, Size( 3, 3 ), -1 );
  GaussianBlur( frame2, frame2_blurred, Size( 3, 3 ), -1 );
  absdiff( frame1_blurred, frame2_blurred, mask);
  cv::threshold( mask, mask,  MOTION_THRESHOLD, 255, THRESH_BINARY );
  morphologyEx( mask, mask, MORPH_CLOSE, Mat() );
  Mat openingKernel = Mat( 5, 5, CV_8UC1);
  morphologyEx( mask, mask, MORPH_OPEN, openingKernel,
                  Point( -1, -1 ), 1, BORDER_CONSTANT, Scalar( 0 ) );

  if (countNonZero(mask) > 0) {
	return true;
  } else {
	return false;
  }
}

cv::Rect Main::maxMovingRegion(cv::Mat& last_gray) {
   bool isRegionFound = false;
   bool isMotionFound = false;
   Rect maxRegion = Rect(0, 0, 0, 0);
   vector<Rect>::iterator endIt;
   Mat motionHistoryImage = Mat( last_gray.size(), CV_32FC1 );
   Mat segmask = Mat( last_gray.size(), CV_32FC1 );
   Mat mask;
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
   Mat current_gray;
   Mat frame;
   Mat frame_gray;
   Mat current_blurred;
   Mat last_blurred;
   IplImage *img = NULL;
   IplImage c_mhi;
   IplImage c_segmask;
   CvSeq* seq;
   Rect comp_rect;
   CvMemStorage* storage = cvCreateMemStorage(0);

do {
      if(!imAcqHasMoreFrames(imAcq)) {
          cvReleaseMemStorage(&storage);
          return Rect(0, 0, 0, 0);
      }
   img = imAcqGetImg(imAcq);
   frame = cvarrToMat(img);
   cvtColor(frame, frame_gray, CV_BGR2GRAY);
   frame_gray.copyTo(current_gray);
   gui->showImage(img);
   IMG_RELEASE(img);
   cvWaitKey(1);
   //gui->writeImage(img);
   //writer.write(frame);
   if (Main::isMotionInBox(last_gray,current_gray)) {
      isMotionFound = true;
   }
   current_gray.copyTo(last_gray);
} while (!isMotionFound);

GaussianBlur(last_gray, last_blurred, Size( 3, 3 ), -1 );

do {
   motionHistoryImage.release();
   motionHistoryImage = Mat::zeros( last_gray.rows, last_gray.cols, CV_32FC1 );
   maxRegion = Rect(0, 0, 0, 0);
   for (int i=0; i < motionHistoryDuration*fps; i++) {
       if(!imAcqHasMoreFrames(imAcq)) {cvReleaseMemStorage(&storage ); return Rect(0, 0, 0, 0);}
       img = imAcqGetImg(imAcq);
       frame = cvarrToMat(img);
      cvtColor(frame, frame_gray, CV_BGR2GRAY);
      frame_gray.copyTo(current_gray);
      gui->showImage(img);
      IMG_RELEASE(img);
      cvWaitKey(1);
      //gui->writeImage(img);
      //writer.write(frame);
      GaussianBlur( current_gray, current_blurred, Size( 3, 3 ), -1 );
      absdiff(current_blurred, last_blurred, mask );
      cv::threshold( mask, mask,  MOTION_THRESHOLD, 255, THRESH_BINARY );
      morphologyEx( mask, mask, MORPH_CLOSE, Mat() );
      Mat openingKernel = Mat( 5, 5, CV_8UC1);
      morphologyEx( mask, mask, MORPH_OPEN, openingKernel,
                      Point( -1, -1 ), 1, BORDER_CONSTANT, Scalar( 0 ) );

      updateMotionHistory(mask, motionHistoryImage, (i+1) / fps, motionHistoryDuration );
      current_blurred.copyTo(last_blurred);
   }
   current_gray.copyTo(last_gray);

   //motionHistoryImage
   c_mhi = motionHistoryImage;
   c_segmask = segmask;
   seq = cvSegmentMotion( &c_mhi, &c_segmask,
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

//find contours
vector<vector<Point> > contours;
vector<Point> one_contour;
Mat mask_new = mask(maxRegion);
findContours( mask_new, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//find bounding box
if (!contours.size()) {cvReleaseMemStorage(&storage ); return maxRegion;}
for (int i = 0; i < contours.size(); i++) {
   for (int j = 0; j < contours[i].size(); j++) {
      one_contour.push_back(contours[i][j]);
   }
}
Rect result = boundingRect( Mat(one_contour) );
result.x += maxRegion.x;
result.y += maxRegion.y;

//show maxRegion and result
cvReleaseMemStorage(&storage );
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
