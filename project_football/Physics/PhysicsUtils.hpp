#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>
#include <algorithm>

namespace PhysicsUtils
{
	// Returns the length of a vector.
    inline float length(sf::Vector2f v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

	// Returns a unit vector in the same direction as v, or (0, 0) if v is very short.
    inline sf::Vector2f normalize(sf::Vector2f v)
    {
        float len = length(v);
        if (len < 0.0001f) return { 0.f, 0.f };
        return v / len;
    }

	// Returns the dot product of two vectors.
    inline float dot(sf::Vector2f a, sf::Vector2f b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // Returns true when two circles overlap.
    // outNormal  : unit vector pointing FROM b TO a  (push direction for a).
    // outPenetration : overlap depth in pixels.

    // Returns true when a circle overlaps an axis-aligned rectangle.
    // outNormal     : unit vector pointing away from the rect surface (push direction for circle).
    // outPenetration: overlap depth in pixels.
    inline bool circleVsAABB(
        sf::Vector2f circlePos, float radius,
        sf::FloatRect rect,
        sf::Vector2f& outNormal,
        float& outPenetration)
    {
        float cx = std::clamp(circlePos.x, rect.position.x, rect.position.x + rect.size.x);
        float cy = std::clamp(circlePos.y, rect.position.y, rect.position.y + rect.size.y);

        sf::Vector2f delta = circlePos - sf::Vector2f{ cx, cy };
        float dist = length(delta);

        if (dist >= radius) return false;

        outNormal      = (dist > 0.0001f) ? delta / dist : sf::Vector2f{ 0.f, -1.f };
        outPenetration = radius - dist;
        return true;
    }

	// Note: this does not handle the case of two circles in the same position (zero distance). 버그 생길 수도? 
    inline bool circleVsCircle(
        sf::Vector2f posA, float rA,
        sf::Vector2f posB, float rB,
        sf::Vector2f& outNormal,
        float& outPenetration)
    {
        sf::Vector2f delta = posA - posB;
        float dist = length(delta);
        float sumR = rA + rB;

        if (dist >= sumR) return false;

		outNormal = (dist > 0.0001f) ? delta / dist : sf::Vector2f{ 0.f, -1.f }; // Arbitrary normal if centers are the same
		outPenetration = sumR - dist; // how much we need to push A out of B to resolve the collision 겹침 정도
        return true;
    }
}