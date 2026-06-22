#include "configurations.hpp"
#include "Physics.hpp"
#include "Sim.hpp"

namespace phy 
{
	RigidBody createBody(sf::Vector2f position, float mass, float radius)
	{
		RigidBody body;
		sf::CircleShape shape(radius);

		body.position = position;
		body.radius = radius;
		body.mass = mass;

		shape.setOrigin({ radius, radius });
		shape.setPosition(position);
		shape.setFillColor(sf::Color::White);
		body.shape = shape;

		return body;
	}

	void computeGravity(Sim& sim)
	{
		auto& bodies = sim.getBodies();

		// Reset to zero before computing
		for (auto& b : bodies) {
			b.net_force = { 0.0f, 0.0f };
		}

		// Compute gravity of individual planets
		for (int i = 0; i < bodies.size(); ++i) {
			for (int j = i + 1; j < bodies.size(); ++j) {
				applyGravity(bodies[i], bodies[j]);
			}
		}
	}

	// Apply gravity to 1 according to 2 and negative gravity to 2
	void applyGravity(RigidBody& b1, RigidBody& b2)
	{
		sf::Vector2f disVector = b2.position - b1.position;
		float dis = disVector.length();

		if (dis < 1.0f) return;

		sf::Vector2f normal = disVector.normalized();

		float forceMagnitude = static_cast<float>((conf::GRAVITY_CONST * b1.mass * b2.mass) / (dis * dis));

		b1.net_force += normal * forceMagnitude;
		b2.net_force -= normal * forceMagnitude;
	}
}