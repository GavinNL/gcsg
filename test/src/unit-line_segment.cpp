#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/line_segment.h>


SCENARIO( "Line-Segment Hyperplanes" ) {

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
                REQUIRE( h3[0] == Approx(-h1.m_normal[0]) );
                REQUIRE( h3[1] == Approx(-h1.m_normal[1]) );
            }
            THEN("The Normals are negative of each other")
            {
                REQUIRE( glm::length( h1.normal() - glm::vec2(0, 1)) == Approx( 0.0 ));
                REQUIRE( glm::length( h2.normal() - glm::vec2(0,-1)) == Approx( 0.0 ));
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

        gcgs::line_segment ab{ a, b }; // the normal will always be the vector from point a to point b, rotated 90 counter-clockwisse
        gcgs::line_segment cd{ c, d };

        WHEN("The hyperplanes are extracted")
        {
            auto h1 = ab.get_hyperplane();
            auto h2 = cd.get_hyperplane();

            THEN("The intersection point of ab and bd is the origin")
            {
                auto intersection_point = ab.intersection_point(h2);
                REQUIRE( glm::length( intersection_point) == Approx( 0.0 ));
            }
            THEN("The intersection point of cd and ab is the origin")
            {
                auto intersection_point = cd.intersection_point(h1);
                REQUIRE( glm::length( intersection_point) == Approx( 0.0 ));
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

            THEN("Both line segments have the name normal")
            {
                auto h1 = inside[0].get_hyperplane();
                auto h2 = outside[0].get_hyperplane();

                REQUIRE( glm::length(h1.normal()-h2.normal()) == Approx(0.0f));
                REQUIRE( glm::length(h1.normal()-Pab.normal()) == Approx(0.0f));
            }

            THEN("There is an intersection point on the plane cd")
            {
                REQUIRE( fabs(Pcd.distance(inside[0][0]) ) == Approx(0.0f) );
                REQUIRE( fabs(Pcd.distance(inside[0][1]) ) != Approx(0.0f) );

                REQUIRE( fabs(Pcd.distance(outside[0][1]) ) == Approx(0.0f) );
                REQUIRE( fabs(Pcd.distance(outside[0][0]) ) != Approx(0.0f) );
            }
        }
    }
}
