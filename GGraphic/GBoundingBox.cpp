#include "pch.h"
#include "GBoundingBox.h"

namespace Common
{
    GBoundingBox::GBoundingBox(const Point& min, const Point& max) : min_(min) , max_(max)
    {
    }

    GBoundingBox::GBoundingBox(const Point& pnt)
    {
        Set(pnt);
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

    bool GBoundingBox::GetPointListBoundingBox(int dim, int is_rat, int count, int stride, const float* points, GBoundingBox& tight_bbox, int bGrowBox, const glm::mat4* xform)
    {
        // bounding box workhorse
        bool rc = false;
        if (bGrowBox && !tight_bbox.IsValid())
        {
            bGrowBox = false;
        }
        if (!bGrowBox)
        {
            tight_bbox = GBoundingBox();
        }
        if (is_rat)
        {
            is_rat = 1;
        }

        if (count > 0 && dim > 0 && points && (count == 1 || stride >= dim + is_rat))
        {
            GBoundingBox bbox;
            glm::vec3 P(0.0, 0.0, 0.0);
            float w;
            int i = 0, wi;
            const auto identity = glm::identity<glm::mat4>();
            if (xform && *xform == identity)
            {
                xform = 0;
            }
            wi = dim;
            if (dim > 3)
            {
                dim = 3;
            }

            rc = true;
            if (is_rat)
            {
                // skip bogus starting points
                while (count > 0 && points[wi] == 0.0)
                {
                    count--;
                    points += stride;
                    rc = false;
                }
                if (count <= 0)
                    return false;
            }
         
            memcpy(&bbox.min_.x, points, dim * sizeof(bbox.min_.x));
            if (is_rat)
            {
                w = 1.0f / points[wi];
                bbox.min_.x *= w; bbox.min_.y *= w; bbox.min_.z *= w;
            }
            if (xform)
            {
               // bbox.min_ *= (*xform);
            }
            bbox.max_ = bbox.min_;
            points += stride;
            count--;

            if (count > 0)
            {
                if (is_rat)
                {
                    // homogeneous rational points
                    if (xform)
                    {
                        //for ( /*empty*/; count--; points += stride)
                        //{
                        //    if (0.0 == (w = points[wi]))
                        //    {
                        //        rc = false;
                        //        continue;
                        //    }
                        //    memcpy(&P.x, points, dim * sizeof(P.x));
                        //    w = 1.0 / w;
                        //    P.x *= w; P.y *= w; P.z *= w;
                        //    P *= (*xform);
                        //    if (bbox.min_.x > P.x) bbox.min_.x = P.x; else if (bbox.max_.x < P.x) bbox.max_.x = P.x;
                        //    if (bbox.min_.y > P.y) bbox.min_.y = P.y; else if (bbox.max_.y < P.y) bbox.max_.y = P.y;
                        //    if (bbox.min_.z > P.z) bbox.min_.z = P.z; else if (bbox.max_.z < P.z) bbox.max_.z = P.z;
                        //}
                        //if (dim < 3)
                        //{
                        //    for (i = dim; i < 3; i++)
                        //    {
                        //        bbox.min_[i] = 0.0;
                        //        bbox.max_[i] = 0.0;
                        //    }
                        //}
                    }
                    else
                    {
                        for ( /*empty*/; count--; points += stride)
                        {
                            if (0.0f == (w = points[wi]))
                            {
                                rc = false;
                                continue;
                            }
                            memcpy(&P.x, points, dim * sizeof(P.x));
                            w = 1.0f / w;
                            P.x *= w; P.y *= w; P.z *= w;
                            if (bbox.min_.x > P.x) bbox.min_.x = P.x; else if (bbox.max_.x < P.x) bbox.max_.x = P.x;
                            if (bbox.min_.y > P.y) bbox.min_.y = P.y; else if (bbox.max_.y < P.y) bbox.max_.y = P.y;
                            if (bbox.min_.z > P.z) bbox.min_.z = P.z; else if (bbox.max_.z < P.z) bbox.max_.z = P.z;
                        }
                    }
                }
                else
                {
                    // bounding box of non-rational points
                    if (xform)
                    {
                        //for ( /*empty*/; count--; points += stride)
                        //{
                        //    memcpy(&P.x, points, dim * sizeof(P.x));
                        //    P *= (*xform);
                        //    if (bbox.min_.x > P.x) bbox.min_.x = P.x; else if (bbox.max_.x < P.x) bbox.max_.x = P.x;
                        //    if (bbox.min_.y > P.y) bbox.min_.y = P.y; else if (bbox.max_.y < P.y) bbox.max_.y = P.y;
                        //    if (bbox.min_.z > P.z) bbox.min_.z = P.z; else if (bbox.max_.z < P.z) bbox.max_.z = P.z;
                        //}
                        //if (dim < 3)
                        //{
                        //    for (i = dim; i < 3; i++)
                        //    {
                        //        bbox.min_[i] = 0.0;
                        //        bbox.max_[i] = 0.0;
                        //    }
                        //}
                    }
                    else
                    {
                        for ( /*empty*/; count--; points += stride)
                        {
                            memcpy(&P.x, points, dim * sizeof(P.x));
                            if (bbox.min_.x > P.x) bbox.min_.x = P.x; else if (bbox.max_.x < P.x) bbox.max_.x = P.x;
                            if (bbox.min_.y > P.y) bbox.min_.y = P.y; else if (bbox.max_.y < P.y) bbox.max_.y = P.y;
                            if (bbox.min_.z > P.z) bbox.min_.z = P.z; else if (bbox.max_.z < P.z) bbox.max_.z = P.z;
                        }
                    }
                }
            }

            tight_bbox.Union(bbox);
        }
        else if (bGrowBox)
        {
            // result is still valid if no points are added to a valid input box
            rc = (0 == count);
        }

        return rc;
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
