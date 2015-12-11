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
 * NNClassifier.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef NNCLASSIFIER_H_
#define NNCLASSIFIER_H_

#include <vector>

#include <opencv/cv.h>

#include "NormalizedPatch.h"
#include "DetectionResult.h"

#include "ml.h"

namespace tld
{

/**
 * @brief The nearest neighbor classifier.
 *
 */
class NNClassifier
{
  
public:
    bool enabled; //!< enables nearest neighbor сlassifier

    int *windows; //!< the array of windows
    float thetaFP; //!< lower threshold for confidence in lerning of NNClassifier
    float thetaTP; //!< upper threshold for confidence in lerning and filter of NNClassifier
    DetectionResult *detectionResult; //!< the result of detection
    std::vector<NormalizedPatch>* falsePositives;//!< the vector of negative patches
    std::vector<NormalizedPatch>* truePositives;//!< the vector of positives patches
    CvKNearest knn;
    int K;
    bool scratch;
    /**
     * @brief This constructor initialize thresholds and vectors.
     *
     */
    NNClassifier();

    /**
     * @brief Clears and releases the vectors.
     *
     */
    virtual ~NNClassifier();

    /**
     * @brief Clears the vectors.
     *
     */
    void release();

    /**
     * @brief Compares patch to positive and negative patches.
     *
     * @param patch
     *          the patch
     * @return the distance
     */
    float classifyPatch(NormalizedPatch *patch);

    /**
     * @brief Compares patch (obtain from the image and the bounding box) to positive and negative patches.
     *
     * @param img
     *          the image
     * @param bb
     *          the bounding box
     * @return the distance
     */
    float classifyBB(const cv::Mat &img, cv::Rect *bb);

    /**
     * @brief Compares patch (obtain from the image and the index of window) to positive and negative patches
     *
     * @param img
     *          the image
     * @param windowIdx
     *          the index of window
     * @return the distance
     */
    float classifyWindow(const cv::Mat &img, int windowIdx);

    /**
     * @brief Learns the nearest neighbor classifier. Distributes pathes to negative or positives patches or not.
     *
     * @param patches
     *          the vector of patches
     */
    void learn(std::vector<NormalizedPatch> patches);

    /**
     * @brief Checks if the patche is the potential positive patch.
     *
     * @param img
     *          the image
     * @param windowIdx
     *          the index of window
     * @return true, if the patch is filtered; false, otherwise
     */
    bool filter(const cv::Mat &img, int windowIdx);
};

} /* namespace tld */
#endif /* NNCLASSIFIER_H_ */
