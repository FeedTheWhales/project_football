#pragma once
#include "../Core/Scene.hpp"
#include "../Game/BitmapFont.hpp"
#include <optional>


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
};
