#pragma once
#ifndef CGCS_LINE_SEGMENT_H
#define CGCS_LINE_SEGMENT_H

#include "hyperplane.h"

#include <ostream>

namespace gcgs
{
class line_segment;
}
std::ostream & operator << (std::ostream & out, gcgs::line_segment const & L);

namespace gcgs
{

/**
 * @brief The line_segment class
 *
 * A line segment is a line connecting two points
 * in two dimensions.
 */
class line_segment
{
public:
    using plane_type      = hyperplane<2>;
    using point_type      = vec<2>;
    using normal_type     = vec<2>;

    point_type m_point[2];


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
        const auto d = m_point[1] - m_point[0];
        const auto n = normal_type{ -d.y, d.x};
        return plane_type( m_point[1] , n );
    }

    /**
     * @brief split
     * @param plane
     * @param inside
     * @param outside
     *
     * Splits the line-segment using the plane. It returns a list of line_segments which are
     * below the plane and above the plane.
     *
     * This function is required for Tree
     */
    void split(const plane_type & plane, std::vector<line_segment> & below, std::vector<line_segment> & above) const
    {
        auto d0 = plane.distance( m_point[0]);
        auto d1 = plane.distance( m_point[1]);

        if( d0 <= 0 && d1 <=0)
        {
            below.push_back(*this);
        }
        else if( d0 >= 0 && d1 >= 0)
        {
            above.push_back(*this);
        }
        else
        {
            auto v = m_point[1] - m_point[0];
            //auto t = -glm::dot( plane.m_normal, m_point[0] ) / glm::dot( v, plane.m_normal);

            auto t = plane.distance(m_point[0]) / glm::length( v );

            assert( !std::isnan(t) );
            auto p_i = intersection_point(plane);// m_point[0] + t*v; // intersection point

            if( d0 < 0)
            {
                below.push_back(  line_segment{m_point[0], p_i});
                above.push_back( line_segment{p_i, m_point[1]});
            }
            else
            {
                above.push_back( line_segment{m_point[0], p_i});
                below.push_back(  line_segment{p_i, m_point[1]});
            }

        }
    }


    /**
     * @brief intersection_point
     * @param plane
     * @return
     *
     * Returns the intersection point of this linesegment with the plane.
     *
     */
    point_type intersection_point( plane_type const & plane) const
    {
        const auto v = m_point[1]-m_point[0];
        //const auto d = plane.distance( m_point[0] );
        const auto t = glm::dot(  plane.m_normal, plane.m_point-m_point[0]) / glm::dot( plane.m_normal, v);
        return m_point[0] + t*v;
    }



    void print() const
    {
        std::cout << *this << std::endl;
    }
};



}

inline std::ostream & operator << (std::ostream & out, gcgs::line_segment const & L)
{
    out << "(" << L[0].x << "," << L[0].y << ",";
    out << L[1].x << "," << L[1].y << ")";
    return out;
}



#endif
