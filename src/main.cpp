#include <SFML/Graphics.hpp>
#include "configurations.hpp"
#include "events.hpp"
#include "Physics.hpp"
#include "Sim.hpp"
#include "UI.hpp"

int main()
{
	// Simulator intiiliazation
	Sim sim;

	// Window configuration
	sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(conf::window_size.x),
											static_cast<unsigned int>(conf::window_size.y) }),
		"SFML works!",
		sf::State::Fullscreen
	);
	window.setFramerateLimit(conf::max_framerate);
	
	// Entities
	phy::RigidBody body1 = phy::createBody({ (conf::window_size.x * 0.5f), conf::window_size.y * 0.5f }, 100000.0f, 50.0f);
	phy::RigidBody body2 = phy::createBody({ (conf::window_size.x * 0.5f) + 500.0f, conf::window_size.y * 0.5f }, 1000.0f, 20.0f);
		
	// initial stats
	body1.velocity = { 0.0f, 0.0f };
	body2.velocity = { 0.0f, 250.0f };
	
	sim.addBody(body2);
	sim.addBody(body1);
	
	// -- UI --
	// Rounded rectangle properties
	sf::Vector2f position = { 1400.0f, 100.0f };
	sf::Vector2f size = { 400.0f, 200.0f };
	float radius = 40.0f;
	uint32_t quality = 32;

	sf::VertexArray va(sf::PrimitiveType::TriangleFan, 32);
	generateRoundedRectangle(va, position, size, radius, quality, sf::Color(140, 230, 60));

	// 1. Create a view that represents your physics space
	// Arguments: Center X, Center Y, Width, Height
	sf::View physicsView({ conf::window_size.x * 0.5f, conf::window_size.y * 0.5f }, { conf::window_size.x, conf::window_size.y });
	// 2. Flip the Y-axis by making the vertical size negative!
	physicsView.setSize({ conf::window_size.x, -conf::window_size.y });
	
	// Game / Sim Loop
	while ( window.isOpen() )
	{
		// Handle events
		processEvents(window);

		//updatePhysics(&planet, &sat);
		sim.update(conf::dt);

		// Rendering
		window.clear(conf::bg);

		window.setView(physicsView); // 3. Apply the view to the window before your game loop
		
		// Scene Camera
		sim.render(window);

		// Default View
		window.setView(window.getDefaultView());

		// UI rendering
		window.draw(va);
		drawVelocityBar(window, sim.getBodies()[0], conf::MAX_SPEED);

		window.display();
	}

}