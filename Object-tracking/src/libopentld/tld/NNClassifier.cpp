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
 * NNClassifier.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#include "NNClassifier.h"

#include "DetectorCascade.h"
#include "TLDUtil.h"

using namespace std;
using namespace cv;

namespace tld
{

NNClassifier::NNClassifier() : thetaTP(.65), knn(), K(1), scratch(true)
{
    truePositives = new vector<NormalizedPatch>();
    falsePositives = new vector<NormalizedPatch>();

}

NNClassifier::~NNClassifier()
{
    release();

    delete truePositives;
    delete falsePositives;
}

void NNClassifier::release()
{
    falsePositives->clear();
    truePositives->clear();
}

float NNClassifier::classifyPatch(NormalizedPatch *patch)
{
    CvMat *sample = cvCreateMat(1, TLD_PATCH_SIZE * TLD_PATCH_SIZE, CV_32FC1);

    for(int i = 0; i < sample->cols; i++)
        sample->data.fl[i] = patch->values[i];

    float response = knn.find_nearest(sample, K);

    cvReleaseMat(&sample);
    return response;
}

float NNClassifier::classifyBB(const Mat &img, Rect *bb)
{
    NormalizedPatch patch;

    tldExtractNormalizedPatchRect(img, bb, patch.values);
    return classifyPatch(&patch);

}

float NNClassifier::classifyWindow(const Mat &img, int windowIdx)
{
    NormalizedPatch patch;

    int *bbox = &windows[TLD_WINDOW_SIZE * windowIdx];
    tldExtractNormalizedPatchBB(img, bbox, patch.values);

    return classifyPatch(&patch);
}

bool NNClassifier::filter(const Mat &img, int windowIdx)
{
    if(!enabled) 
        return true;

    return classifyWindow(img, windowIdx) > thetaTP;
}

void NNClassifier::learn(vector<NormalizedPatch> patches)
{
    CvMat* trainData = cvCreateMat(patches.size(), TLD_PATCH_SIZE * TLD_PATCH_SIZE, CV_32FC1);
    CvMat* trainClasses = cvCreateMat(patches.size(), 1, CV_32FC1);

    for(int i = 0; i < trainData->rows; i++) {
        for(int j = 0; j < trainData->cols; j++)
            trainData->data.fl[i * trainData->cols + j] = patches[i].values[j];
        trainClasses->data.fl[i] = patches[i].positive;
    }
            
    knn.train(trainData, trainClasses, 0, false, K, !scratch);

    if(scratch)
        scratch = false;

    cvReleaseMat(&trainClasses);
    cvReleaseMat(&trainData);
}


} /* namespace tld */
