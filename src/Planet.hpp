#pragma once
#include <SFML/System/Vector2.hpp>

struct Planet
{
	// Physics / Simulation data
	sf::Vector2f position;
	float radius = 10.0f;	// default radius = 10

	// Visual Representation
	sf::CircleShape shape;
};