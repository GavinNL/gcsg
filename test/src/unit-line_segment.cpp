#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/line_segment.h>


bool equal_points(glm::vec2 const &a, glm::vec2 const & b)
{
    return (Catch::Detail::Approx(a.x) == b.x) && (Catch::Detail::Approx(a.y) == b.y);
}

// checks if two triangles are equal:
// abc == bca == bca
bool equal_lines(gcgs::line_segment const & ab, gcgs::line_segment const & AB)
{
    return equal_points( ab[0], AB[0] ) && equal_points( ab[1], AB[1] );
}

// returns true if b is in the triangle abc
bool contains_point(gcgs::line_segment const & ab, glm::vec2 const & b)
{
    return equal_points(ab[0], b) || equal_points(ab[1], b);
}

// returns the index the point is located at
int point_index(gcgs::line_segment const & ab, glm::vec2 const & b)
{
    for(int i=0;i<2;i++)
    {
        if( equal_points(ab[i],b) )
            return i;
    }
    return -1;
}

// given a triangle abc and two points from that triangle, return the third point
glm::vec2 missing_point(gcgs::line_segment const & ab, glm::vec2 const & a )
{
    if( equal_points(ab[0],a) ) return ab[1];
    if( equal_points(ab[1],a) ) return ab[0];
}

// two triangles are coplanar if their normals are the same
bool are_parallel(gcgs::line_segment const & T1, gcgs::line_segment const & T2)
{
    return glm::length(
                glm::normalize( T1[1]-T1[0]) - glm::normalize( T2[1]-T2[0])
                ) == Catch::Detail::Approx(0.0f);
}

// two triangles are antiplanar if their normals are negatives of each other
bool are_antiparallel(gcgs::line_segment const & T1, gcgs::line_segment const & T2)
{
    return glm::length(
                glm::normalize( T1[1]-T1[0]) + glm::normalize( T2[1]-T2[0])
                ) == Catch::Detail::Approx(0.0f);
}

SCENARIO( "Local tests" )
{
    GIVEN( "A line segment ab and ba" )
    {
        glm::vec2 a(-1, 0 );
        glm::vec2 b(1 , 0 );



        gcgs::line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        gcgs::line_segment ba{ b, a };

        gcgs::line_segment AB{ 2.f*a, 2.f*b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse

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
        glm::vec2 a(-1, 0 );
        glm::vec2 b(1 , 0 );

        glm::vec2 outside(1, 4);  // point inside the line
        glm::vec2 inside(-15, -1); // point outside the line

        gcgs::line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        gcgs::line_segment ba{ b, a };

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
                REQUIRE( equal_points( h1.normal() , glm::vec2(0, 1)) );
                REQUIRE( equal_points( h2.normal() , glm::vec2(0,-1)) );
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


        glm::vec2 a(-1, 0 );
        glm::vec2 b(1 , 0 );
        glm::vec2 c(0 ,-1 );
        glm::vec2 d(0 , 1 );

        glm::vec2 origin(0.f);

        gcgs::line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        gcgs::line_segment cd{ c, d };


        WHEN("The hyperplanes are extracted")
        {
            auto h1 = ab.get_hyperplane();
            auto h2 = cd.get_hyperplane();

            THEN("The intersection point of ab and bd is the origin")
            {
                auto intersection_point = ab.intersection_point(h2);
                REQUIRE( equal_points( intersection_point, glm::vec2(0) ) );
            }
            THEN("The intersection point of cd and ab is the origin")
            {
                auto intersection_point = cd.intersection_point(h1);
                REQUIRE( equal_points( intersection_point, glm::vec2(0) ) );
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


        glm::vec2 a(-1, 0 );
        glm::vec2 b(1 , 0 );
        glm::vec2 c(0 ,-1 );
        glm::vec2 d(0 , 1 );

        gcgs::line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        gcgs::line_segment cd{ c, d };

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
            std::vector<gcgs::line_segment> inside;
            std::vector<gcgs::line_segment> outside;

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
                auto x = missing_point(inside[0] ,a);
                auto X = missing_point(outside[0],b);

                REQUIRE( equal_points(x,X));

                REQUIRE( Pcd.distance(x) == Approx(0.0f));
                REQUIRE( Pcd.distance(X) == Approx(0.0f));

            }
        }
    }
}

