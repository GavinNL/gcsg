#pragma once
#ifndef GCGS_HYPERPLANE
#define GCGS_HYPERPLANE

#include "vec.h"

namespace gcsg
{

template<uint32_t dim>
/**
 * @brief The hyperplane class
 *
 * The Hyerplane class represents a plane in N dimensions.
 */
class hyperplane
{
public:
    using float_type  = float;
    using point_type  = typename std::conditional<dim==2, vec2<float_type>, vec3<float_type> >::type;
    using normal_type = typename std::conditional<dim==2, vec2<float_type>, vec3<float_type> >::type;

    struct point_of_intersection
    {
        point_type point;
        float      t;
    };

    /**
     * @brief distance
     * @param d
     * @return
     *
     * Gets the distance between the hyperplane and the point.
     *
     * The distance is positive if the normal of the plane
     * points in the direction of the point and negative
     * if the normal points in the opposite direction
     *
     *                  |           positive
     *                  |-> n
     * negative         |
     */
    float distance(point_type const & d) const
    {
        const auto r = d-m_point;
        return glm::dot(r, m_normal) / glm::length(m_normal);
    }

    hyperplane()
    {

    }

    /**
     * @brief hyperplane
     * @param p
     * @param n
     *
     * Construct a hyperplane using a point in space and the normal
     */
    hyperplane(point_type const & p, normal_type const & n) : m_normal( glm::normalize(n) ), m_point( p )
    {

    }

    normal_type const & normal() { return m_normal; };


    /**
     * @brief intersection_point
     * @param a
     * @param b
     * @return
     *
     * Given a line defined by ab, return the intersection point with the hyperplane
     */
    point_of_intersection intersection_point(point_type const & a, point_type const & b) const
    {
        const auto v = b-a;
        auto t = glm::dot( m_normal, m_point-a) / glm::dot( m_normal, v);
        assert( !std::isnan(t));
        auto B = a + t*v;
        return {B,t};

    }

    normal_type m_normal;
    point_type  m_point;
};

}

template<uint32_t N>
inline std::ostream & operator << (std::ostream & out, gcsg::hyperplane<N> const & L)
{
    auto & n = L.m_normal;
    auto & p = L.m_point;
    if(N==2)
    {
        out << "(" << p[0] << "," << p[1] << "|";
               out << n[0] << "," << n[1] << ")";
    }
    else {
        out << "(" << p[0] << "," << p[1] << "," << p[2] << "|";
               out << n[0] << "," << n[1] << "," << n[2] << ")";
    }
    return out;
}

#endif
