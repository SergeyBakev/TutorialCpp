#include "pch.h"
#include "GBoundingBox.h"

namespace Common
{
    GBoundingBox::GBoundingBox(const Point& min, const Point& max) : min_(min) , max_(max)
    {
    }

    GBoundingBox::Point GBoundingBox::Min() const
    {
        return min_;
    }

    GBoundingBox::Point GBoundingBox::Min(const Point& min)
    {
        min_ = min;
        return min_;
    }

    GBoundingBox::Point GBoundingBox::Max() const
    {
        return max_;
    }
    GBoundingBox::Point GBoundingBox::Max(const Point& max)
    {
        max_ = max;
        return max_;

    }
}
