#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/triangle.h>

bool equal_points(glm::vec3 const &a, glm::vec3 const & b)
{
    return (Catch::Detail::Approx(a.x) == b.x) && (Catch::Detail::Approx(a.y) == b.y) && (Catch::Detail::Approx(a.z) == b.z);
}

// checks if two triangles are equal:
// abc == bca == bca
bool equal_triangles(gcgs::triangle const & abc, gcgs::triangle const & ABC)
{
    return
    (equal_points( abc[0], ABC[0]) && equal_points( abc[1], ABC[1]) && equal_points( abc[2], ABC[2])) ||
    (equal_points( abc[0], ABC[1]) && equal_points( abc[1], ABC[2]) && equal_points( abc[2], ABC[0])) ||
    (equal_points( abc[0], ABC[2]) && equal_points( abc[1], ABC[0]) && equal_points( abc[2], ABC[1]));
    //return (Catch::Detail::Approx(a.x) == b.x) && (Catch::Detail::Approx(a.y) == b.y) && (Catch::Detail::Approx(a.z) == b.z);
}

// returns true if b is in the triangle abc
bool contains_point(gcgs::triangle const & abc, glm::vec3 const & b)
{
    return equal_points(abc[0], b) || equal_points(abc[1], b) || equal_points(abc[2], b);
}

// returns the index the point is located at
int point_index(gcgs::triangle const & abc, glm::vec3 const & b)
{
    for(int i=0;i<3;i++)
    {
        if( equal_points(abc[i],b) )
            return i;
    }
    return -1;
}

// given a triangle abc and two points from that triangle, return the third point
glm::vec3 missing_point(gcgs::triangle const & abc, glm::vec3 const & a, glm::vec3 const & b)
{
    auto i = point_index(abc,a)+1;
    auto j = point_index(abc,b)+1;

    return abc[6 - (i+j) -1];

}

// two triangles are coplanar if their normals are the same
bool are_coplanar(gcgs::triangle const & T1, gcgs::triangle const & T2)
{
    return glm::length(T1.get_hyperplane().normal() - T2.get_hyperplane().normal()) == Catch::Detail::Approx(0.0f);
}

// two triangles are antiplanar if their normals are negatives of each other
bool are_antiplanar(gcgs::triangle const & T1, gcgs::triangle const & T2)
{
    return glm::length(T1.get_hyperplane().normal() + T2.get_hyperplane().normal()) == Catch::Detail::Approx(0.0f);
}

SCENARIO( "Local Tests" )
{
    glm::vec3 a( 1 ,  0 , 0);
    glm::vec3 b(-1 ,  1 , 0);
    glm::vec3 c(-1 , -1 , 0);

    GIVEN("Three triangles abc, bca, cba")
    {
        gcgs::triangle abc{a,b,c};
        gcgs::triangle bca{b,c,a};
        gcgs::triangle cab{c,a,b};

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
        gcgs::triangle abc{a,b,c};
        gcgs::triangle cba{c,b,a};

        THEN("triangles are not equal because of opposite winding order")
        {
            REQUIRE( !equal_triangles(abc,cba) );
            REQUIRE( !are_coplanar(abc,cba) );
            REQUIRE( are_antiplanar(abc,cba) );
        }
    }

    GIVEN("a triangle abc")
    {
        gcgs::triangle abc{a,b,c};

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
        gcgs::triangle T1{a,b,c};

        glm::vec3 o(1,2,3);
        gcgs::triangle T2{a+o,b+o,c+o};

        THEN("triangles are coplanar")
        {
            REQUIRE( are_coplanar(T1,T2));
            REQUIRE( !are_antiplanar(T1,T2));
        }
    }

    GIVEN("two triangles whose points are translated")
    {
        gcgs::triangle T1{a,b,c};
        gcgs::triangle T2{c,b,a};

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


SCENARIO( "Splitting a Triangle into two sub triangles" ) {

    GIVEN( "a hyperplane in the xy plane and a triangle abc" )
    {
        glm::vec3 a( 1 ,  0 , 0);
        glm::vec3 b(-1 ,  1 , 0);
        glm::vec3 c(-1 , -1 , 0);

        gcgs::triangle abc{a,b,c};

        WHEN("A plane in the yz plane splits the triangle")
        {
            gcgs::hyperplane<3> splitting_plane( c, glm::normalize(b-a));

            std::vector<gcgs::triangle> below, above;
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
            gcgs::hyperplane<3> splitting_plane( c, glm::normalize(a-b));

            std::vector<gcgs::triangle> below, above;
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
            gcgs::hyperplane<3> splitting_plane( b, glm::normalize(c-a));


            std::vector<gcgs::triangle> below, above;
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
            gcgs::hyperplane<3> splitting_plane( b, glm::normalize(a-c));


            std::vector<gcgs::triangle> below, above;
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


