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

void print_line_segments( gcgs::Tree<2, gcgs::line_segment> & T)
{
    using namespace gcgs;

    T.for_each(
    [&](line_segment & L)
    {
        printf("%f %f %f %f\n", L[0].x,L[0].y,L[1].x,L[1].y);
    });
}


int main()
{
    using namespace gcgs;

    Tree<2, line_segment> S1;
    Tree<2, line_segment> S2;

    auto B1 = geo2d::get_circle(1.0, glm::vec2(0.0), 20);//get_box(1,glm::vec2(0));
    auto B2 = geo2d::get_box(1,glm::vec2(1,-1));

    for(auto & L : B1)
    {
        S1.add(L);
    }
    for(auto & L : B2)
    {
        S2.add(L);
    }

    auto S3 = S1.Union(S2);


    print_line_segments(S3);

    return 0;
}
