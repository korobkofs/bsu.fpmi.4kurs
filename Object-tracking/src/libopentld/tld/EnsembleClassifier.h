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
 * EnsembleClassifier.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef ENSEMBLECLASSIFIER_H_
#define ENSEMBLECLASSIFIER_H_

#include <opencv/cv.h>
#include <opencv/ml.h>
#include <iostream>
#include <vector>
 
using namespace std;
using namespace cv;

namespace tld
{

/**
 * @brief The ensemble classifier.
 *
 */
class EnsembleClassifier
{
    const unsigned char *img; //!< the current image

    CvGBTrees trees;
    bool trained;
    vector <int *> featureVectors;
    vector <float> resultVector;

    /**
     * @brief Calculates the verdict of trees (0 to 1).
     *
     * @param featureVector
     *          the vector of features
     * @return the confidence
     */
    float calcConfidence(int *featureVector);

    /**
     * @brief Calculates branch (response tree to patch).
     *
     * @param windowIdx
     *          the window index
     * @param treeIdx
     *          the tree index
     * @return the index of branch
     */
    int calcFernFeature(int windowIdx, int treeIdx);

    /**
     * @brief Calculates the vector of features (the indeces of branches for all trees).
     *
     * @param windowIdx
     *          the window index
     * @param featureVector
     *          the destination vector of features
     */
    void calcFeatureVector(int windowIdx, int *featureVector);

    /**
     * @brief Updates positive or negative branches and posteriors for all tree.
     *
     * @param featureVector
     *          the vector of features
     * @param positive
     *          if 1 , positive; if 0, negative patch
     * @param amount
     *         the amount of patches
     */
    void updatePosteriors(int *featureVector, int positive, int amount);
public:
    bool enabled; //!< enables ensemble classifier

    //Configurable members
    int numTrees; //!< the number of trees
    int numFeatures; //!< the number of fetures

    int imgWidthStep; //!< the width step of image
    int numScales; //!< the number of scales
    cv::Size *scales; //!< the array of scales

    int *windowOffsets; //!< the window offsets
    int *featureOffsets; //!< the feature offsets
    float *features; //!< the array of features

    int numIndices; //!< the number of indices

    float *posteriors; //!< the vector of verdicts
    int *positives; //!< the positive branches
    int *negatives; //!< the negative branches

    DetectionResult *detectionResult; //!< the result of detection

    void train();
    /**
     * @brief This constructor initializes the class variables by default.
     *
     */
    EnsembleClassifier();

    /**
     * @brief This destructor releases the memory.
     *
     */
    virtual ~EnsembleClassifier();

    /**
     * @brief Initializes the all class variables.
     *
     */
    void init();

    /**
    * @brief Generates random measurements in the format <x1,y1,x2,y2>.
    *
    */
    void initFeatureLocations();

    /**
    * @brief Creates offsets that can be added to bounding boxes
    * \n offsets are contained in the form delta11, delta12,... (combined index of dw and dh)
    * \n  Order: scale.tree->feature
    *
    */
    void initFeatureOffsets();

    /**
     * @brief Initializes the variable such as posteriors, positives, negatives.
     *
     */
    void initPosteriors();

    /**
     * @brief Releases the memory.
     *
     */
    void release();

    /**
     * @brief Sets the current image.
     *
     * @param img
     *          the image
     */
    void nextIteration(const cv::Mat &img);

    /**
     * @brief Classifies the window (positive or negative).
     *
     * @param windowIdx
     *     the index of window
     */
    void classifyWindow(int windowIdx);

    /**
     * @brief Updates positive or negative branches and posteriors for one tree.
     *
     * @param treeIdx
     *          the index of tree
     * @param idx
     *          the index of branch
     * @param positive
     *          if 1 , positive; if 0, negative patch
     * @param amount
     *          the amount of patches
     */
    void updatePosterior(int treeIdx, int idx, int positive, int amount);

    /**
     * @brief Update positive or negative patches.
     *
     * @param boundary
     *          not used
     * @param positive
     *          if 1 , positive; if 0, negative patch
     * @param featureVector
     *          the vector of features
     */
    void learn(int *boundary, int positive, int *featureVector);

    /**
     * @brief Filtered the windows.
     *
     * @param i
     *      the index of window
     * @return true, if the  window isn't filtered; false, otherwise
     */
    bool filter(int i);
};

} /* namespace tld */
#endif /* ENSEMBLECLASSIFIER_H_ */
