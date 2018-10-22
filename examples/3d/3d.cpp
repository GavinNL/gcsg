#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include <vector>
#include <cstdint>

#include<gcgs/hyperplane.h>
#include<gcgs/line_segment.h>
#include<gcgs/tree.h>

#include <gcgs/geometry3d.h>

using namespace std;

void print_mesh( gcsg::Tree<3, gcsg::triangle> const & T)
{
    using namespace gcsg;

    printf("solid name\n");
    T.for_each(
    [&](triangle & L)
    {
        auto n = L.get_hyperplane().normal();
        auto & v1 = L[0];
        auto & v2 = L[1];
        auto & v3 = L[2];

        printf(" facet normal %f %f %f     \n", static_cast<double>(n.x), static_cast<double>(n.y), static_cast<double>(n.z));
        printf("     outer loop            \n");
        printf("         vertex %f %f %f   \n", static_cast<double>(v1.x), static_cast<double>(v1.y), static_cast<double>(v1.z) );
        printf("         vertex %f %f %f   \n", static_cast<double>(v2.x), static_cast<double>(v2.y), static_cast<double>(v2.z) );
        printf("         vertex %f %f %f   \n", static_cast<double>(v3.x), static_cast<double>(v3.y), static_cast<double>(v3.z) );
        printf("     endloop               \n");
        printf(" endfacet                  \n");

       // std::cout << L[0].x << ' ' << L[0].y << ' ' << L[1].x << ' ' << L[1].y << '\n';
    });
    printf("endsolid name\n");
}


int main()
{
    using namespace gcsg;

    Tree<3, triangle> S1;
    Tree<3, triangle> S2;
    Tree<3, triangle> S3;
    Tree<3, triangle> S4;
    Tree<3, triangle> S5;
  //  Tree<3, triangle> S6;

    auto B1 = geo3d::box_mesh( vec3(2,2,2), vec3(0));

    auto B5 = geo3d::box_mesh( vec3(2,2,2), vec3(1));

    auto B2 = geo3d::box_mesh( vec3(1,4,1), vec3(0));
    auto B3 = geo3d::box_mesh( vec3(4,1,1), vec3(0));
    auto B4 = geo3d::box_mesh( vec3(1,1,4), vec3(0));



    for(auto & L : B1)
    {
        S1.add(L);
    }
    for(auto & L : B2)
    {
        S2.add(L);
    }
    for(auto & L : B3)
    {
        S3.add(L);
    }
    for(auto & L : B4)
    {
        S4.add(L);
    }
    for(auto & L : B5)
    {
        S5.add(L);
    }
    auto S6 = S1.Difference(S2).Difference(S3).Difference(S4).Union(S5);

    print_mesh(  S6 );

    return 0;
}

