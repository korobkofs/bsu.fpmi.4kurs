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
 * DetectorCascade.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef DETECTORCASCADE_H_
#define DETECTORCASCADE_H_

#include "DetectionResult.h"
#include "ForegroundDetector.h"
#include "VarianceFilter.h"
#include "EnsembleClassifier.h"
#include "Clustering.h"
#include "NNClassifier.h"


namespace tld
{

//Constants
static const int TLD_WINDOW_SIZE = 5; //!< the size of array, defining the window
static const int TLD_WINDOW_OFFSET_SIZE = 6; //!< the size of array, defining the window offset

/**
 * @brief The detector cascade.
 *
 */
class DetectorCascade
{
    //Working data
    int numScales; //!< the number of scales
    cv::Size *scales; //!< the array of scales
public:
    //Configurable members
    int minScale; //!< the minimum scale
    int maxScale; //!< the maximum scale
    bool useShift; //!< if true the shift of window is used
    float shift; //!< the shift of window
    int minSize; //!< the minimum size of scan windows
    int numFeatures; //!< the number of features
    int numTrees; //!< the number of trees

    //Needed for init
    int imgWidth; //!< the width of image
    int imgHeight; //!< the height of image
    int imgWidthStep; //!< the width step of image
    int objWidth; //!< the width of bounding box of object
    int objHeight; //!< the height of bounding box of object

    int numWindows; //!< the number of windows
    int *windows; //!< the windows
    int *windowOffsets; //!< the window offsets

    //State data
    bool initialised; //!< true, if the initialize was done.

    //Components of Detector Cascade
    ForegroundDetector *foregroundDetector; //!< the foreground detector
    VarianceFilter *varianceFilter; //!< the variance filter
    EnsembleClassifier *ensembleClassifier; //!< the ensemble classifier
    Clustering *clustering; //!< the clustering
    NNClassifier *nnClassifier; //!< the nearest neighbor classifier

    DetectionResult *detectionResult; //!< the result of detection

    /**
     * @brief Propagates the data to every members of detector cascade.
     *
     */
    void propagateMembers();

    /**
     * @brief This constructor initialize by default.
     *
     */
    DetectorCascade();

    /**
     * @brief This destructor releases the memory.
     *
     */
    ~DetectorCascade();

    /**
     * @brief  Creates offsets initialize by default.
     *
     */
    void init();

    /**
     * @brief Creates offsets that can be added to bounding boxes
     * \n offsets are contained in the form delta11, delta12,... (combined index of dw and dh)
     * \n Order: scale->tree->feature
     * \n windowOffsets:
     * \n         x1-1,y1-1
     * \n         x1-1,y2
     * \n         x2,y1-1
     * \n         x2,y2
     * \n         pointer to features for this scale
     * \n         area of bounding box
     *
     */
    void initWindowOffsets();

    /**
     * @brief Returns number of bounding boxes, bounding boxes, number of scales,
     * \n scales bounding boxes are stored in an array of size 5*numBBs using the format <x y w h scaleIndex>
     * \n scales are stored using the format <w h>
     */
    void initWindowsAndScales();

    /**
     * @brief Releases memory.
     *
     */
    void release();

    /**
     * @brief Cleans the previous detection result.
     *
     */
    void cleanPreviousData();

    /**
     * @brief Detects the Object in image.
     *
     * @param img
     *          the image
     */
    void detect(const cv::Mat &img);
};

} /* namespace tld */
#endif /* DETECTORCASCADE_H_ */
