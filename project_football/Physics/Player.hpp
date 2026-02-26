#pragma once

#include <SFML/Graphics.hpp>
#include "Physics.hpp"

class Player
{
public:
    static constexpr float RADIUS = 24.f;

    // playerNum: 1 = left side (blue, WASD), 2 = right side (red, arrow keys)
    explicit Player(int playerNum);

    Player(float startX, float startY);

    // Call every frame before update()
    void handleInput(sf::Time dt);

    // Integrate physics, resolve ground collision
    void update(sf::Time dt);

    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_vel; }
    float        getRadius()   const { return RADIUS; }

    void setPosition(sf::Vector2f pos) { m_pos = pos; }
    void setVelocity(sf::Vector2f vel) { m_vel = vel; }

	// 공과 충돌했을 때 호출 (공이 플레이어의 머리를 맞았는지 여부는 임시로 GameScene에서 판단)
    // 최종적으로는 Player에서 구현해야 할 듯
    void onHeadBall() {}

	// Returns the rectangle in which the player can kick the ball with their foot.
    sf::FloatRect getKickZone() const;

private:
    int          m_playerNum = 1;
    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    bool         m_onGround = false;

    // SFML 3: CircleShape constructor requires the radius
    sf::CircleShape m_body;
    sf::CircleShape m_eyeL;
    sf::CircleShape m_eyeR;
};