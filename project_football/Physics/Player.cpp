#include "Player.hpp"
#include "Physics.hpp"

Player::Player(float startX, float startY)
    : m_pos(startX, startY)
    , m_vel(0.f, 0.f)
    , m_body(RADIUS)          
    , m_eyeL(5.f)
    , m_eyeR(5.f)
{
    // --- Body ---
    m_body.setOrigin({ RADIUS, RADIUS });                          // sf::Vector2f
    m_body.setFillColor(sf::Color(70, 130, 220));
    m_body.setOutlineThickness(2.f);
    m_body.setOutlineColor(sf::Color(30, 60, 160));

    // --- Eyes ---
    m_eyeL.setOrigin({ 5.f, 5.f });
    m_eyeL.setFillColor(sf::Color::White);

    m_eyeR.setOrigin({ 5.f, 5.f });
    m_eyeR.setFillColor(sf::Color::White);
}

void Player::handleInput(sf::Time dt)
{
    float moveX = 0.f;
	// Check for left/right movement keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        moveX -= Physics::PLAYER_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        moveX += Physics::PLAYER_SPEED;

    m_vel.x = moveX;

    if (m_onGround)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            m_vel.y = Physics::PLAYER_JUMP;
            m_onGround = false;
        }
    }

    (void)dt;
}

void Player::update(sf::Time dt)
{
    const float s = dt.asSeconds();
    if (!m_onGround)
        m_vel.y += Physics::GRAVITY * s;

    m_pos += m_vel * s;

    // Ground collision
    const float groundTop = Physics::GROUND_Y - RADIUS;
    if (m_pos.y >= groundTop)
    {
        m_pos.y = groundTop;
        m_vel.y = 0.f;
        m_onGround = true;
    }
    else
    {
        m_onGround = false;
    }

    // Horizontal screen clamping
    if (m_pos.x < RADIUS)
        m_pos.x = RADIUS;
    if (m_pos.x > Physics::WINDOW_WIDTH - RADIUS)
        m_pos.x = Physics::WINDOW_WIDTH - RADIUS;
}

void Player::draw(sf::RenderWindow& window) const
{
    sf::CircleShape body = m_body;
    sf::CircleShape eyeL = m_eyeL;
    sf::CircleShape eyeR = m_eyeR;

    body.setPosition(m_pos);
    eyeL.setPosition(m_pos + sf::Vector2f{ -9.f, -8.f });
    eyeR.setPosition(m_pos + sf::Vector2f{ 9.f, -8.f });

    window.draw(body);
    window.draw(eyeL);
    window.draw(eyeR);

    // Draw kick zone
    sf::FloatRect zone = getKickZone();
    sf::RectangleShape zoneRect(zone.size);
    zoneRect.setPosition(zone.position);
    zoneRect.setFillColor(sf::Color(255, 255, 0, 50));       // transparent yellow fill
    zoneRect.setOutlineThickness(1.f);
    zoneRect.setOutlineColor(sf::Color(255, 255, 0, 180));   // brighter yellow outline
    window.draw(zoneRect);
}

// Returns a rectangle in front of the player where they can kick the ball with their foot.
sf::FloatRect Player::getKickZone() const
{
    // A rectangular zone to the right of the player.
    // Width defines max kick range; height is forgiving vertically.
    constexpr float zoneWidth = 55.f;
    constexpr float zoneHeight = 40.f;

    return sf::FloatRect(
        { m_pos.x + RADIUS,                    // starts at the player's right edge
         m_pos.y - zoneHeight / 2.f },         // vertically centered on player
        { zoneWidth, zoneHeight }
    );
}