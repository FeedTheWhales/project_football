#pragma once

#include <SFML/Graphics.hpp>
#include "Physics.hpp"

class Player
{
public:
    static constexpr float RADIUS = 24.f;

    Player(float startX, float startY);

    // Call every frame before update()
    void handleInput(sf::Time dt);

    // Integrate physics, resolve ground collision
    void update(sf::Time dt);

    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_vel; }
    float        getRadius()   const { return RADIUS; }

	// 공과 충돌했을 때 호출 (공이 플레이어의 머리를 맞았는지 여부는 임시로 GameScene에서 판단)
    // 최종적으로는 Player에서 구현해야 할 듯
    void onHeadBall() {}

	// Returns the rectangle in which the player can kick the ball with their foot.
    sf::FloatRect getKickZone() const;

private:
    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    bool         m_onGround = false;

    // SFML 3: CircleShape constructor requires the radius
    sf::CircleShape m_body;
    sf::CircleShape m_eyeL;
    sf::CircleShape m_eyeR;
};