#include "Ball.hpp"
#include "Physics.hpp"
#include <cmath>

Ball::Ball(float startX, float startY)
    : m_pos(startX, startY)
    , m_vel(120.f, 0.f)
    , m_shape(RADIUS)                                       // SFML 3: radius in ctor
    , m_stripe({ RADIUS * 2.f, 4.f })                         // SFML 3: size as Vector2f
{
    // Body
    m_shape.setOrigin({ RADIUS, RADIUS });
    m_shape.setFillColor(sf::Color(230, 60, 60));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color(140, 20, 20));

	// Stripe 공 돌아가는거 보여주는 역할
    m_stripe.setOrigin({ RADIUS, 2.f });
    m_stripe.setFillColor(sf::Color(255, 255, 255, 120));
}

void Ball::update(sf::Time dt)
{
    const float s = dt.asSeconds();
    // Gravity
    m_vel.y += Physics::GRAVITY * s;

	// Air friction (simple exponential decay, independent of velocity)
    m_vel.x *= std::pow(Physics::BALL_FRICTION, s * 60.f);

    m_pos += m_vel * s;

    // ---- Ground bounce ----
    const float groundTop = Physics::GROUND_Y - RADIUS;
    if (m_pos.y >= groundTop)
    {
        m_pos.y = groundTop;
        m_vel.y = -std::abs(m_vel.y) * Physics::BALL_BOUNCE;

        if (std::abs(m_vel.y) < Physics::MIN_BOUNCE_VY)
            m_vel.y = 0.f;
    }

	// ---- Wall bounce ----
    if (m_pos.x < RADIUS)
    {
        m_pos.x = RADIUS;
        m_vel.x = std::abs(m_vel.x) * 0.7f;
    }
    if (m_pos.x > Physics::WINDOW_WIDTH - RADIUS)
    {
        m_pos.x = Physics::WINDOW_WIDTH - RADIUS;
        m_vel.x = -std::abs(m_vel.x) * 0.7f;
    }

    // Visual spin
    m_angle += m_vel.x * s * 2.f;
}

void Ball::draw(sf::RenderWindow& window) const
{
    // We copy the shapes to set position/rotation without mutating members
    // (keeps draw() const – swap to mutable members if you prefer)
    sf::CircleShape    body = m_shape;
    sf::RectangleShape stripe = m_stripe;

    body.setPosition(m_pos);        
    body.setRotation(sf::degrees(m_angle));   

    stripe.setPosition(m_pos);
    stripe.setRotation(sf::degrees(m_angle));

    window.draw(body);
    window.draw(stripe);
}