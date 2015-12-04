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
 * EnsembleClassifier.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#include "DetectorCascade.h"

#include <cstdlib>
#include <cmath>

#include <opencv/cv.h>
#include <opencv/ml.h>

#include "EnsembleClassifier.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

namespace tld
{

//TODO: Convert this to a function
#define sub2idx(x,y,widthstep) ((int) (floor((x)+0.5) + floor((y)+0.5)*(widthstep)))

EnsembleClassifier::EnsembleClassifier() :
    features(NULL),
    featureOffsets(NULL),
    posteriors(NULL),
    positives(NULL),
    negatives(NULL)
{
    numTrees = 10;
    numFeatures = 13;
    enabled = true;
    trained = false;
}

EnsembleClassifier::~EnsembleClassifier()
{
    release();
}


void EnsembleClassifier::init()
{
    numIndices = pow(2.0f, numFeatures);

    initFeatureLocations();
    initFeatureOffsets();
    initPosteriors();
}

void EnsembleClassifier::release()
{
    delete[] features;
    features = NULL;
    delete[] featureOffsets;
    featureOffsets = NULL;
    delete[] posteriors;
    posteriors = NULL;
    delete[] positives;
    positives = NULL;
    delete[] negatives;
    negatives = NULL;

    featureVectors.clear();
    positiveVector.clear();
}

void EnsembleClassifier::initFeatureLocations()
{
    int size = 2 * 2 * numFeatures * numTrees;

    features = new float[size];

    for(int i = 0; i < size; i++)
    {
        features[i] = rand() / (float)RAND_MAX;
    }

}

void EnsembleClassifier::initFeatureOffsets()
{

    featureOffsets = new int[numScales * numTrees * numFeatures * 2];
    int *off = featureOffsets;

    for(int k = 0; k < numScales; k++)
    {
        Size scale = scales[k];

        for(int i = 0; i < numTrees; i++)
        {
            for(int j = 0; j < numFeatures; j++)
            {

                float *currentFeature  = features + (4 * numFeatures) * i + 4 * j;
                *off++ = sub2idx((scale.width - 1) * currentFeature[0] + 1, (scale.height - 1) * currentFeature[1] + 1, imgWidthStep); //We add +1 because the index of the bounding box points to x-1, y-1
                *off++ = sub2idx((scale.width - 1) * currentFeature[2] + 1, (scale.height - 1) * currentFeature[3] + 1, imgWidthStep);
            }
        }
    }
}

void EnsembleClassifier::initPosteriors()
{
    posteriors = new float[numTrees * numIndices];
    positives = new int[numTrees * numIndices];
    negatives = new int[numTrees * numIndices];

    for(int i = 0; i < numTrees; i++)
    {
        for(int j = 0; j < numIndices; j++)
        {
            posteriors[i * numIndices + j] = 0;
            positives[i * numIndices + j] = 0;
            negatives[i * numIndices + j] = 0;
        }
    }
}

void EnsembleClassifier::nextIteration(const Mat &img)
{
    if(!enabled) return;

    this->img = (const unsigned char *)img.data;
}

//Classical fern algorithm
int EnsembleClassifier::calcFernFeature(int windowIdx, int treeIdx)
{

    int index = 0;
    int *bbox = windowOffsets + windowIdx * TLD_WINDOW_OFFSET_SIZE;
    int *off = featureOffsets + bbox[4] + treeIdx * 2 * numFeatures; //bbox[4] is pointer to features for the current scale

    for(int i = 0; i < numFeatures; i++)
    {
        index <<= 1;

        int fp0 = img[bbox[0] + off[0]];
        int fp1 = img[bbox[0] + off[1]];

        if(fp0 > fp1)
        {
            index |= 1;
        }

        off += 2;
    }

    return index;
}

void EnsembleClassifier::calcFeatureVector(int windowIdx, int *featureVector)
{
    for(int i = 0; i < numTrees; i++)
    {
        featureVector[i] = calcFernFeature(windowIdx, i);
    }
}

float EnsembleClassifier::calcConfidence(int *featureVector)
{
    if (!trained) {
        return 1;
    } else {
        vector <float> featurePredictVector;

        for (int i =0; i < numTrees; i++) {
            featurePredictVector.push_back(featureVector[i]);
        }

        Mat featureMat(featurePredictVector);
        return boost.predict(featureMat);
    }
}

void EnsembleClassifier::classifyWindow(int windowIdx)
{
    int *featureVector = detectionResult->featureVectors + numTrees * windowIdx;
    calcFeatureVector(windowIdx, featureVector);

    detectionResult->posteriors[windowIdx] = calcConfidence(featureVector);
}

bool EnsembleClassifier::filter(int i)
{
    if(!enabled) return true;

    classifyWindow(i);

    if(detectionResult->posteriors[i] < 0.5) return false;

    return true;
}

void EnsembleClassifier::updatePosterior(int treeIdx, int idx, int positive, int amount)
{
    int arrayIndex = treeIdx * numIndices + idx;
    (positive) ? positives[arrayIndex] += amount : negatives[arrayIndex] += amount;
    posteriors[arrayIndex] = ((float) positives[arrayIndex]) / (positives[arrayIndex] + negatives[arrayIndex]) / (float) numTrees;
}

void EnsembleClassifier::updatePosteriors(int *featureVector, int positive, int amount)
{

    for(int i = 0; i < numTrees; i++)
    {

        int idx = featureVector[i];
        updatePosterior(i, idx, positive, amount);

    }
}

void EnsembleClassifier::learn(int *boundary, int positive, int *featureVector)
{
    positive = positive > 0 ? 1 : -1;

    featureVectors.push_back(featureVector);
    positiveVector.push_back((float)positive);
}

void EnsembleClassifier::train()
{
    int size = featureVectors.size();
    Mat trainedPositivesMat(positiveVector);
    Mat trainedDataMat(size, numTrees, CV_32FC1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < numTrees; j++) {
            trainedDataMat.at<float>(i, j) = (float)featureVectors[i][j];
        }
    }

    boost.train(trainedDataMat,
        CV_ROW_SAMPLE,
        trainedPositivesMat);

    trained = true;
}

} /* namespace tld */
