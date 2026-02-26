#pragma once
#include "../Core/Scene.hpp"
#include "../Game/BitmapFont.hpp"
#include <optional>

#include "../Physics/Player.hpp"
#include "../Physics/Ball.hpp"


class GameScene final : public Scene
{
	//temporary rectangle to fill the screen
    sf::RectangleShape tempo;
public:
    GameScene();

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

    std::optional<SceneTransition> pollTransition() override;

private:
    std::optional<SceneTransition> requested;

    // --- UI ---
    BitmapFont hudFont;
private: 
    void resolvePlayerBallCollision();
    void resolveKick();

    bool  m_kickedThisPress = false;

    Player m_player;
    Ball   m_ball;

    sf::RectangleShape m_sky;
    sf::RectangleShape m_ground;
};

// 공차기 공식
//**How the arc math works : **
//
//The `t` value is a normalized 0–1 distance across the kick zone.It gets linearly mapped to an angle between 80° and 10°, then decomposed with `cos`/`sin` into `vx`/`vy`. At exactly `t = 0.5` the angle lands at 45°, which gives the maximum range — a natural sweet spot without any special - casing.
//```
//t = 0.0  → 80°  → mostly upward, speed 520
//t = 0.5  → 45°  → equal up + right, speed 440   ← sweet spot
//t = 1.0  → 10°  → mostly rightward, speed 360