#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "VirtualResolution.hpp"

enum class SceneType
{
    Menu,
    Loading,
    Game,
    Pause
};

enum class TransitionOp
{
    Replace, // replace the top scene
    Push,    // push a new scene on top (overlay)
    Pop      // pop the top scene
};

struct SceneTransition
{
    TransitionOp op = TransitionOp::Replace;
    SceneType next = SceneType::Loading; // used for Replace / Push (ignored for Pop)
};

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void handleEvent(sf::RenderWindow& window, const sf::Event& event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual std::optional<SceneTransition> pollTransition() = 0;
};
