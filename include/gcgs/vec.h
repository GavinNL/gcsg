#pragma once
#ifndef GCGS_VEC_H
#define GCGS_VEC_H

#include <glm/glm.hpp>

#include<type_traits>

namespace gcsg
{

//template<uint32_t length>
//using vec =  typename std::conditional<length==2, glm::vec2, glm::vec3>::type;

template<typename T=float>
using vec3 = glm::tvec3<T, glm::highp>; //vec<3>;

template<typename T=float>
using vec2 = glm::tvec2<T, glm::highp>; //vec<2>;

using vec3f = vec3<float>;
using vec2f = vec2<float>;

using vec3lf = vec3<double>;
using vec2lf = vec2<double>;

}

#endif


