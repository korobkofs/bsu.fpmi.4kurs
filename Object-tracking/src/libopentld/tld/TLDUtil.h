/*  Copyright 2011 AIT Austrian Institute of Technology
*
*   This file is part of OpenTLD.
*
*   OpenTLD is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   OpenTLD is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/*
 * util.h
 *
 *  Created on: 30.05.2011
 *      Author: Georg Nebehay
 */

#ifndef TLDUTIL_H_
#define TLDUTIL_H_

#include <utility>

#include <opencv/cv.h>

namespace tld
{

template <class T1, class T2>
/**
 * @brief Converts the bouning box to other type.
 *
 * @param src
 *          the source bouning box
 * @param dest
 *          the destination bouning box
 */
void tldConvertBB(T1 *src, T2 *dest)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}

template <class T>
/**
 * @brief Copies the bouning box to the other.
 *
 *
 * @param src
 *          the source bouning box
 * @param dest
 *          the destination bouning box
 */
void tldCopyBB(T *src, T *dest)
{
    tldConvertBB<T, T>(src, dest);
}

template <class T>
/**
 * @brief Copies the boundary to the array (size 4).
 *
 * @param x
 *          the x coordinate of the left top point of boundary
 * @param y
 *          the y coordinate of the left top point of boundary
 * @param width
 *          the width boundary
 * @param height
 *          the height boundary
 * @param array
 *          the destination array
 */
void tldCopyBoundaryToArray(T x, T y, T width, T height, T *array)
{
    array[0] = x;
    array[1] = y;
    array[2] = width;
    array[3] = height;
}

template <class T>
/**
 * @brief Extracts the date from array (size 4).
 *
 * @param boundary
 *          the source array.comparing
 * @param x
 *          the 0 element of array
 * @param y
 *          the 1 element of array
 * @param width
 *          the 2 element of array
 * @param height
 *          the 3 element of array
 */
void tldExtractDimsFromArray(T *boundary, T *x, T *y, T *width, T *height)
{
    *x = boundary[0];
    *y = boundary[1];
    *width = boundary[2];
    *height = boundary[3];
}

template <class T>
/**
 * @brief Converts the rectangle to array (size 4).
 *
 * @param rect
 *          the source rectangle
 * @param boundary
 *          the destination array
 */
void tldRectToArray(cv::Rect rect, T *boundary)
{
    boundary[0] = rect.x;
    boundary[1] = rect.y;
    boundary[2] = rect.width;
    boundary[3] = rect.height;
}

template <class T>
/**
 * @brief Converts the array (size 4) to rectangle.
 *
 * @param boundary
 *          the array
 * @return the rectangle
 */
cv::Rect tldArrayToRect(T *boundary)
{
    cv::Rect rect;
    rect.x = boundary[0];
    rect.y = boundary[1];
    rect.width = boundary[2];
    rect.height = boundary[3];

    return rect;
}


/**
 * @brief Checks whether the one bouning box is completely inside the other.
 *
 * @param bb1
 *          the one bouning box
 * @param bb2
 *          the other bouning box
 * @return 1, if the one bouning box is completely inside the other; 0, otherwise
 */
int tldIsInside(int *bb1, int *bb2);

/**
 * @brief Converts the rectangle to two points.
 *
 * @param rect
 *          the rectangle
 * @param p1
 *          the left top point
 * @param p2
 *          the right bottom point
 */
void tldRectToPoints(CvRect rect, CvPoint *p1, CvPoint *p2);

/**
 * @brief Converts the array (size 4) to two points.
 *
 * @param bb
 *          the array (bouning box)
 * @param p1
 *          the left top point
 * @param p2
 *          the right bottom point
 */
void tldBoundingBoxToPoints(int *bb, CvPoint *p1, CvPoint *p2);

/**
 * @brief Returns mean-normalized patch, the image must be greyscale.
 *
 * @param img
 *          the source image
 * @param output
 *          the destination array (patch)
 */
void tldNormalizeImg(const cv::Mat &img, float *output);


