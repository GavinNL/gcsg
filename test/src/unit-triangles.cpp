#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/triangle.h>

#include "geometry_tests.h"

using value_type = float;
using vec = gcsg::vec3<value_type>;
using triangle = gcsg::triangle<value_type>;
using hyperplane = gcsg::hyperplane<value_type,3>;

SCENARIO( "Local Tests" )
{
    vec a( 1 ,  0 , 0);
    vec b(-1 ,  1 , 0);
    vec c(-1 , -1 , 0);

    GIVEN("Three triangles abc, bca, cba")
    {
        triangle abc{a,b,c};
        triangle bca{b,c,a};
        triangle cab{c,a,b};

        THEN("All three triangles are equal")
        {
            REQUIRE( equal_triangles(abc,bca) );
            REQUIRE( equal_triangles(abc,cab) );
            REQUIRE( equal_triangles(bca,cab) );
        }

        THEN("Points a b and c are all within each triangle")
        {
            REQUIRE( contains_point(abc,a));
            REQUIRE( contains_point(abc,b));
            REQUIRE( contains_point(abc,c));

            REQUIRE( contains_point(bca,a));
            REQUIRE( contains_point(bca,b));
            REQUIRE( contains_point(bca,c));

            REQUIRE( contains_point(cab,a));
            REQUIRE( contains_point(cab,b));
            REQUIRE( contains_point(cab,c));
        }


    }
    GIVEN("Three triangles abc, cba ")
    {
        triangle abc{a,b,c};
        triangle cba{c,b,a};

        THEN("triangles are not equal because of opposite winding order")
        {
            REQUIRE( !equal_triangles(abc,cba) );
            REQUIRE( !are_coplanar(abc,cba) );
            REQUIRE( are_antiplanar(abc,cba) );
        }
    }

    GIVEN("a triangle abc")
    {
        triangle abc{a,b,c};

        THEN("The point indices are a=0, b=1,c=2")
        {
            REQUIRE( point_index(abc,a)==0);
            REQUIRE( point_index(abc,b)==1);
            REQUIRE( point_index(abc,c)==2);
        }
        THEN("We can find the missing point given at least two points")
        {
            REQUIRE( equal_points( missing_point(abc,a,b) , c ) );
            REQUIRE( equal_points( missing_point(abc,b,c) , a ) );
            REQUIRE( equal_points( missing_point(abc,c,a) , b ) );
        }
    }

    GIVEN("two triangles whose points are translated")
    {
        triangle T1{a,b,c};

        vec o(1,2,3);
        triangle T2{a+o,b+o,c+o};

        THEN("triangles are coplanar")
        {
            REQUIRE( are_coplanar(T1,T2));
            REQUIRE( !are_antiplanar(T1,T2));
        }
    }

    GIVEN("two triangles whose points are translated")
    {
        triangle T1{a,b,c};
        triangle T2{c,b,a};

        THEN("triangles are antiplanar")
        {
            REQUIRE( !are_coplanar(T1,T2) );
            REQUIRE( are_antiplanar(T1,T2) );
        }
    }
}

SCENARIO( "Triangle Hyperplanes" ) {

    GIVEN( "A line segment ab and ba" )
    {
        vec a( 1 ,  0 , 0);
        vec b(-1 ,  1 , 0);
        vec c(-1 , -1 , 0);

        vec outside(1,   4, 1);  // point inside the line
        vec inside(-15, -1, -1); // point outside the line

        triangle abc{a,b,c};

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
                REQUIRE( glm::length( h1.normal() - vec(0, 0, 1)) == Approx( 0.0 ));
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
        hyperplane splitting_plane( vec(0), vec(1,0,0));

        vec a( 1 ,  0 , 0);
        vec b(-1 ,  1 , 0);
        vec c(-1 , -1 , 0);

        WHEN("A plane in the yz plane splits the triangle")
        {
            triangle abc{a,b,c};

            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 3 triangles")
            {
                REQUIRE( below.size() == 2);
                REQUIRE( above.size() == 1);
            }
        }
        WHEN("A plane in the yz plane splits the triangle")
        {
            triangle abc{b,c,a};

            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 3 triangles")
            {
                REQUIRE( below.size() == 2);
                REQUIRE( above.size() == 1);
            }
        }
        WHEN("A plane in the yz plane splits the triangle")
        {
            triangle abc{c,a,b};

            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 3 triangles")
            {
                REQUIRE( below.size() == 2);
                REQUIRE( above.size() == 1);
            }
        }
    }
}


