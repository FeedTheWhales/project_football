#pragma once
#include <SFML/Graphics.hpp>
#include "Physics.hpp"

class Ball
{
public:
    static constexpr float RADIUS = 18.f;

    Ball(float startX, float startY);

    void update(sf::Time dt);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_vel; }
    float        getRadius()   const { return RADIUS; }

    void setPosition(sf::Vector2f pos) { m_pos = pos; }
    void setVelocity(sf::Vector2f vel) { m_vel = vel; }
    void applyImpulse(sf::Vector2f impulse) { m_vel += impulse; }

private:
    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    float        m_angle = 0.f;   // visual spin in degrees

    // SFML 3: constructed with radius
    sf::CircleShape  m_shape;
    sf::RectangleShape m_stripe;  // spin indicator stripe
};