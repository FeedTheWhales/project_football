#include "GameScene.hpp"
#include <utility>
#include <string>

#include "../Physics/PhysicsUtils.hpp"
#include "../Physics/Physics.hpp"

GameScene::GameScene()
    : m_player(200.f, Physics::GROUND_Y - Player::RADIUS)
    , m_ball(600.f, 200.f)
    , m_sky({ Physics::WINDOW_WIDTH, Physics::GROUND_Y })    
    , m_ground({ Physics::WINDOW_WIDTH, Physics::WINDOW_HEIGHT - Physics::GROUND_Y })
{
    m_sky.setPosition({ 0.f, 0.f });
    m_sky.setFillColor(sf::Color(135, 206, 235));

    m_ground.setPosition({ 0.f, Physics::GROUND_Y });
    m_ground.setFillColor(sf::Color(80, 160, 80));

    // HUD
    if (hudFont.loadFromFile("Assets/Fonts/BoldPixels.png", { 10, 16 }, 16))
    {
        hudFont.setScale(0.8f);
    }
    tempo.setFillColor(sf::Color::Magenta);
    tempo.setPosition({ 0.f, 0.f });
    tempo.setSize(kVirtualSizeF);
}

std::string hudMessage;

void GameScene::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    // Pause overlay
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Escape)
        {
            requested = SceneTransition{ TransitionOp::Push, SceneType::Pause };
            return;
        }
        
    }
    hudMessage.clear();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        hudMessage += "You are moving left!\n";
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        hudMessage += "You are moving right!\n";

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        hudMessage += "You are moving up!\n";
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        hudMessage += "You are moving down!\n";
    }
}

void GameScene::update(sf::Time dt)
{
    m_player.handleInput(dt);
    m_player.update(dt);
    m_ball.update(dt);
    resolvePlayerBallCollision();
    resolveKick();
}

void GameScene::draw(sf::RenderWindow& window)
{
    window.draw(tempo);

    window.draw(m_sky);
    window.draw(m_ground);
    m_ball.draw(window);
    m_player.draw(window);


    // HUD
    std::string t;
	t += "Hello World!\n";
    t += hudMessage;

    hudFont.draw(window, t, { 12.f, 10.f });

}

std::optional<SceneTransition> GameScene::pollTransition()
{
    return std::exchange(requested, std::nullopt);
}

void GameScene::resolvePlayerBallCollision()
{
    sf::Vector2f bPos = m_ball.getPosition();
    sf::Vector2f pPos = m_player.getPosition();
    float        bR = m_ball.getRadius();
    float        pR = m_player.getRadius();

    sf::Vector2f normal;
    float        penetration;

    if (!PhysicsUtils::circleVsCircle(bPos, bR, pPos, pR, normal, penetration))
        return;

    // Push ball out of player to prevent sticking
    m_ball.setPosition(bPos + normal * (penetration + 5.f));

    sf::Vector2f ballVel = m_ball.getVelocity();
    sf::Vector2f playerVel = m_player.getVelocity();

    const bool isHeadHit = (normal.y < -0.3f); // ball is above player center

    if (isHeadHit)
    {
        // Strong upward kick + directional horizontal bias
        float horizKick = playerVel.x * Physics::HEAD_HORIZ_MUL
            + normal.x * Physics::HEAD_HORIZ_ADD;

        m_ball.setVelocity({ horizKick, Physics::HEAD_BOUNCE_VY });
        m_player.onHeadBall();
    }
    else
    {
        // Elastic-ish reflection: only resolve if ball is moving toward player
        float velAlongNormal = PhysicsUtils::dot(ballVel, normal);
        if (velAlongNormal < 0.f)
        {
            constexpr float restitution = 0.6f;
            sf::Vector2f impulse = normal * (-(1.f + restitution) * velAlongNormal);
            m_ball.applyImpulse(impulse);
        }
    }
}

// Checks if the player is trying to kick the ball and applies an appropriate launch velocity if so.
void GameScene::resolveKick()
{
    bool keyHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);

    // Reset when player releases the key so next press can kick again
    if (!keyHeld)
    {
        m_kickedThisPress = false;
        return;
    }

    // Key is held but we already fired this press — do nothing
    if (m_kickedThisPress)
        return;

    sf::FloatRect zone = m_player.getKickZone();
    sf::Vector2f  bPos = m_ball.getPosition();

    if (!zone.contains(bPos))
        return;

	// The kick strength and angle depend on how far into the kick zone the ball is, with a curve to it.
    float zoneLeft  = zone.position.x;
    float zoneRight = zone.position.x + zone.size.x;
    float t = std::clamp((bPos.x - zoneLeft) / (zoneRight - zoneLeft), 0.f, 1.f);

    constexpr float angleNear = 80.f;
    constexpr float angleFar  = 10.f;
    constexpr float speedNear = 520.f;
    constexpr float speedFar  = 360.f;

    float angleDeg = angleNear + t * (angleFar - angleNear);
    float angleRad = angleDeg * (3.14159265f / 180.f);
    float speed = (speedNear + t * (speedFar - speedNear)) * Physics::KICK_POWER;

    float vx =  speed * std::cos(angleRad);
    float vy = -speed * std::sin(angleRad);

    m_ball.setVelocity({vx, vy});

    m_kickedThisPress = true;   // lock out further kicks until key is released
}