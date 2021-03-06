#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include <vector>
#include <cstdint>

#include<gcgs/hyperplane.h>
#include<gcgs/line_segment.h>
#include<gcgs/tree.h>

#include <gcgs/geometry2d.h>

using namespace std;

template<typename T>
void print_line_segments( gcsg::Tree<2, gcsg::line_segment<T> > const & tree)
{
    using namespace gcsg;

    tree.for_each(
    [&](gcsg::line_segment<T> & L)
    {
        std::cout << L[0].x << ' ' << L[0].y << ' ' << L[1].x << ' ' << L[1].y << '\n';
    });
}


int main()
{
    using line_segment = gcsg::line_segment<float>;
    using vec2         = gcsg::vec2f;

    gcsg::Tree<2, line_segment> S1;
    gcsg::Tree<2, line_segment> S2;
    gcsg::Tree<2, line_segment> S3;

    auto B1 = gcsg::geo2d::get_circle(1.0f, vec2(0.0), 20);//get_box(1,glm::vec2(0));
    auto B2 = gcsg::geo2d::get_box(1.f ,glm::vec2(1,-1));
    auto B3 = gcsg::geo2d::get_circle(1.0f, vec2(2.0,-2), 20);//get_box(1,glm::vec2(0));

    //auto B1 = geo2d::get_box(1,glm::vec2(0,0));
    //auto B2 = geo2d::get_box(1,glm::vec2(2,0));
    //auto B3 = geo2d::get_box(1,glm::vec2(4,0));

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

    auto S4 = S1.Union(S2).Union(S3);

    print_line_segments(  S4 );

    return 0;
}

