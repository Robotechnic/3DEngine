#pragma once

#define CAMERA_DISTANCE 500.0
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>
#include "math/vector3.h"
#include "shape.h"


class Cube : public Shape  {
	public:
		Cube(Vector3f size, Vector3f pos = Vector3f(0, 0, 0));

		std::vector<Triangle> getTriangles() const;
		std::vector<sf::Color> getColors() const;
		
};