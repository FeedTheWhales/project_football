#pragma once
#include "../Core/Scene.hpp"
#include "../Game/BitmapFont.hpp"
#include <optional>

class MenuScene final : public Scene
{
public:
    MenuScene();

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

    std::optional<SceneTransition> pollTransition() override;

private:
    sf::RectangleShape box;
    std::optional<SceneTransition> requested;
    // --- UI ---
    BitmapFont hudFont;
};
