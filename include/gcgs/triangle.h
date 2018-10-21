#pragma once
#ifndef CGCS_TRIANGLE_H
#define CGCS_TRIANGLE_H

#include "hyperplane.h"

#include <ostream>

namespace gcgs
{
class triangle;
}
std::ostream & operator << (std::ostream & out, gcgs::triangle const & L);

namespace gcgs
{

/**
 * @brief The triangle class
 *
 * A line segment is a line connecting two points
 * in two dimensions.
 */
class triangle
{
public:
    using plane_type      = hyperplane<3>;
    using point_type      = vec<3>;
    using normal_type     = vec<3>;

    point_type m_point[3];

    point_type & operator[](int i)
    {
        return m_point[i];
    }
    point_type const & operator[](int i) const
    {
        return m_point[i];
    }

    /**
     * @brief get_hyperplane
     * @return
     *
     * Returns the hyperplane represented by the line segment.
     * The normal of the hyperplane is the vector p[1]-p[0] rotated
     * 90 degrees counter-clockwise.
     *
     * This function is required for Tree
     */
    plane_type get_hyperplane() const
    {
        const auto v1 = m_point[1] - m_point[0];
        const auto v2 = m_point[2] - m_point[0];
        const auto n = glm::cross(v1,v2);
        return plane_type( m_point[0], n );
    }

    triangle invert() const
    {
        return triangle{ m_point[0], m_point[2], m_point[1]};
    }
    /**
     * @brief split
     * @param plane
     * @param inside
     * @param outside
     *
     * Splits the line-segment using the plane. It returns a list of triangles which are
     * below the plane and above the plane.
     *
     * This function is required for Tree
     */
    void split(const plane_type & plane, std::vector<triangle> & below, std::vector<triangle> & above) const
    {
        auto d0 = plane.distance( m_point[0]);
        auto d1 = plane.distance( m_point[1]);
        auto d2 = plane.distance( m_point[2]);



        if( d0 <= 0 && d1 <= 0 && d2 <= 0 )
        {
            below.push_back(*this);
        }
        else if( d0 >= 0 && d1 >=0 && d2 >= 0)
        {
            above.push_back(*this);
        }
        else
        {
            // This algorithm of splitting a triangle into 3 by a dividing plane only works
            // if point_0 is on one side of the dividing plane and the other two points are on
            // the other side of the plane.

            // We will use pointers to the points to rearrage the points so that
            // only point 0 is on one side.

            auto * p0 = &m_point[0];
            auto * p1 = &m_point[1];
            auto * p2 = &m_point[2];

            auto * D0 = &d0;

            // create a 3-bit code which indicates which points are on which side of the plane
            // a bit value of 0 indicates it's on the positive side of the plane.
            auto code = std::signbit(d0)*1 + std::signbit(d1)*2 + std::signbit(d2)*4;
            switch(code)
            {
                case 1: // point 0 is on one side and point 1 and 2 are on the other
                case 6:
                    p0 = &m_point[0];
                    p1 = &m_point[1];
                    p2 = &m_point[2];

                    D0 = &d0; //D1 = &d1; D2 = &d2;
                    break;
                case 2: // point 1 is on one side, and point 0 and 2 are on the other
                case 5:
                    p0 = &m_point[1];
                    p1 = &m_point[2];
                    p2 = &m_point[0];
                    D0 = &d1; //D1 = &d2; D2 = &d0;
                    break;
                case 4: // point 2 is on one side, and point 0 and 1 are on the other side.
                case 3:
                    p0 = &m_point[2];
                    p1 = &m_point[0];
                    p2 = &m_point[1];
                    D0 = &d2; //D1 = &d0; D2 = &d1;
                    break;
                default:
                    assert(0); // this should never happen!

            }
            // Assume a is on one side
            //     c     b
            //     +----+
            //     |\  /
            //   C |_\/ B        Plane cuts triangle and produces two new points B and C
            //     | /
            //     |/
            //     a
            auto & a = *p0;
            auto & b = *p1;
            auto & c = *p2;

            const auto v1 = a-b;
            const auto t1 = glm::dot(  plane.m_normal, plane.m_point-a) / glm::dot( plane.m_normal, v1);
            auto B = a + t1*v1;

            const auto v2 = a-c;
            const auto t2 = glm::dot(  plane.m_normal, plane.m_point-c) / glm::dot( plane.m_normal, v2);
            auto C = a + t2*v2;
            assert( !std::isnan(t1) );
            assert( !std::isnan(t2) );

            if( *D0 < 0)
            {
                below.push_back( triangle{a,B,C});
                above.push_back( triangle{c,C,B});
                above.push_back( triangle{c,B,b});
            }
            else
            {
                above.push_back( triangle{a,B,C} );
                below.push_back( triangle{c,C,B} );
                below.push_back( triangle{c,B,b} );
            }

        }
    }


};



}

inline std::ostream & operator << (std::ostream & out, gcgs::triangle const & L)
{
    out << "(" << L[0].x << "," << L[0].y << ",";
    out << L[1].x << "," << L[1].y << ")";
    return out;
}



#endif
