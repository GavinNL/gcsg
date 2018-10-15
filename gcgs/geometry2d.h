#pragma once
#ifndef GCGS_GEOMETRY_2D
#define GCGS_GEOMETRY_2D

#include "line_segment.h"
#include<vector>

namespace gcgs
{

struct geo2d
{

    static std::vector<gcgs::line_segment> get_circle(float R, glm::vec2 offset = glm::vec2(0,0), uint32_t N=10)
    {
        std::vector<gcgs::line_segment> b;
        float t = 0.0;
        float dt = 3.141592653589 * 2.0 / N;
        for(uint32_t i=0;i<N;i++)
        {
            auto c  = R * cos(t);
            auto s =  R * sin(t);

            auto c2  =  R * cos(t + dt);
            auto s2  =  R * sin(t + dt);
            b.push_back( { glm::vec2{c2, s2}, glm::vec2{c, s}} );
            t += dt;
        }
        return b;
    }


    static std::vector<gcgs::line_segment> get_box(float s, glm::vec2 offset = glm::vec2(0,0))
    {
        std::vector<gcgs::line_segment> b;
        b.push_back( { glm::vec2{-1, 1}, glm::vec2{1, 1}} );
        b.push_back( { glm::vec2{ 1, 1}, glm::vec2{1,-1}} );
        b.push_back( { glm::vec2{ 1,-1}, glm::vec2{-1,-1}} );
        b.push_back( { glm::vec2{-1,-1}, glm::vec2{-1, 1}} );
        for(auto & l : b)
        {
            l[0] *= s;
            l[0] += offset;
            l[1] *= s;
            l[1] += offset;
        }
        return b;
    }
};

}

#endif
