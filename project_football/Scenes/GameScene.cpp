#include "GameScene.hpp"
#include <utility>
#include <string>

#include "../Physics/PhysicsUtils.hpp"
#include "../Physics/Physics.hpp"

GameScene::GameScene()
    : m_player1(1)
    , m_player2(2)
    , m_ball(Physics::WINDOW_WIDTH / 2.f, 200.f)
    , m_sky({ Physics::WINDOW_WIDTH, Physics::GROUND_Y })    
    , m_ground({ Physics::WINDOW_WIDTH, Physics::WINDOW_HEIGHT - Physics::GROUND_Y })
{
    m_sky.setPosition({ 0.f, 0.f });
    m_sky.setFillColor(sf::Color(135, 206, 235));

    m_ground.setPosition({ 0.f, Physics::GROUND_Y });
    m_ground.setFillColor(sf::Color(80, 160, 80));

    // --- Goal visuals ---
    const float GH = Physics::GOAL_HEIGHT;
    const float GD = Physics::GOAL_DEPTH;
    const float GP = Physics::GOAL_POST;
    const float W  = Physics::WINDOW_WIDTH;
    const float GY = Physics::GROUND_Y;

    const sf::Color netColor  { 255, 255, 255, 35 };
    const sf::Color postColor { 240, 240, 240 };

    // Left goal
    m_goalLeftNet.setSize({ GD, GH });
    m_goalLeftNet.setPosition({ 0.f, GY - GH });
    m_goalLeftNet.setFillColor(netColor);

    m_goalLeftCrossbar.setSize({ GD + GP, GP });
    m_goalLeftCrossbar.setPosition({ 0.f, GY - GH - GP });
    m_goalLeftCrossbar.setFillColor(postColor);

    m_goalLeftPost.setSize({ GP, GH + GP });
    m_goalLeftPost.setPosition({ GD, GY - GH - GP });
    m_goalLeftPost.setFillColor(postColor);

    // Right goal (mirror)
    m_goalRightNet.setSize({ GD, GH });
    m_goalRightNet.setPosition({ W - GD, GY - GH });
    m_goalRightNet.setFillColor(netColor);

    m_goalRightCrossbar.setSize({ GD + GP, GP });
    m_goalRightCrossbar.setPosition({ W - GD - GP, GY - GH - GP });
    m_goalRightCrossbar.setFillColor(postColor);

    m_goalRightPost.setSize({ GP, GH + GP });
    m_goalRightPost.setPosition({ W - GD - GP, GY - GH - GP });
    m_goalRightPost.setFillColor(postColor);

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
    m_player1.handleInput(dt);
    m_player1.update(dt);
    m_player2.handleInput(dt);
    m_player2.update(dt);
    m_ball.update(dt);
    resolvePlayerPlayerCollision();
    resolvePlayerBallCollision();
    resolveGoalPostCollision();
    resolveKick();
    checkGoal();
}

void GameScene::draw(sf::RenderWindow& window)
{
    window.draw(tempo);

    window.draw(m_sky);
    window.draw(m_ground);

    // Goal nets (behind ball and players)
    window.draw(m_goalLeftNet);
    window.draw(m_goalRightNet);

    m_ball.draw(window);
    m_player1.draw(window);
    m_player2.draw(window);

    // Goal posts (in front of ball and players)
    window.draw(m_goalLeftCrossbar);
    window.draw(m_goalLeftPost);
    window.draw(m_goalRightCrossbar);
    window.draw(m_goalRightPost);


    // Score
    std::string score = std::to_string(m_score1) + " : " + std::to_string(m_score2);
    hudFont.draw(window, score, { Physics::WINDOW_WIDTH / 2.f - 20.f, 10.f });

}

std::optional<SceneTransition> GameScene::pollTransition()
{
    return std::exchange(requested, std::nullopt);
}

void GameScene::resolvePlayerPlayerCollision()
{
    sf::Vector2f p1Pos = m_player1.getPosition();
    sf::Vector2f p2Pos = m_player2.getPosition();
    float        r     = Player::RADIUS;

    sf::Vector2f normal;
    float        penetration;

    if (!PhysicsUtils::circleVsCircle(p1Pos, r, p2Pos, r, normal, penetration))
        return;

    // Push each player apart by half the penetration depth
    m_player1.setPosition(p1Pos + normal * (penetration * 0.5f));
    m_player2.setPosition(p2Pos - normal * (penetration * 0.5f));

    // Cancel velocity components pushing the two players into each other
    sf::Vector2f v1 = m_player1.getVelocity();
    sf::Vector2f v2 = m_player2.getVelocity();

    float v1n = PhysicsUtils::dot(v1, normal);
    float v2n = PhysicsUtils::dot(v2, normal);

    if (v1n - v2n < 0.f)   // players are moving toward each other
    {
        // Zero out the normal component so they slide along each other
        m_player1.setVelocity(v1 - normal * v1n);
        m_player2.setVelocity(v2 + normal * v2n);
    }
}

