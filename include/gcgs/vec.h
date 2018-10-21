#pragma once
#ifndef GCGS_VEC_H
#define GCGS_VEC_H

#include <glm/glm.hpp>


namespace gcgs
{

template<int length>
using vec = glm::vec<length, float, glm::highp>;


}

#endif


