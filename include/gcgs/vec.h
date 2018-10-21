#pragma once
#ifndef GCGS_VEC_H
#define GCGS_VEC_H

#include <glm/glm.hpp>

#include<type_traits>

namespace gcgs
{

template<uint32_t length>
using vec =  typename std::conditional<length==2, glm::vec2, glm::vec3>::type;

using vec3 = vec<3>;
using vec2 = vec<2>;

}

#endif


