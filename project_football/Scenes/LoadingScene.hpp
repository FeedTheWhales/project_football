#pragma once
#include "../Core/Scene.hpp"
#include <optional>

class LoadingScene final : public Scene
{
public:
    LoadingScene();

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

    std::optional<SceneTransition> pollTransition() override;

private:
    sf::CircleShape circle;
    std::optional<SceneTransition> requested;
};
