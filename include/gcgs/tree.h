#pragma once
#ifndef CGCS_TREE_H
#define CGCS_TREE_H


#include <ostream>
#include <memory>
#include <iostream>
#include <vector>

#include "hyperplane.h"
#include "log.h"

namespace gcgs
{

#define EPSILON 1e-6



template<class face_t, class plane_t, class point_t>
class Node
{
public:
                              //   3D                 2D
    using face_type  = face_t;  //  triangle           line_segment
    using plane_type = plane_t;  //  plane              line
    using point_type = point_t;

    face_type  m_face;
    plane_type m_plane;

    Node()
    {}

    Node(face_type const & f) : m_face(f)
    {
        m_plane = m_face.get_hyperplane();
    }

    size_t count() const
    {
        size_t s = 1;
        if(m_back)
            s += m_back->count();
        if(m_front)
            s += m_front->count();
        return s;
    }

    std::unique_ptr<Node> m_back;
    std::unique_ptr<Node> m_front;

};


template<uint32_t N, class face_t>
/**
 * @brief The Tree class
 *
 * The tree class is a generic container for 2d and 3d surface segemnts.
 *
 * In 2D the surface segment is a Line Segement. In 3D it is a triangle (or possibly a quad)
 */
class Tree
{
public:
                                                     //   3D                 2D
    using face_type  = face_t;                       //  triangle           line_segment
    using plane_type = typename face_t::plane_type;  //  plane              line
    using point_type = typename face_t::point_type;

    using tree_type = Tree<N, face_t>;
    using node_type = Node<face_type, plane_type, point_type>;

    std::unique_ptr<node_type> m_root;

#if defined GCSG_USE_SPDLOG
    static std::shared_ptr<spdlog::logger> get_logger()
    {
        static auto l = CREATE_LOGGER("Tree");
        return l;
    }
#endif

    Tree()
    {

    }
    Tree(tree_type const & other)
    {
        other.for_each(
        [&](face_type const & L)
        {
            add(L);
        });
    }

    tree_type & operator=(tree_type const & other)
    {
        if( &other != this)
        {
            m_root.reset();
            other.for_each(
            [&](face_type const & L)
            {
                add(L);
            });
        }
        return *this;
    }


    tree_type & operator=(tree_type && other)
    {
        if( &other != this)
        {
            m_root = std::move( other.m_root);
        }
        return *this;
    }

    tree_type Union(tree_type const & S2) const
    {
        using namespace gcgs;

        tree_type inside;
        tree_type outside;

        // would need to traverse S2 to get all the triangles instead
        // of looping through B2.
        S2.for_each(
        [&](face_type const & L)
        {
            partition(L, inside, outside);
        });
        for_each(
        [&](face_type const & L)
        {
            S2.partition(L, inside, outside);
        });

        return outside;
    }

    tree_type Intersect(tree_type const & S2) const
    {
        using namespace gcgs;

        tree_type inside;
        tree_type outside;

        // would need to traverse S2 to get all the triangles instead
        // of looping through B2.
        S2.for_each(
        [&](face_type const & L)
        {
            partition(L, inside, outside);
        });
        for_each(
        [&](face_type const & L)
        {
            S2.partition(L, inside, outside);
        });

        return inside;
    }

    tree_type Difference(tree_type const & S2) const
    {
        return Intersect( S2.Invert() );
    }

    tree_type Invert() const
    {
        tree_type ret;
        for_each(
        [&](face_type & L)
        {
            ret.add( L.invert() );
        });
        return  ret;

    }

    /**
     * @brief add
     * @param T
     *
     * Adds a face to the Tree. Any changes added using this method should be a
     * a valid face on the surface. that is, the face should not intersect any other
     * faces within the surface.
     */
    void add(face_type const & T)
    {
        #if defined GCSG_USE_SPDLOG
        auto log = get_logger();
        #endif

        if( !m_root.get() )
        {
            _DEBUG(log, "{} inserted as root node", T);
            m_root.reset( new node_type(T)  );
        }
        else
        {
            __add(m_root.get(), T);
        }
    }


    void partition( face_type const & T, tree_type & inside, tree_type & outside) const
    {
        __partition( m_root.get(), T, inside, outside);
    }

    template<typename callable>
    void for_each( callable c)
    {
        if( m_root.get() ) __for_each( m_root.get(), c);
    }

    template<typename callable>
    void for_each( callable c) const
    {
        if( m_root.get() ) __for_each( m_root.get(), c);
    }
    /**
     * @brief print
     *
     * Prints all the faces in the tree to std out
     */
    void print()
    {
        if( m_root.get() )
            __print( m_root.get() );
    }

protected:
    template<typename callable>
    static void __for_each(node_type * n, callable c)
    {
        c(n->m_face);
        if( n->m_back.get() ) __for_each( n->m_back.get(), c);
        if( n->m_front.get() ) __for_each( n->m_front.get(), c);
    }

    template<typename callable>
    static void __for_each(node_type const * n, callable c)
    {
        c(n->m_face);
        if( n->m_back.get() ) __for_each( n->m_back.get(), c);
        if( n->m_front.get() ) __for_each( n->m_front.get(), c);
    }

