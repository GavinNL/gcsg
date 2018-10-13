#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include<vector>
#include <cstdint>
//#include "gcgs/triangle.h"
#include <gcgs/triangle.h>
#include<gcgs/hyperplane.h>
#include<gcgs/line_segment.h>
#include<gcgs/tree.h>

using namespace std;

auto box_mesh(float dx , float dy , float dz )
{
    std::vector< glm::vec3 > P;
    std::vector< uint32_t> I;

    std::vector<triangle> T;

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
//       |       Position               |   UV         |     Normal    |
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,0.0 - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,dz -0.5*dz)) ;
        P.push_back( glm::vec3(dx  - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;
        P.push_back( glm::vec3(0.0 - 0.5*dx  ,dy  - 0.5*dy  ,0.0-0.5*dz)) ;

    //=========================
    // Edges of the triangle : postion delta

     for(int i=0 ; i < P.size(); i+=3)
     {
         T.push_back( triangle(P[i], P[i+1], P[i+2] ));
     }
    return T;


}


std::vector<gcgs::line_segment> get_box(float s, glm::vec2 offset = glm::vec2(0,0))
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


int main()
{
    using namespace gcgs;


    Tree<2, line_segment> test;

    auto B1 = get_box(1,glm::vec2(0));
    auto B2 = get_box(1,glm::vec2(1,-1));

    for(auto & L : B1)
    {
        test.add(L);
    }
    for(auto & L : B2)
    {
        test.add(L);
    }

   // std::cout << "Adding: " << B2[0] << std::endl;
   //     test.add( B2[0]);

    std::cout << "printing" << std::endl;
    test.print();
    return 0;



    return 0;
}
