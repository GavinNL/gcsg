#pragma once
#ifndef GCGS_HYPERPLANE
#define GCGS_HYPERPLANE

#include "vec.h"

namespace gcgs
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
    using point_type  = vec<dim>;
    using normal_type = vec<dim>;

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

    normal_type m_normal;
    point_type  m_point;
};

}

#endif
