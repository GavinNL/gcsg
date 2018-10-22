#pragma once
#ifndef GCGS_GEOMETRY_3D
#define GCGS_GEOMETRY_3D

#include "triangle.h"
#include<vector>

namespace gcsg
{

struct geo3d
{

    template<typename T>
    static std::vector<gcsg::triangle<T> > box_mesh( gcsg::vec3<T> const & dim, gcsg::vec3<T> const & offset)
    {
        using vec = gcsg::vec3<T>;

        std::vector< vec > P;
        std::vector< uint32_t> I;

        std::vector<gcsg::triangle<T> > tri;

        auto & dx = dim[0];
        auto & dy = dim[1];
        auto & dz = dim[2];

    //       |       Position               |   UV         |     Normal    |
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,T(0.0) - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,dz -T(0.5)*dz)) ;
            P.push_back( offset + vec(dx  - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;
            P.push_back( offset + vec(T(0.0) - T(0.5)*dx  ,dy  - T(0.5)*dy  ,T(0.0)-T(0.5)*dz)) ;

        //=========================
        // Edges of the triangle : postion delta

         for(uint32_t i=0 ; i < P.size(); i+=3)
         {
             tri.push_back( gcsg::triangle<T>{ P[i], P[i+1], P[i+2] } );
         }
        return tri;


    }
};

}

#endif
