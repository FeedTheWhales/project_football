#include "LoadingScene.hpp"
#include <utility>

LoadingScene::LoadingScene()
{
    circle.setRadius(80.0f);
    circle.setFillColor(sf::Color::Blue);
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });

    // Center in virtual space
    circle.setPosition({ kVirtualSizeF.x * 0.5f, kVirtualSizeF.y * 0.5f });
}

void LoadingScene::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.getIf<sf::Event::KeyPressed>())
        requested = SceneTransition{ TransitionOp::Replace, SceneType::Game };
}

void LoadingScene::update(sf::Time dt)
{
    constexpr float rotationSpeedDegPerSec = 60.f;
    const float deg = rotationSpeedDegPerSec * dt.asSeconds();
    circle.rotate(sf::degrees(deg));
}

void LoadingScene::draw(sf::RenderWindow& window)
{
    window.draw(circle);
}

std::optional<SceneTransition> LoadingScene::pollTransition()
{
    return std::exchange(requested, std::nullopt);
}
