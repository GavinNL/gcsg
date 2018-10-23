#pragma once
#ifndef GCGS_GEOMETRY_2D
#define GCGS_GEOMETRY_2D

#include "line_segment.h"
#include<vector>

namespace gcsg
{

struct geo2d
{

    template<typename T=float>
    static std::vector<gcsg::line_segment<T> > get_circle(T R, gcsg::vec2<T> offset = gcsg::vec2<T>(0,0), uint32_t N=10)
    {
        std::vector<gcsg::line_segment<T> > b;
        float t = 0.0;
        float dt = 3.141592653589f * 2.0f / static_cast<float>(N);
        for(uint32_t i=0;i<N;i++)
        {
            auto c  = R * std::cos(t);
            auto s =  R * std::sin(t);

            auto c2  =  R * std::cos(t + dt);
            auto s2  =  R * std::sin(t + dt);
            b.push_back( { gcsg::vec2<T>{c2, s2}+offset, gcsg::vec2<T>{c, s}+offset} );
            t += dt;
        }
        return b;
    }


    template<typename T=float>
    static std::vector<gcsg::line_segment<T>> get_box(float s, gcsg::vec2<T> offset = gcsg::vec2<T>(0,0))
    {
        std::vector<gcsg::line_segment<T> > b;
        b.push_back( { gcsg::vec2<T>{-1, 1}, gcsg::vec2<T>{1, 1}} );
        b.push_back( { gcsg::vec2<T>{ 1, 1}, gcsg::vec2<T>{1,-1}} );
        b.push_back( { gcsg::vec2<T>{ 1,-1}, gcsg::vec2<T>{-1,-1}} );
        b.push_back( { gcsg::vec2<T>{-1,-1}, gcsg::vec2<T>{-1, 1}} );
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
