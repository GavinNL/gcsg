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

    std::vector<gcgs::triangle> T;

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
         T.push_back( gcgs::triangle{ P[i], P[i+1], P[i+2] } );
     }
    return T;


}



std::vector<gcgs::line_segment> get_circle(float R, glm::vec2 offset = glm::vec2(0,0), uint32_t N=10)
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


    Tree<2, line_segment> S1;
    Tree<2, line_segment> S2;


    Tree<2, line_segment> inside;
    Tree<2, line_segment> outside;

    auto B1 = get_circle(1.0, glm::vec2(0.0), 20);//get_box(1,glm::vec2(0));
    //auto B1 = get_box(1,glm::vec2(0));
    auto B2 = get_box(1,glm::vec2(1,-1));

    for(auto & L : B1)
    {
        S1.add(L);
    }
    for(auto & L : B2)
    {
        S2.add(L);
    }

    // would need to traverse S2 to get all the triangles instead
    // of looping through B2.

    S2.for_each(
    [&](line_segment & L)
    {
        S1.partition(L, inside, outside);
    });
    S1.for_each(
    [&](line_segment & L)
    {
        S2.partition(L, inside, outside);
    });

    std::cout << "printing" << std::endl;
    outside.print();
    return 0;



    return 0;
}
