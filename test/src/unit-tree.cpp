#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <gcgs/triangle.h>
#include <gcgs/tree.h>

#include "geometry_tests.h"

using value_type = float;
using vec = gcsg::vec3<value_type>;
using triangle = gcsg::triangle<value_type>;
using hyperplane = gcsg::hyperplane<value_type,3>;

SCENARIO( "Triangle Hyperplanes" ) {

    GIVEN( "A line segment ab and ba" )
    {
        // on the xy plane
        vec a( 1 ,  0 , 0);
        vec b(-1 ,  1 , 0);
        vec c(-1 , -1 , 0);

        triangle abc{a,b,c};

        gcsg::Tree<3, triangle> T;

        T.add(abc);

        WHEN("Triangle ABC which is above abc is added to the plane")
        {
            // above the xy plane
            vec t( 0 ,  0 , 1);
            triangle ABC{a+t,b+t,c+t};

            T.add( ABC );

            THEN("The front subtree must be valid and the back subtree must not")
            {
                REQUIRE( T.m_root->m_front );
                REQUIRE( !T.m_root->m_back );
            }
        }
        WHEN("Triangle ABC which is below abc is added to the plane")
        {
            // above the xy plane
            vec t( 0 ,  0 , -1);
            triangle ABC{a+t,b+t,c+t};

            T.add( ABC );

            THEN("The front subtree must be valid and the back subtree must not")
            {
                REQUIRE( !T.m_root->m_front );
                REQUIRE( T.m_root->m_back );
            }
        }
    }
}



SCENARIO( "Quad Planes" ) {

    using TreeType = gcsg::Tree<3, triangle>;
    GIVEN( "A line segment ab and ba" )
    {
        // on the xy plane
        vec a( 1 ,  0 , 0);
        vec b( 0 ,  0 , 0);
        vec c( 0 ,  0 , 1);
        vec d( 1 ,  0 , 1);

        WHEN("Two triangles which are coplanar (in the xz plane) are added to an empty tree")
        {
            triangle t1{a,c,b};
            triangle t2{a,d,c};

            auto n1 = t1.get_hyperplane().normal();
            auto n2 = t1.get_hyperplane().normal();

            REQUIRE( equal_points(n1,n2));
            REQUIRE( equal_points(n1, vec(0,-1,0)));

            TreeType T;

            T.add(t1);
            T.add(t2);

            THEN("Then T2 is considered behind T1")
            {
                REQUIRE( !T.m_root->m_front );
                REQUIRE( T.m_root->m_back );
                REQUIRE( T.m_root->count() == 2);
            }

            WHEN("A triangle which is in the xy plane and intersects the plane is added")
            {
                vec A(  0 , -1 , 0);
                vec B(  2 ,  1 , 0);
                vec C( -2,   1 , 0);

                triangle ABC{A,B,C};

                THEN("Triangle ABC is partioned by the tree")
                {
                    REQUIRE( TreeType::is_partitioned(T.m_root.get(), ABC) );
                }
                THEN("The partitioning triangle is t1")
                {
                    auto P = TreeType::partitioning_face(T.m_root.get(), ABC);

                    REQUIRE( equal_triangles(P, t1));
                }
                WHEN("Triangle ABC is added to the tree")
                {
                    std::vector<triangle> below, above;
                    auto h1 = t1.get_hyperplane();

                    ABC.split( h1, below, above);

                    REQUIRE( below.size() == 2);
                    REQUIRE( above.size() == 1);

                    REQUIRE( below[0].surface_area() +
                             below[1].surface_area()  +
                             above[0].surface_area() == Approx(ABC.surface_area()));

                    T.add(ABC);

                    REQUIRE( T.m_root->count() == 5);
                    REQUIRE( T.m_root->m_front->count() == 1);
                    REQUIRE( T.m_root->m_back->count() == 3);
                }

            }

        }


    }
}
