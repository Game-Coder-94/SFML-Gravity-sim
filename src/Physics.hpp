#pragma once
#include <SFML/Graphics.hpp>

class Sim;

namespace phy
{
	struct RigidBody
	{
		// Physics props
		sf::Vector2f position;
		sf::Vector2f velocity = { 0.0f,0.0f };
		sf::Vector2f net_force = { 0.0f, 0.0f };
		float mass;
		float radius;

		// Graphical props
		sf::CircleShape shape;
		
	};

	RigidBody createBody(sf::Vector2f position, float mass, float radius);
	void computeGravity(Sim& sim);
	void applyGravity(RigidBody& b1, RigidBody& b2);
}