#pragma once

#define CAMERA_DISTANCE 500.0
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "math/vector3.hpp"
#include "shapes/shape.hpp"


class Cube : public Shape  {
	public:
		Cube(Vector3f size = Vector3f(10, 10, 10), Vector3f pos = Vector3f(0, 0, 0));
		void setFaceColor(const unsigned face, const sf::Color color);
		void setFacesColors(const sf::Color colors[6]);
	
		sf::Color color;
	
	private:
		void shape_update() override;
		void shape_init() override;
		
};