    static void __print(node_type * n)
    {
        if(n->m_back.get())  __print(n->m_back.get());
        if(n->m_front.get()) __print(n->m_front.get());
        std::cout << n->m_face << std::endl;
    }

    static void __partition(node_type const * n, face_type const & T, tree_type & inside, tree_type & outside)
    {
        #if defined GCSG_USE_SPDLOG
        auto log = get_logger();
        auto & face = n->m_face;
        #endif
        assert( n );
        auto & plane = n->m_plane;

        // Distance between point and plane
        std::array<float, N> f;

        int32_t i=0;
        bool all_inside  = true;
        bool all_outside = true;

        for(auto & _f : f)
        {
            _f = plane.distance( T[i++]);
            if (fabs(_f) < EPSILON ) _f = 0.0f;

            all_inside  &= _f <= 0;
            all_outside &= _f >= 0;
        }

        auto & m_back  = n->m_back;
        auto & m_front = n->m_front;
        if(all_inside) //if( f0 <= 0 && f1 <= 0 && f2 <= 0 )
        {
            _DEBUG(log, "{} is behind {}", T, face);
            if( !m_back.get() )
                inside.add(T);
            else
                __partition(n->m_back.get(), T, inside, outside);
        }
        else if(all_outside) //( f0 >= 0 && f1 >= 0 && f2 >= 0 )
        {
            _DEBUG(log, "{} is in front of{}", T, face);
            if( !m_front.get() )
            {
                outside.add(T);
            }
            else
            {
                __partition(n->m_front.get(), T, inside, outside);
            }
        }
        else // the face spans the dividing plane.
        {
            //std::cout << T << " is split by " << face << std::endl;
            std::vector<face_type> inside_faces;
            std::vector<face_type> outside_faces;

            // split the face and return a vector of face inside and outside the plane
            //std::cout << "Splitting Line: " << T << std::endl;
            T.split( plane, inside_faces, outside_faces);

#if defined GCSG_USE_SPDLOG
            for(auto & i : inside_faces)
                _DEBUG(log, "  inside: {}" ,i);
            for(auto & i : outside_faces)
                _DEBUG(log, "  outside: {}" ,i);
#endif

            for(auto & t : inside_faces)
            {
                __partition(n, t, inside, outside);
            }
            for(auto & t : outside_faces)
            {
                __partition(n, t, inside, outside);
            }
        }
    }

    static void __add(node_type * n, face_type const & T)
    {
#if defined GCSG_USE_SPDLOG
auto log = get_logger();
        auto & face = n->m_face;
#endif
        auto & plane = n->m_plane;

        // Distance between point and plane
        std::array<float, N> f;

        int32_t i=0;
        bool all_inside  = true;
        bool all_outside = true;

        for(auto & _f : f)
        {
            _f = plane.distance( T[i++]);
            if (fabs(_f) < EPSILON ) _f = 0.0f;

            all_inside  &= _f <= 0;
            all_outside &= _f >= 0;
        }

        if(all_inside) //if( f0 <= 0 && f1 <= 0 && f2 <= 0 )
        {
            _DEBUG(log, "{} is behind {}", T, face);
            __add_back(n, T);
        }
        else if(all_outside) //( f0 >= 0 && f1 >= 0 && f2 >= 0 )
        {
            _DEBUG(log, "{} is in front of{}", T, face);
            __add_front(n, T);
        }
        else // the face spans the dividing plane.
        {
            //std::cout << T << " is split by " << face << std::endl;
            std::vector<face_type> inside_faces;
            std::vector<face_type> outside_faces;

            // split the face and return a vector of face inside and outside the plane
            //std::cout << "Splitting Line: " << T << std::endl;
            T.split( plane, inside_faces, outside_faces);

            #if defined GCSG_USE_SPDLOG
            for(auto & i : inside_faces)
                _DEBUG(log, "  inside: {}" ,i);
            for(auto & i : outside_faces)
                _DEBUG(log, "  outside: {}" ,i);
            #endif

            for(auto & t : inside_faces)
            {
                __add_back(n, t);
            }
            for(auto & t : outside_faces)
            {
                __add_front(n, t);
            }
        }
    }

    static void __add_front(node_type * n, face_type const & t)
    {
#if defined GCSG_USE_SPDLOG
auto log = get_logger();
#endif
        auto & m_front = n->m_front;

        if( !m_front.get() )
        {
            _DEBUG(log, "{} added to front of {}", t, n->m_face);
            m_front.reset( new node_type(t) );
        }
        else
        {
            __add(m_front.get(), t);
        };
    }

    static void __add_back(node_type *n, face_type const & t)
    {
#if defined GCSG_USE_SPDLOG
auto log = get_logger();
#endif

        auto & m_back  = n->m_back;

        if( !m_back.get() )
        {
            _DEBUG(log, "{} added to back of {}", t, n->m_face);
            m_back.reset( new node_type(t) );
        }
        else
        {
            //printf("__add back\n");
            __add(m_back.get(), t);
        }
    }
};



}


#endif
