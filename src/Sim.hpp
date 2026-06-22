#pragma once
#include <vector>
#include "Physics.hpp"

class Sim {
private:
	std::vector<phy::RigidBody> bodies;
	uint32_t count;

public:
	Sim() = default;

	void addBody(const phy::RigidBody& body);
	void update(float dt);
	void render(sf::RenderWindow& window);

	std::vector<phy::RigidBody>& getBodies() { return bodies; }
	const std::vector<phy::RigidBody>& getBodies() const { return bodies; }
};