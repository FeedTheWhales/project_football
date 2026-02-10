#pragma once
#include "Scene.hpp"
#include <memory>

std::unique_ptr<Scene> createScene(SceneType type);
