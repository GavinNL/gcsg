#pragma once
#ifndef CGCS_TREE_H
#define CGCS_TREE_H


#include <ostream>
#include <memory>
#include <iostream>
#include <vector>


#include <glm/glm.hpp>
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

    using node_type = Node<face_type, plane_type, point_type>;

    std::unique_ptr<node_type> m_root;

    /**
     * @brief add
     * @param T
     *
     * Adds a face to the Tree
     */
    void add(face_type const & T)
    {
#if defined GCGS_USE_SPDLOG
        static auto log = CREATE_LOGGER("Tree::add");
        log->set_level(spdlog::level::debug);
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

    template<typename callable>
    void for_each( callable c)
    {
        if( m_root.get() ) __for_each( m_root.get(), c);
    }

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

    static void __print(node_type * n)
    {
        if(n->m_back.get())  __print(n->m_back.get());
        if(n->m_front.get()) __print(n->m_front.get());
        n->m_face.print();
    }
    static void __add(node_type * n, face_type const & T)
    {
        #if defined GCGS_USE_SPDLOG
            static auto log = CREATE_LOGGER("Tree::__add");
            log->set_level(spdlog::level::debug);
        #endif
        auto & face  = n->m_face;
        auto & plane = n->m_plane;

        // Distance between point and plane
        std::array<float, N> f;

        uint32_t i=0;
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
            for(auto & i : inside_faces)
                _DEBUG(log, "  inside: {}" ,i);
            for(auto & i : outside_faces)
                _DEBUG(log, "  outside: {}" ,i);

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
        #if defined GCGS_USE_SPDLOG
            static auto log = CREATE_LOGGER("Tree::__add_front");
            log->set_level(spdlog::level::debug);
        #endif

        auto & m_back  = n->m_back;
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
        #if defined GCGS_USE_SPDLOG
            static auto log = CREATE_LOGGER("Tree::__add_back");
            log->set_level(spdlog::level::debug);
        #endif

        auto & m_back  = n->m_back;
        auto & m_front = n->m_front;

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
