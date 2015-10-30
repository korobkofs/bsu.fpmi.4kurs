#ifndef DISTANCEFILTER_H
#define DISTANCEFILTER_H

#include <opencv/cv.h>

namespace tld {

class DistanceFilter
{
public:
    int maxDistance; //!< max distance for filter
    cv::Rect *prevBB; //!< the previous bounding box

    DistanceFilter();

    bool filter(int * windows, int idx);
};

} /* namespace tld */

#endif // DISTANCEFILTER_H
