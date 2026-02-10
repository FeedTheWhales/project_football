#include "SceneFactory.hpp"

#include <cassert>

#include "../Scenes/MenuScene.hpp"
#include "../Scenes/LoadingScene.hpp"
#include "../Scenes/GameScene.hpp"
#include "../Scenes/PauseScene.hpp"

std::unique_ptr<Scene> createScene(SceneType type)
{
    switch (type)
    {
    case SceneType::Menu:    return std::make_unique<MenuScene>();
    case SceneType::Loading: return std::make_unique<LoadingScene>();
    case SceneType::Game:    return std::make_unique<GameScene>();
    case SceneType::Pause:   return std::make_unique<PauseScene>();
    }

    assert(false && "Unhandled SceneType in createScene()");
    return nullptr;
}
