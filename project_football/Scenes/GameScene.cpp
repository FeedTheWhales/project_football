#include "GameScene.hpp"
#include <utility>
#include <string>

GameScene::GameScene()
{
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

    
}

void GameScene::draw(sf::RenderWindow& window)
{
    window.draw(tempo);

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