SCENARIO( "Splitting a Triangle into two sub triangles" ) {

    GIVEN( "a hyperplane in the xy plane and a triangle abc" )
    {
        vec a( 1 ,  0 , 0);
        vec b(-1 ,  1 , 0);
        vec c(-1 , -1 , 0);

        triangle abc{a,b,c};

        WHEN("A plane in the yz plane splits the triangle")
        {
            hyperplane splitting_plane( c, glm::normalize(b-a));

            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 2 triangles. One inside and one outside")
            {
                REQUIRE( below.size() == 1);
                REQUIRE( above.size() == 1);
            }
            THEN("The two new triangles are aBC and cBb")
            {
                auto & aBc = below.front();
                auto & cBb = above.front();

                REQUIRE( contains_point(aBc, a));
                REQUIRE( contains_point(aBc, c));

                REQUIRE( contains_point(cBb, c));
                REQUIRE( contains_point(cBb, b));

                auto B1 = missing_point(aBc,a,c);
                auto B2 = missing_point(cBb,c,b);

                REQUIRE(equal_points(B1,B2));
                REQUIRE(are_coplanar(aBc,cBb));

                REQUIRE( aBc.surface_area() + cBb.surface_area() == Approx(abc.surface_area()));
            }
        }

        WHEN("A plane in the yz plane splits the triangle")
        {
            hyperplane splitting_plane( c, glm::normalize(a-b));

            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 2 triangles. One inside and one outside")
            {
                REQUIRE( below.size() == 1);
                REQUIRE( above.size() == 1);
            }
            THEN("The two new triangles are aBC and cBb")
            {
                auto & aBc = above.front();
                auto & cBb = below.front();

                REQUIRE( contains_point(aBc, a));
                REQUIRE( contains_point(aBc, c));

                REQUIRE( contains_point(cBb, c));
                REQUIRE( contains_point(cBb, b));

                auto B1 = missing_point(aBc,a,c);
                auto B2 = missing_point(cBb,c,b);

                REQUIRE(equal_points(B1,B2));
                REQUIRE(are_coplanar(aBc,cBb));

                REQUIRE( aBc.surface_area() + cBb.surface_area() == Approx(abc.surface_area()));
            }
        }

        WHEN("A plane in the yz plane splits the triangle")
        {
            hyperplane splitting_plane( b, glm::normalize(c-a));


            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 2 triangles. One inside and one outside")
            {
                REQUIRE( below.size() == 1);
                REQUIRE( above.size() == 1);
            }
            THEN("The two new triangles are aBC and cBb, with B being the")
            {
                auto & abC = below.front();
                auto & cCb = above.front();

                REQUIRE( contains_point(abC, a) );
                REQUIRE( contains_point(abC, b) );

                REQUIRE( contains_point(cCb, c) );
                REQUIRE( contains_point(cCb, b) );

                auto C1 = missing_point(cCb,c,b);
                auto C2 = missing_point(abC,a,b);

                REQUIRE( equal_points(C1,C2));
                REQUIRE(are_coplanar(abC,cCb));

                REQUIRE( abC.surface_area() + cCb.surface_area() == Approx(abc.surface_area()));
            }
        }

        WHEN("A plane in the yz plane splits the triangle")
        {
            hyperplane splitting_plane( b, glm::normalize(a-c));


            std::vector<triangle> below, above;
            abc.split(splitting_plane, below, above);

            THEN("The triangle can be split into 2 triangles. One inside and one outside")
            {
                REQUIRE( below.size() == 1);
                REQUIRE( above.size() == 1);
            }
            THEN("The two new triangles are aBC and cBb, with B being the")
            {
                auto & abC = above.front();
                auto & cCb = below.front();

                REQUIRE( contains_point(abC, a) );
                REQUIRE( contains_point(abC, b) );

                REQUIRE( contains_point(cCb, c) );
                REQUIRE( contains_point(cCb, b) );

                auto C1 = missing_point(cCb,c,b);
                auto C2 = missing_point(abC,a,b);

                REQUIRE( equal_points(C1,C2));
                REQUIRE(are_coplanar(abC,cCb));

                REQUIRE( abC.surface_area() + cCb.surface_area() == Approx(abc.surface_area()));
            }
        }
    }
}


