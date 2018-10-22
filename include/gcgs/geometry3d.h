#pragma once
#ifndef GCGS_GEOMETRY_3D
#define GCGS_GEOMETRY_3D

#include "triangle.h"
#include<vector>

namespace gcsg
{

struct geo3d
{

    static std::vector<gcsg::triangle> box_mesh( gcsg::vec3<float> const & dim, gcsg::vec3<float> const & offset)
    {
        std::vector< glm::vec3 > P;
        std::vector< uint32_t> I;

        std::vector<gcsg::triangle> T;

        auto & dx = dim[0];
        auto & dy = dim[1];
        auto & dz = dim[2];

    //       |       Position               |   UV         |     Normal    |
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,0.0f - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,dz -0.5f*dz)) ;
            P.push_back( offset + glm::vec3(dx  - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;
            P.push_back( offset + glm::vec3(0.0f - 0.5f*dx  ,dy  - 0.5f*dy  ,0.0f-0.5f*dz)) ;

        //=========================
        // Edges of the triangle : postion delta

         for(uint32_t i=0 ; i < P.size(); i+=3)
         {
             T.push_back( gcsg::triangle{ P[i], P[i+1], P[i+2] } );
         }
        return T;


    }
};

}

#endif
