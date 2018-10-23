#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/line_segment.h>


using line_segment = gcsg::line_segment<test_value_type>;
using vec          = gcsg::vec2<test_value_type>;

bool equal_points(vec const &a, vec const & b)
{
    return (Catch::Detail::Approx(a.x) == b.x) && (Catch::Detail::Approx(a.y) == b.y);
}

// checks if two triangles are equal:
// abc == bca == bca
template<typename T>
bool equal_lines(gcsg::line_segment<T> const & ab, gcsg::line_segment<T> const & AB)
{
    return equal_points( ab[0], AB[0] ) && equal_points( ab[1], AB[1] );
}

// returns true if b is in the triangle abc
template<typename T>
bool contains_point(gcsg::line_segment<T> const & ab, gcsg::vec2<T> const & b)
{
    return equal_points(ab[0], b) || equal_points(ab[1], b);
}

// returns the index the point is located at
template<typename T>
int point_index(gcsg::line_segment<T> const & ab, gcsg::vec2<T> const & b)
{
    for(int i=0;i<2;i++)
    {
        if( equal_points(ab[i],b) )
            return i;
    }
    return -1;
}

// given a triangle abc and two points from that triangle, return the third point
template<typename T>
gcsg::vec2<T> missing_point(gcsg::line_segment<T> const & ab, gcsg::vec2<T> const & a )
{
    if( equal_points(ab[0],a) ) return ab[1];
    if( equal_points(ab[1],a) ) return ab[0];
    throw std::runtime_error("Point is not of the end points");
}

// two triangles are coplanar if their normals are the same
template<typename T>
bool are_parallel(gcsg::line_segment<T> const & T1, gcsg::line_segment<T> const & T2)
{
    return glm::length(
                glm::normalize( T1[1]-T1[0]) - glm::normalize( T2[1]-T2[0])
                ) == Catch::Detail::Approx(0.0f);
}

// two triangles are antiplanar if their normals are negatives of each other
template<typename T>
bool are_antiparallel(gcsg::line_segment<T> const & T1, gcsg::line_segment<T> const & T2)
{
    return glm::length(
                glm::normalize( T1[1]-T1[0]) + glm::normalize( T2[1]-T2[0])
                ) == Catch::Detail::Approx(0.0f);
}



