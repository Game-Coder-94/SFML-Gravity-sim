#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdint>

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
inline void generateRoundedRectangle(sf::VertexArray& vertex_array, sf::Vector2f position, sf::Vector2f size, float radius, uint32_t quality)
{
	// Create a generator
	RoundedRectangleGenerator const generator{ size, radius, quality };
	//Add the points to vertex array
	for (uint32_t i = 0; i < quality; ++i) {
		vertex_array[i].position = position + generator.getPoint(i);
		vertex_array[i].color = sf::Color{ 117, 255, 136 };
	}
}