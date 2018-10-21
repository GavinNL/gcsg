#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/triangle.h>

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
