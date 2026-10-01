#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>

#include "Physics.hpp"

// Computes points positions for a circle
struct CircleGenerator
{
	static constexpr float pi{ 3.1415927f };
	float const radius;
	uint32_t const quality;
	float const da;

	// The generator's constructor
	CircleGenerator(float radius_, uint32_t quality_)
		: radius{ radius_ }
		, quality{ quality_ }
		, da{ (2.0f * pi) / static_cast<float>(quality_) }
	{}

	// Returns the ith point of the perimeter
	sf::Vector2f getPoint(uint32_t i) const
	{
		// Compute the angle associated with the requested point
		float const angle{ da * static_cast<float>(i) };

		// Fix: Use explicit std:: prefixes for trigonometry functions
		return sf::Vector2f{ std::cos(angle), std::sin(angle) } * radius;
	}
};

// Computes points positions for a rounded rectangle
struct RoundedRectangleGenerator
{
	sf::Vector2f const size;
	sf::Vector2f const centers[4];
	uint32_t const arc_quality;
	CircleGenerator const generator;

	RoundedRectangleGenerator(sf::Vector2f size_, float radius, uint32_t quality)
		: size{ size_ }
		, centers{
			sf::Vector2f{size_.x - radius, size_.y - radius},	// Bottom right
			sf::Vector2f{radius, size_.y - radius},	            // Bottom left
			sf::Vector2f{radius, radius},	                    // Top left
			sf::Vector2f{size_.x - radius, radius}	            // Top right
		}
		, arc_quality{ quality / 4 }
		, generator{ radius, quality - 4 }
	{}

	// Returns the ith point of the perimeter
	sf::Vector2f getPoint(uint32_t i) const
	{
		uint32_t const corner_idx{ i / arc_quality };
		// Protect against index out of bounds on edge selections
		uint32_t const safe_idx = (corner_idx > 3) ? 3 : corner_idx;

		return centers[safe_idx] + generator.getPoint(i - safe_idx);
	}
};

// Adds circle points inside a pre-allocated Vertex array
// In SFML 3, this cleanly populates positions on your drawable primitives
inline void generateCircle(sf::VertexArray& vertex_array, sf::Vector2f position, float radius, uint32_t quality)
{
	CircleGenerator const generator{ radius, quality };

	for (uint32_t i = 0; i < quality; ++i) {
		vertex_array[i].position = position + generator.getPoint(i);
		vertex_array[i].color = sf::Color{ 117, 255, 136 };
	}
}

// Adds rounded rectangle points inside a pre-allocated vertex array
inline void generateRoundedRectangle(sf::VertexArray& vertex_array, sf::Vector2f position, sf::Vector2f size, float radius, uint32_t quality, sf::Color color)
{
	// Create a generator
	RoundedRectangleGenerator const generator{ size, radius, quality };
	//Add the points to vertex array
	for (uint32_t i = 0; i < quality; ++i) {
		vertex_array[i].position = position + generator.getPoint(i);
		vertex_array[i].color = color;
	}
}

void drawVelocityBar(sf::RenderWindow& window, const phy::RigidBody& body, float maxSpeed = 1000.0f)
{
	// Current speed
	float currentSpeed = body.velocity.length();

	// ratio = current speed / max speed
	// Map speed to a 0.0 -> 1.0 percentage scale (clamped so it doesn't break past the bar)
	float speedRatio = currentSpeed / maxSpeed;
	speedRatio = std::min(speedRatio, 1.0f);

	// Define progress bar dimensions
	float bgHeight = 10.0f;
	float bgWidth = 300.0f;
	float bgRadius = 4.0f;
	sf::Vector2f bgPos = { 1410.0f, 130.0f };

	float padding = 3.0f;
	float fgMaxHeight = bgHeight - (2.0f * padding);
	float fgMaxWidth = bgWidth - (2.0f * padding);
	float fgRadius = bgRadius - padding;

	float fgWidth = fgMaxWidth * speedRatio;

	sf::Vector2f fgPos = {
		bgPos.x + padding,
		bgPos.y + padding
	};

	if (fgWidth < fgRadius * 2.0f) {
		fgWidth = fgRadius * 2.0f;
	}

	// Background Track (Empty Bar)
	sf::VertexArray bgBar(sf::PrimitiveType::TriangleFan, 8);
	generateRoundedRectangle(bgBar, bgPos, { bgWidth, bgHeight }, bgRadius, 8, sf::Color(50, 50, 50, 180));

	// Foreground Indicator (Filled Bar)
	// The width changes dynamically: barWidth * speedRatio
	sf::VertexArray fgBar(sf::PrimitiveType::TriangleFan, 8);
	generateRoundedRectangle(fgBar, fgPos, { fgWidth, fgMaxHeight }, fgRadius, 8, sf::Color::White);

	window.draw(bgBar);
	if (speedRatio > 0.01f) { window.draw(fgBar); }
}