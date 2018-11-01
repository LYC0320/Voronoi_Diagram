#pragma once
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>
#include <utility>

using namespace std;
using namespace glm;

class edgePoint
{
public:
	pair<int, int> p;
	vec2 pos;
};