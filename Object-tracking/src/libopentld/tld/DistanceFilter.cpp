#include "DistanceFilter.h"

namespace tld {

DistanceFilter::DistanceFilter()
{
    maxDistance = 26;
    prevBB = NULL;
}

bool DistanceFilter::filter(int *window, int idx)
{
    if (!prevBB || !window)
        return true;

    if (prevBB->width > 0 && prevBB->height > 0)
    {
        int euclidian_distance_sqr = (prevBB->x - window[0])*(prevBB->x - window[0]) + (prevBB->y - window[1])*(prevBB->y - window[1]);
        if ( euclidian_distance_sqr > (maxDistance*maxDistance) )
        {
            if ( idx % 2 == 0 )
            {
                return false;
            }
        }
    }

    return true;
}

} // namespace tld
