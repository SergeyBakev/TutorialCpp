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

    bool GBoundingBox::Set(const GBoundingBox& bbx)
    {
        *this = bbx;
        return IsValid();
    }

    bool GBoundingBox::Set(const glm::vec3& P, int bGrowBox)
    {
        if (!bGrowBox || !IsValid())
        {
            min_ = P;
            max_ = P;
        }
        else
        {
            if (P.x < Min().x) min_.x = P.x; else if (Max().x < P.x) max_.x = P.x;
            if (P.y < Min().y) min_.y = P.y; else if (Max().y < P.y) max_.y = P.y;
            if (P.z < Min().z) min_.z = P.z; else if (Max().z < P.z) max_.z = P.z;
        }
        return true;
    }


    bool GBoundingBox::GetCorners(glm3Vectors& corners) const
    {
        corners.clear();
        if (!IsValid())
            return false;

        corners.resize(8);

        corners[0].x = Min().x; corners[0].y = Min().y; corners[0].z = Min().z;
        corners[1].x = Max().x; corners[1].y = Min().y; corners[1].z = Min().z;
        corners[2].x = Max().x; corners[2].y = Max().y; corners[2].z = Min().z;
        corners[3].x = Min().x; corners[3].y = Max().y; corners[3].z = Min().z;

        corners[4].x = Min().x; corners[4].y = Min().y; corners[4].z = Max().z;
        corners[5].x = Max().x; corners[5].y = Min().y; corners[5].z = Max().z;
        corners[6].x = Max().x; corners[6].y = Max().y; corners[6].z = Max().z;
        corners[7].x = Min().x; corners[7].y = Max().y; corners[7].z = Max().z;
        return true;
    }


    bool GBoundingBox::Union(const GBoundingBox& bbox)
    {
        if (IsValid())
        {
            if (bbox.IsValid())
            {
                if (bbox.Min().x < Min().x)
                    min_.x = bbox.Min().x;
                if (bbox.Min().y < Min().y)
                    min_.y = bbox.Min().y;
                if (bbox.Min().z < Min().z)
                    min_.z = bbox.Min().z;

                if (bbox.Max().x > Max().x)
                    max_.x = bbox.Max().x;
                if (bbox.Max().y > Max().y)
                    max_.y = bbox.Max().y;
                if (bbox.Max().z > Max().z)
                    max_.z = bbox.Max().z;
            }
        }
        else if (bbox.IsValid())
            *this = bbox;
        else
        {
            //_ASSERT(0);
        }

        return IsValid();

    }

    double GBoundingBox::MaximumDistanceTo(const glm::vec3& P) const
    {
        glm::vec3 V;

        V.x = ((P.x < 0.5 * (Min().x + Max().x)) ? Max().x : Min().x) - P.x;
        V.y = ((P.y < 0.5 * (Min().x + Max().y)) ? Max().y : Min().y) - P.y;
        V.z = ((P.z < 0.5 * (Min().x + Max().z)) ? Max().z : Min().z) - P.z;

        return glm::length(V);
    }

    glm::vec3 GBoundingBox::Center() const
    {
        _ASSERT(IsValid());
        return 0.5f * (Max() + Min());
    }

    bool GBoundingBox::IsValid() const
    {
        return min_.x <= max_.x && min_.y <= max_.y && min_.z <= max_.z;

    }

    glm::vec3 GBoundingBox::Diagonal() const
    {
        _ASSERT(IsValid());
        return max_ - min_;
    }

    
}
