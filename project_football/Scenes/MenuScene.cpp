#include "MenuScene.hpp"
#include <utility>

MenuScene::MenuScene()
{
	// HUD
    if (hudFont.loadFromFile("Assets/Fonts/BoldPixels.png", { 10, 16 }, 16))
    {
        hudFont.setScale(0.8f);
    }
    box.setSize({ 140.f, 60.f });
    box.setFillColor(sf::Color(140, 140, 140));
    box.setOrigin(box.getSize() * 0.5f);

    // Center-ish in virtual space
    box.setPosition({ kVirtualSizeF.x * 0.5f, kVirtualSizeF.y * 0.5f });
}

void MenuScene::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.getIf<sf::Event::KeyPressed>())
        requested = SceneTransition{ TransitionOp::Replace, SceneType::Loading };
}

void MenuScene::update(sf::Time) {}

void MenuScene::draw(sf::RenderWindow& window)
{
    window.draw(box);
    // HUD
    std::string t;
    t += "Main Menu Scene\n";

    hudFont.draw(window, t, { kVirtualSizeF.x * 0.5f - 60, kVirtualSizeF.y * 0.5f });
}

std::optional<SceneTransition> MenuScene::pollTransition()
{
    return std::exchange(requested, std::nullopt);
}
