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
            auto * p0 = &m_point[0];
            auto * p1 = &m_point[1];
            auto * p2 = &m_point[2];

            while( !(std::signbit(d1) == std::signbit(d2) && std::signbit(d0)!=std::signbit(d1)) )
            {
                auto * tmp = p0;
                p0 = p1;
                p1 = p2;
                p2 = tmp;

                auto tmpd = d0;
                d0 = d1;
                d1 = d2;
                d2 = tmpd;
            }

            // Assume a is on one side
            //     c     b
            //     +-----+
            //     |    /
            //   C |___/ B        Plane cuts triangle and produces two new points B and C
            //     |  /
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

            if( d0 < 0)
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