void GameScene::resolvePlayerBallCollision()
{
    for (Player* player : { &m_player1, &m_player2 })
    {
        sf::Vector2f bPos = m_ball.getPosition();
        sf::Vector2f pPos = player->getPosition();
        float        bR   = m_ball.getRadius();
        float        pR   = player->getRadius();

        sf::Vector2f normal;
        float        penetration;

        if (!PhysicsUtils::circleVsCircle(bPos, bR, pPos, pR, normal, penetration))
            continue;

        // Push ball out of player to prevent sticking
        m_ball.setPosition(bPos + normal * (penetration + 5.f));

        sf::Vector2f ballVel   = m_ball.getVelocity();
        sf::Vector2f playerVel = player->getVelocity();

        const bool isHeadHit = (normal.y < -0.3f);

        if (isHeadHit)
        {
            float horizKick = playerVel.x * Physics::HEAD_HORIZ_MUL
                + normal.x * Physics::HEAD_HORIZ_ADD;

            float incomingVy = std::abs(ballVel.y);
            float newVy = -(incomingVy * Physics::BALL_BOUNCE);
            if (std::abs(newVy) < Physics::MIN_BOUNCE_VY)
                newVy = 0.f;

            newVy = std::max(newVy, Physics::HEAD_BOUNCE_VY);

            m_ball.setVelocity({ horizKick, newVy });
            player->onHeadBall();
        }
        else
        {
            float velAlongNormal = PhysicsUtils::dot(ballVel, normal);
            if (velAlongNormal < 0.f)
            {
                constexpr float restitution = 0.6f;
                sf::Vector2f impulse = normal * (-(1.f + restitution) * velAlongNormal);
                m_ball.applyImpulse(impulse);
            }
        }
    }
}

// Checks if the player is trying to kick the ball and applies an appropriate launch velocity if so.
void GameScene::resolveKick()
{
    // { player ref, kick key, index into m_kickedThisPress }
    struct KickEntry { Player* player; bool keyHeld; int idx; };
    KickEntry entries[2] = {
        { &m_player1,
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z),
          0 },
        { &m_player2,
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Slash),
          1 },
    };

    constexpr float angleNear = 80.f;
    constexpr float angleFar  = 10.f;
    constexpr float speedNear = 520.f;
    constexpr float speedFar  = 360.f;

    for (auto& e : entries)
    {
        if (!e.keyHeld)
        {
            m_kickedThisPress[e.idx] = false;
            continue;
        }

        if (m_kickedThisPress[e.idx])
            continue;

        sf::FloatRect zone = e.player->getKickZone();
        sf::Vector2f  bPos = m_ball.getPosition();

        if (!zone.contains(bPos))
            continue;

        float zoneLeft  = zone.position.x;
        float zoneRight = zone.position.x + zone.size.x;

        // t: 0 = near the player's body, 1 = far end of kick zone
        float t = (e.idx == 0)
            ? std::clamp((bPos.x - zoneLeft)  / (zoneRight - zoneLeft), 0.f, 1.f)
            : std::clamp((zoneRight - bPos.x) / (zoneRight - zoneLeft), 0.f, 1.f);

        float angleDeg = angleNear + t * (angleFar - angleNear);
        float angleRad = angleDeg * (3.14159265f / 180.f);
        float speed    = (speedNear + t * (speedFar - speedNear)) * Physics::KICK_POWER;

        // Player 1 kicks right (+vx), Player 2 kicks left (-vx)
        float vx = (e.idx == 0 ? 1.f : -1.f) * speed * std::cos(angleRad);
        float vy = -speed * std::sin(angleRad);

        m_ball.setVelocity({ vx, vy });
        m_kickedThisPress[e.idx] = true;
    }
}

void GameScene::resolveGoalPostCollision()
{
    const float GH = Physics::GOAL_HEIGHT;
    const float GD = Physics::GOAL_DEPTH;
    const float GP = Physics::GOAL_POST;
    const float W  = Physics::WINDOW_WIDTH;
    const float GY = Physics::GROUND_Y;

    // Crossbars only (matches the visual shapes exactly)
    const sf::FloatRect posts[] = {
        { { 0.f,         GY - GH - GP }, { GD + GP, GP } },  // left crossbar
        { { W - GD - GP, GY - GH - GP }, { GD + GP, GP } },  // right crossbar
    };

    constexpr float restitution = 0.7f;

    for (const auto& rect : posts)
    {
        sf::Vector2f normal;
        float        penetration;

        if (!PhysicsUtils::circleVsAABB(m_ball.getPosition(), Ball::RADIUS, rect, normal, penetration))
            continue;

        // Push ball out of the post
        m_ball.setPosition(m_ball.getPosition() + normal * penetration);

        // Reflect velocity along the collision normal
        sf::Vector2f vel    = m_ball.getVelocity();
        float        velDot = PhysicsUtils::dot(vel, normal);
        if (velDot < 0.f)
            m_ball.setVelocity(vel - normal * ((1.f + restitution) * velDot));
    }
}

void GameScene::checkGoal()
{
    const sf::Vector2f bPos = m_ball.getPosition();
    const bool inGoalHeight  = (bPos.y >= Physics::GROUND_Y - Physics::GOAL_HEIGHT);

    const bool leftGoal  = inGoalHeight && (bPos.x < Physics::GOAL_DEPTH);
    const bool rightGoal = inGoalHeight && (bPos.x > Physics::WINDOW_WIDTH - Physics::GOAL_DEPTH);

    if (leftGoal)
    {
        ++m_score2;
        resetPositions();
    }
    else if (rightGoal)
    {
        ++m_score1;
        resetPositions();
    }
}

void GameScene::resetPositions()
{
    m_player1.setPosition({ Physics::WINDOW_WIDTH * 0.25f, Physics::GROUND_Y - Player::RADIUS });
    m_player1.setVelocity({ 0.f, 0.f });

    m_player2.setPosition({ Physics::WINDOW_WIDTH * 0.75f, Physics::GROUND_Y - Player::RADIUS });
    m_player2.setVelocity({ 0.f, 0.f });

    m_ball.setPosition({ Physics::WINDOW_WIDTH / 2.f, 200.f });
    m_ball.setVelocity({ 0.f, 0.f });

    m_kickedThisPress[0] = false;
    m_kickedThisPress[1] = false;
}