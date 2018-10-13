#pragma once
#ifndef GCGS_TRIANGLE
#define GCGS_TRIANGLE

#include <glm/glm.hpp>

class triangle
{
     public:
        triangle()
        {

        }

        triangle(const glm::vec3 & a,const glm::vec3 & b, const glm::vec3 & c)
        {
            m_points[0]=a;
            m_points[1]=b;
            m_points[2]=c;
        }

        inline glm::vec3 & operator[](int i)
        {
            return m_points[i];
        }
        inline glm::vec3 const & operator[](int i) const
        {
            return m_points[i];
        }
        glm::vec3 normal() const
        {
            return glm::cross(  m_points[1]-m_points[0], m_points[2]-m_points[0] );
        }

        auto begin()
        {
            return m_points.begin();
        }

        auto end()
        {
            return m_points.end();
        }

        auto begin() const
        {
            return m_points.begin();
        }

        auto end() const
        {
            return m_points.end();
        }

        std::array<glm::vec3,3> m_points;
};


#endif
