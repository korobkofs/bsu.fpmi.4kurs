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
 * imAcq.h
 *
 *  Created on: 18 May 2011
 *      Author: Georg Nebehaiy
 *  Modified on: 26th May 2013
 *      By: Jonathan Senecal
 */

#ifndef IMACQ_IMPL_H_
#define IMACQ_IMPL_H_

#include <opencv/highgui.h>

/**
 * @brief Capturing method
 *
 */
enum ImacqMethod
{
    IMACQ_IMGS, //!< Images
    IMACQ_CAM, //!< Camera
    IMACQ_VID, //!< Video
    IMACQ_LIVESIM, //!< Livesim
    IMACQ_STREAM //!< Stream
};

/**
 * @brief The structure for the capture frames.
 *
 */
typedef struct
{
    int method;//!< the method of getting frames
    const char *imgPath;//!< the path to source images
    CvCapture *capture;//!< the capturing object
    int lastFrame;//!< the number of last frame
    int currentFrame;//!< the number of current frame
    int startFrame;//!< the number of first frame
    int camNo;//!< the number of camera
    double startTime;//!< the start time
    float fps;//!< the frequency of video
} ImAcq ;

/**
 * @brief Allocates of memory and initializes date (except the capture) of stucture ImAcq
 *
 * @return the allocated stucture ImAcq
 */
ImAcq *imAcqAlloc();

/**
 * @brief Initializes the capture.
 *
 * @param imAcq
 *          the initializing stucture ImAcq
 */
void imAcqInit(ImAcq *imAcq);

/**
 * @brief It isn't realized.
 */
void imAcqRelease(ImAcq *imAcq);

/**
 * @brief Sets the next frame number (IMACQ_VID)
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @param nFrame
 *          the set next frame number
 */
void imAcqVidSetNextFrameNumber(ImAcq *imAcq, int nFrame);

/**
 * @brief Gets the next frame number (IMACQ_VID)
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return the next frame number
 */
int imAcqVidGetNextFrameNumber(ImAcq *imAcq);

/**
 * @brief Gets the number of frames (IMACQ_VID)
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return the number of frames
 */
int imAcqVidGetNumberOfFrames(ImAcq *imAcq);

/**
 * @brief Determines if there are more frame.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return 0, if there are more frame; 1, otherwise
 */
int imAcqHasMoreFrames(ImAcq *imAcq);

/**
 * @brief Gets the next image with twice increase the count of frames.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return the image
 */
IplImage *imAcqGetImgAndAdvance(ImAcq *imAcq);

/**
 * @brief Gets the next image with increase the count of frames.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return the image
 */
IplImage *imAcqGetImg(ImAcq *imAcq);

/**
 * @brief Gets image by the frame number.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @param fNo
 *          the frame number
 * @return the image
 */
IplImage *imAcqGetImgByFrame(ImAcq *imAcq, int fNo);

/**
 * @brief Gets image by current time.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return the image
 */
IplImage *imAcqGetImgByCurrentTime(ImAcq *imAcq);

/**
 * @brief Loads the image.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @param path
 *          the path to the image
 * @return the images
 */
IplImage *imAcqLoadImg(ImAcq *imAcq, char *path);

/**
 * @brief Loads the current frame.
 *
 * @param imAcq
 *          the using stucture ImAcq
 * @return the image.
 */
IplImage *imAcqLoadCurrentFrame(ImAcq *imAcq);

/**
 * @brief It isn't realized.
 */
IplImage *imAcqLoadVidFrame(CvCapture *capture);

/**
 * @brief Grabs the image (for IMACQ_CAM, IMACQ_VID, IMACQ_STREAM).
 *
 * @param capture
 *          the source of frames
 * @return the grabed image
 */
IplImage *imAcqGrab(CvCapture *capture);

/**
 * @brief Increases the count of frames.
 *
 * @param imAcq
 *           the using stucture ImAcq
 */
void imAcqAdvance(ImAcq *imAcq);

/**
 * @brief Releases memory stucture ImAcq
 *
 * @param imAcq
 *          the releasing stucture ImAcq
 */
void imAcqFree(ImAcq *);

#endif /* IMACQ_H_ */
