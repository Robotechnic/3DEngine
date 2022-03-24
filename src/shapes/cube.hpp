#pragma once

#define CAMERA_DISTANCE 500.0
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>
#include "math/vector3.hpp"
#include "shape.hpp"


class Cube : public Shape  {
	public:
		Cube(Vector3f size = Vector3f(10, 10, 10), Vector3f pos = Vector3f(0, 0, 0));

		void update() override;
		void init() override;
	
		sf::Color color;
		
};