/**
 * @brief Extracts normalized patch from the image by rectangle.
 *
 * @param img
 *          the source image
 * @param x
 *          the x coordinate of the left top point of reactangle
 * @param y
 *          the y coordinate of the left top point of reactangle
 * @param w
 *          the width reactangle
 * @param h
 *          the height reactangle
 * @param output
 *          the destination array (patch)
 */
void tldExtractNormalizedPatch(const cv::Mat &img, int x, int y, int w, int h, float *output);

/**
 * @brief Extracts normalized patch from the image by boundary (array).
 *
 * @param img
 *          the source image
 * @param boundary
 *          the array
 * @param output
 *         the destination array (patch)
 */
void tldExtractNormalizedPatchBB(const cv::Mat &img, int *boundary, float *output);

/**
 * @brief Extracts normalized patch from the image by reactangle.
 *
 * @param img
 *          the source image
 * @param rect
 *          the rectangle
 * @param output
 *          the destination array (patch)
 */
void tldExtractNormalizedPatchRect(const cv::Mat &img, cv::Rect *rect, float *output);

/**
 * @brief Extracts the subimage by the reactangle.
 *
 * @param img
 *          the source image
 * @param subImage
 *          the destination subimage
 * @param boundary
 *          the array (size 4)
 */
void tldExtractSubImage(const cv::Mat &img, cv::Mat &subImage, int *boundary);

/**
 * @brief Extracts the subimage by the reactangle.
 *
 * @param img
 *          the source image
 * @param subImage
 *          the destination subimage
 * @param x
 *          the x coordinate of the left top point of reactangle
 * @param y
 *          the y coordinate of the left top point of reactangle
 * @param w
 *          the width reactangle
 * @param h
 *          the height reactangle
 */
void tldExtractSubImage(const cv::Mat &img, cv::Mat &subImage, int x, int y, int w, int h);

/**
 * @brief It isn't realized.
 *
 * @param value
 * @param n
 * @return float
 */
float tldCalcMean(float *value, int n);

/**
 * @brief Calculates the variance.
 *
 * @param value
 *          the array
 * @param n
 *          the size of array
 * @return the value of variance
 */
float tldCalcVariance(float *value, int n);

/**
 * @brief Checks whether the second parameter of one pair is more then the second parameter of other pair.
 *
 * @param bb1
 *          the one pair
 * @param bb2
 *          the other pair
 * @return the result of comparing
 */
bool tldSortByOverlapDesc(std::pair<int, float> bb1 , std::pair<int, float> bb2);

/**
 * @brief Creates the copy of rectangle
 *
 * @param r
 *          the source rectangle
 * @return the new rectangle
 */
cv::Rect *tldCopyRect(cv::Rect *r);

//TODO: Change function names
/**
 * @brief Calculates the share overlap of two rectangle (the share the intersection area in the occupy area).
 *
 * @param r1
 *          the one rectangle
 * @param r2
 *          the two rectangle
 * @return the share overlap(0 to 1)
 */
float tldOverlapRectRect(cv::Rect r1, cv::Rect r2);

/**
 * @brief Calculates the share overlap of one window and the indexed windows (according).
 *
 * @param windows
 *          the array of windows
 * @param numWindows
 *          the number of windows
 * @param index
 *          the index of one window
 * @param indices
 *          the array of the indices of windows
 * @param overlap
 *          the destination array
 */
void tldOverlapOne(int *windows, int numWindows, int index, std::vector<int> * indices, float *overlap);

/**
 * @brief Calculates the share overlap of windows and the boundary.
 *
 * @param windows
 *          the array of windows
 * @param numWindows
 *          the number of windows
 * @param boundary
 *          the array (size 4)
 * @param overlap
 *          the destination array
 */
void tldOverlap(int *windows, int numWindows, int *boundary, float *overlap);

/**
 * @brief Calculates the share overlap of windows and the rectangle.
 *
 * @param windows
 *          the array of windows
 * @param numWindows
 *          the number of windows
 * @param boundary
 *          the rectangle
 * @param overlap
 *          the destination array
 */
void tldOverlapRect(int *windows, int numWindows, cv::Rect *boundary, float *overlap);

#endif /* UTIL_H_ */

} /* End Namespace */
