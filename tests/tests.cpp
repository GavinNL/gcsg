#include <gnl/gnl_interp.h>
#include <glm/glm.hpp>
#include <vector>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <string>

#define GCGS_INCLUDE_TESTS
#include <gcgs/line_segment.h>
#include <gcgs/triangle.h>


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

                REQUIRE( Pcd.distance(inside[1][0]) <= 0.f );
                REQUIRE( Pcd.distance(inside[1][1]) <= 0.f );
            }

            THEN("Both line segments have the name normal")
            {
                auto h1 = inside[0].get_hyperplane();
                auto h2 = inside[0].get_hyperplane();

                REQUIRE( glm::length(h1.normal()-h2.normal()) == Approx(0.0f));
                REQUIRE( glm::length(h1.normal()-Pab.normal()) == Approx(0.0f));
            }

            THEN("There is an intersection point on the plane cd")
            {
                REQUIRE( fabs(Pcd.distance(inside[0][0]) ) == Approx(0.0f) );
                REQUIRE( fabs(Pcd.distance(inside[0][1]) ) != Approx(0.0f) );

                REQUIRE( fabs(Pcd.distance(outside[1][0]) ) == Approx(0.0f) );
                REQUIRE( fabs(Pcd.distance(outside[1][1]) ) != Approx(0.0f) );
            }
        }
    }
}


SCENARIO( "Triangle Hyperplanes" ) {

    GIVEN( "A line segment ab and ba" )
    {
        glm::vec3 a( 1 ,  0 , 0);
        glm::vec3 b(-1 ,  1 , 0);
        glm::vec3 c(-1 , -1 , 0);

        glm::vec3 outside(1,   4, 1);  // point inside the line
        glm::vec3 inside(-15, -1, -1); // point outside the line

        gcgs::triangle abc{a,b,c};

        WHEN("The Triangle is inverted")
        {
            auto Ti = abc.invert();
            auto Pi = Ti.get_hyperplane();
            auto P  = abc.get_hyperplane();

            REQUIRE( Pi.m_normal[0] == Approx(-P.m_normal[0]) );
            REQUIRE( Pi.m_normal[1] == Approx(-P.m_normal[1]) );
            REQUIRE( Pi.m_normal[2] == Approx(-P.m_normal[2]) );
        }
        WHEN("The hyperplans are extracted")
        {
            auto h1 = abc.get_hyperplane();

            THEN("The Normals are negative of each other")
            {
                REQUIRE( glm::length( h1.normal() - glm::vec3(0, 0, 1)) == Approx( 0.0 ));
            }
            THEN("Distance between the points and the hyerplans are zero")
            {
                REQUIRE( h1.distance(a) == Approx( 0.0 ));
                REQUIRE( h1.distance(b) == Approx( 0.0 ));
                REQUIRE( h1.distance(c) == Approx( 0.0 ));
            }

            THEN("Distance between the plane and the outside points are always positive")
            {
                REQUIRE( h1.distance(outside) == Approx(  1 ));
                REQUIRE( h1.distance(inside) == Approx( -1 ));
            }
        }
    }
}




SCENARIO( "Splitting a Triangle with Hyperplanes 2" ) {

    GIVEN( "a hyperplane in the yz plane and a triangle abc" )
    {
        gcgs::hyperplane<3> splitting_plane( glm::vec3(0), glm::vec3(1,0,0));

        glm::vec3 a( 1 ,  0 , 0);
        glm::vec3 b(-1 ,  1 , 0);
        glm::vec3 c(-1 , -1 , 0);

        WHEN("A plane in the yz plane splits the triangle")
        {
            gcgs::triangle abc{a,b,c};

            std::vector<gcgs::triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 3 triangles")
            {
                REQUIRE( below.size() == 2);
                REQUIRE( above.size() == 1);
            }
        }
        WHEN("A plane in the yz plane splits the triangle")
        {
            gcgs::triangle abc{b,c,a};

            std::vector<gcgs::triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 3 triangles")
            {
                REQUIRE( below.size() == 2);
                REQUIRE( above.size() == 1);
            }
        }
        WHEN("A plane in the yz plane splits the triangle")
        {
            gcgs::triangle abc{c,a,b};

            std::vector<gcgs::triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 3 triangles")
            {
                REQUIRE( below.size() == 2);
                REQUIRE( above.size() == 1);
            }
        }
    }
}
