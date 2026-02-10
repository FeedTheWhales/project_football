#include "PauseScene.hpp"
#include <cmath>
#include <utility>


PauseScene::PauseScene()
{
    // HUD
    if (hudFont.loadFromFile("Assets/Fonts/BoldPixels.png", { 10, 16 }, 16))
    {
        hudFont.setScale(0.8f);
    }

    // Semi-transparent dark overlay
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    overlay.setPosition({ 0.f, 0.f });
    overlay.setSize(kVirtualSizeF); // IMPORTANT: virtual size, not window size

    dot.setRadius(10.f);
    dot.setFillColor(sf::Color::Yellow);
    dot.setOrigin({ dot.getRadius(), dot.getRadius() });

    // Place dot relative to virtual screen
    dot.setPosition({ kVirtualSizeF.x * 0.5f, kVirtualSizeF.y * 0.5f });
}

void PauseScene::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Escape)
            requested = SceneTransition{ TransitionOp::Pop, SceneType::Game };
    }
}

void PauseScene::update(sf::Time dt)
{
    bobTime += dt.asSeconds();
    const float baseY = kVirtualSizeF.y * 0.5f;
    const float y = baseY + 6.f * std::sin(bobTime * 3.0f);
    dot.setPosition({ kVirtualSizeF.x * 0.5f, y });
}

void PauseScene::draw(sf::RenderWindow& window)
{
    // No window-size math here. The view handles scaling/letterboxing.
    window.draw(overlay);
    window.draw(dot);

    // HUD
    std::string t;
    t += "Pause Scene\n";

    hudFont.draw(window, t, { kVirtualSizeF.x * 0.5f - 45, kVirtualSizeF.y * 0.5f - 200 });
}

std::optional<SceneTransition> PauseScene::pollTransition()
{
    return std::exchange(requested, std::nullopt);
}
