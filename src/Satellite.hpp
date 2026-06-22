#pragma once
#include <SFML/System/Vector2.hpp>
#include "Planet.hpp"

struct Satellite
{
	// Physics / Simulation data
	sf::Vector2f position;
	float speed = 1.0f;
	float radius = 5.0f;
	float distance = 200.0f;
	float angle = 0.0f;
	Planet* parent = nullptr;
	
	// Visual representation
	sf::CircleShape shape;
};