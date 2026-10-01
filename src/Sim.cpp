#include <SFML/Graphics.hpp>
#include "Sim.hpp"
#include "Physics.hpp"
#include "configurations.hpp"

sf::RectangleShape createVelocityVectorLine(const phy::RigidBody& body, float scaleFactor);

void Sim::addBody(const phy::RigidBody& body)
{
	this->bodies.push_back(body);
}

void Sim::update(float dt)
{
	phy::computeGravity(*this);

	// -- Controls --
	
	// Check for any bodies
	if (!bodies.empty())
	{
		auto& player = bodies[0];	// Set 1st body as a player
		float thrustPower = 100000.0f;	// Force applied per frame

		// Check if keys currently being held down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {	// W - key
			player.net_force.y += thrustPower;	// upward force
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {	// S - key
			player.net_force.y -= thrustPower;	// downward force
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {	// A - key
			player.net_force.x -= thrustPower;	// leftward force
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {	// D - key
			player.net_force.x += thrustPower;	// rightward force
		}
	}

	for (auto& body : bodies) {

		// Movement
		sf::Vector2f acce = body.net_force / body.mass;
		body.velocity += acce * conf::dt;
		body.position += body.velocity * conf::dt;

		// Border collision
		// Border collision Y
		if (body.position.y - body.radius < 0) {
			body.position.y = body.radius; // Reset to surface
			body.velocity.y *= -0.95f;
		}
		else if (body.position.y + body.radius > conf::window_size.y) {
			body.position.y = conf::window_size.y - body.radius; // Reset to surface
			body.velocity.y *= -0.95f;
		}

		// Border collision X
		if (body.position.x - body.radius < 0) {
			body.position.x = body.radius;
			body.velocity.x *= -0.95f;
		}
		else if (body.position.x + body.radius > conf::window_size.x) {
			body.position.x = conf::window_size.x - body.radius;
			body.velocity.x *= -0.95f;
		}

		// Sync graphics
		body.shape.setPosition(body.position);
	}
}

void Sim::render(sf::RenderWindow& window)
{
	// Render Window
	for (auto& body : bodies) {
		// Physics Bodies
		window.draw(body.shape);

		// Velocity vectors
		sf::RectangleShape velLine = createVelocityVectorLine(body, 0.1f);
		window.draw(velLine);
	}

}

sf::RectangleShape createVelocityVectorLine(const phy::RigidBody& body, float scaleFactor = 0.1f)
{
	// 1. Calculate the speed (length of the vector line)
	float speed = body.velocity.length();

	// If the body isn't moving, return a 0-size invisible shape
	if (speed < 0.1f) return sf::RectangleShape();

	// 2. Create a thin line (Width = length of line, Height = thickness)
	// We multiply speed by a scaleFactor so it doesn't stretch off the screen
	sf::RectangleShape line({ speed * scaleFactor, 3.0f });

	// 3. Center the origin vertically so it rotates cleanly from the middle-left edge
	line.setOrigin({ 0.f, 1.5f });
	line.setPosition(body.position);

	// 4. Calculate the angle in radians, then convert to degrees for SFML
	// Standard Math: atan2(y, x). 
	float angleRadians = std::atan2(body.velocity.y, body.velocity.x);
	float angleDegrees = angleRadians * (180.f / 3.14159265f);

	line.setRotation(sf::degrees(angleDegrees));
	line.setFillColor(sf::Color::Red);

	return line;
}