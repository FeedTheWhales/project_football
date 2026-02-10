#pragma once
#include "../Core/Scene.hpp"
#include "../Game/BitmapFont.hpp"
#include <optional>

class PauseScene final : public Scene
{
public:
    PauseScene();

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

    std::optional<SceneTransition> pollTransition() override;

private:
    sf::RectangleShape overlay;
    sf::CircleShape dot;
    float bobTime = 0.f;
    std::optional<SceneTransition> requested;

    // --- UI ---
    BitmapFont hudFont;
};
