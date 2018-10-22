#pragma once
#ifndef GCSG_GEOMETRY_TESTS
#define GCSG_GEOMETRY_TESTS

#include <catch2/catch.hpp>
#include<glm/glm.hpp>

#include <gcgs/triangle.h>

bool equal_points(glm::vec3 const &a, glm::vec3 const & b)
{
    return (Catch::Detail::Approx(a.x) == b.x) && (Catch::Detail::Approx(a.y) == b.y) && (Catch::Detail::Approx(a.z) == b.z);
}

// checks if two triangles are equal:
// abc == bca == bca
bool equal_triangles(gcsg::triangle const & abc, gcsg::triangle const & ABC)
{
    return
    (equal_points( abc[0], ABC[0]) && equal_points( abc[1], ABC[1]) && equal_points( abc[2], ABC[2])) ||
    (equal_points( abc[0], ABC[1]) && equal_points( abc[1], ABC[2]) && equal_points( abc[2], ABC[0])) ||
    (equal_points( abc[0], ABC[2]) && equal_points( abc[1], ABC[0]) && equal_points( abc[2], ABC[1]));
    //return (Catch::Detail::Approx(a.x) == b.x) && (Catch::Detail::Approx(a.y) == b.y) && (Catch::Detail::Approx(a.z) == b.z);
}

// returns true if b is in the triangle abc
bool contains_point(gcsg::triangle const & abc, glm::vec3 const & b)
{
    return equal_points(abc[0], b) || equal_points(abc[1], b) || equal_points(abc[2], b);
}

// returns the index the point is located at
int point_index(gcsg::triangle const & abc, glm::vec3 const & b)
{
    for(int i=0;i<3;i++)
    {
        if( equal_points(abc[i],b) )
            return i;
    }
    return -1;
}

// given a triangle abc and two points from that triangle, return the third point
glm::vec3 missing_point(gcsg::triangle const & abc, glm::vec3 const & a, glm::vec3 const & b)
{
    auto i = point_index(abc,a)+1;
    auto j = point_index(abc,b)+1;

    return abc[6 - (i+j) -1];

}

// two triangles are coplanar if their normals are the same
bool are_coplanar(gcsg::triangle const & T1, gcsg::triangle const & T2)
{
    return glm::length(T1.get_hyperplane().normal() - T2.get_hyperplane().normal()) == Catch::Detail::Approx(0.0f);
}

// two triangles are antiplanar if their normals are negatives of each other
bool are_antiplanar(gcsg::triangle const & T1, gcsg::triangle const & T2)
{
    return glm::length(T1.get_hyperplane().normal() + T2.get_hyperplane().normal()) == Catch::Detail::Approx(0.0f);
}

#endif