SCENARIO( "Local tests" )
{
    GIVEN( "A line segment ab and ba" )
    {
        vec a(-1, 0 );
        vec b(1 , 0 );



        line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        line_segment ba{ b, a };

        line_segment AB{ test_value_type(2)*a, test_value_type(2)*b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse

        REQUIRE( are_antiparallel(ab,ba) );
        REQUIRE( are_parallel(ab,AB) );

        REQUIRE( contains_point(ab,a));
        REQUIRE( contains_point(ab,b));

        REQUIRE( equal_points( missing_point(ab,b), a) );
        REQUIRE( equal_points( missing_point(ab,a), b) );

    }
}

SCENARIO( "Line-Segment Hyperplanes" )
{

    GIVEN( "A line segment ab and ba" )
    {
        vec a(-1, 0 );
        vec b(1 , 0 );

        vec outside(1, 4);  // point inside the line
        vec inside(-15, -1); // point outside the line

        line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        line_segment ba{ b, a };

        WHEN("The hyperplans are extracted")
        {
            auto h1 = ab.get_hyperplane();
            auto h2 = ba.get_hyperplane();

            auto h3 = ab.invert().get_hyperplane().m_normal;

            THEN("The Inverted line segment has a negative normal")
            {
                REQUIRE( equal_points(h3, -h1.m_normal) );
            }
            THEN("The Normals are negative of each other")
            {
                REQUIRE( equal_points( h1.normal() , vec(0, 1)) );
                REQUIRE( equal_points( h2.normal() , vec(0,-1)) );
            }
            THEN("Distance between the points and the hyerplans are zero")
            {
                REQUIRE( h1.distance(a) == Approx( 0.0 ));
                REQUIRE( h1.distance(b) == Approx( 0.0 ));

                REQUIRE( h2.distance(a) == Approx( 0.0 ));
                REQUIRE( h2.distance(b) == Approx( 0.0 ));
            }

            THEN("Distance between the plane and the outside points are always positive")
            {
                REQUIRE( h1.distance(outside) == Approx( 4 ));
                REQUIRE( h2.distance(outside) == Approx( -4 ));
            }

            THEN("Distance between the plane and the inside points are always positive")
            {
                REQUIRE( h1.distance(inside) == Approx( -1 ));

                REQUIRE( h2.distance(inside) == Approx(  1 ));
            }
        }
    }
}


SCENARIO( "Line-Segment Hyperplane Intersections" ) {

    GIVEN( "A line segment ab and cd" )
    {

        //   <--
        //               d          ^
        //               |          |
        // a ------------x-------------- b
        //               |
        //               c


        vec a(-1, 0 );
        vec b(1 , 0 );
        vec c(0 ,-1 );
        vec d(0 , 1 );

        vec origin(0.f);

        line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        line_segment cd{ c, d };


        WHEN("The hyperplanes are extracted")
        {
            auto h1 = ab.get_hyperplane();
            auto h2 = cd.get_hyperplane();

            THEN("The intersection point of ab and bd is the origin")
            {
                auto intersection_point = ab.intersection_point(h2);
                REQUIRE( equal_points( intersection_point, origin ) );
            }
            THEN("The intersection point of cd and ab is the origin")
            {
                auto intersection_point = cd.intersection_point(h1);
                REQUIRE( equal_points( intersection_point, origin ) );
            }
        }
    }
}

SCENARIO( "Splitting a Line-Segment" ) {

    GIVEN( "A line segment ab and cd" )
    {

        //   <--
        //               d          ^
        //               |          |
        // a ------------x-------------- b
        //               |
        //               c


        vec a(-1, 0 );
        vec b(1 , 0 );
        vec c(0 ,-1 );
        vec d(0 , 1 );

        line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        line_segment cd{ c, d };

        auto Pab = ab.get_hyperplane();
        auto Pcd = cd.get_hyperplane();

        THEN("c is inside the plane spanned by ab and d is outside")
        {
            REQUIRE( Pab.distance(c) < 0);
            REQUIRE( Pab.distance(d) > 0);
        }
        THEN("b is inside the plane spanned by cd and a is outside")
        {
            REQUIRE( Pcd.distance(b) < 0);
            REQUIRE( Pcd.distance(a) > 0);
        }
        WHEN("ab is split by cd")
        {
            std::vector<line_segment> inside;
            std::vector<line_segment> outside;

            ab.split(Pcd, inside, outside);

            THEN("There is one line segment inside the plane and one outside")
            {
                REQUIRE( inside.size() == 1);
                REQUIRE( outside.size() == 1);

                REQUIRE( Pcd.distance(inside[0][0]) <= 0.f );
                REQUIRE( Pcd.distance(inside[0][1]) <= 0.f );

                REQUIRE( Pcd.distance(outside[0][0]) >= 0.f );
                REQUIRE( Pcd.distance(outside[0][1]) >= 0.f );
            }

            THEN("Both line segments are parallel")
            {
                REQUIRE( are_parallel(inside[0], outside[0]) );

                REQUIRE( are_parallel(inside[0], ab) );
                REQUIRE( are_parallel(outside[0], ab) );
            }

            THEN("point a is outside the plane, point b is inside the plane")
            {
                REQUIRE( contains_point(outside[0],a));
                REQUIRE( contains_point(inside[0] ,b));
            }
            THEN("A new point X is created on both line segments and is on the plane Pcd")
            {
                auto x = missing_point(outside[0] ,a);
                auto X = missing_point(inside[0],b);

                REQUIRE( equal_points(x,X));

                REQUIRE( Pcd.distance(x) == Approx(0.0f));
                REQUIRE( Pcd.distance(X) == Approx(0.0f));

            }
        }
    }
}

