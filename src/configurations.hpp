#pragma once
#include <SFML/Graphics.hpp>

namespace conf
{
	// Window configurations
	sf::Vector2f const window_size = { 1920, 1080 };
	sf::Vector2f const window_size_f = static_cast<sf::Vector2f>(window_size);
	sf::Color const bg = sf::Color(43, 43, 43, 255);
	uint32_t const max_framerate = 144;
	float const dt = 1.0f / static_cast<float>(max_framerate);	// delta time

	// Physics configuration
	float const GRAVITY_CONST = 6.6743 * pow(10, 2);
	

	// Star configuration
	uint32_t const count = 10000;
	float const radius = 20.0f;
	float const far = 10.0f;
	float const near = 0.2f;
	float const speed = 1.0f;
}