#include <SFML/Graphics.hpp>
#include <algorithm>
#include <optional>
#include <vector>
#include <memory>

#include "Core/SceneFactory.hpp"
#include "Core/VirtualResolution.hpp"

static sf::View makeLetterboxView(sf::Vector2u windowSize, sf::Vector2f virtualSize)
{
    sf::View view(sf::FloatRect({ 0.f, 0.f }, virtualSize));

    const float windowRatio = (float)windowSize.x / (float)windowSize.y;
    const float virtualRatio = virtualSize.x / virtualSize.y;

    float sizeX = 1.f;
    float sizeY = 1.f;
    float posX = 0.f;
    float posY = 0.f;

    if (windowRatio > virtualRatio)
    {
        // Window is wider -> pillarbox (bars left/right)
        sizeX = virtualRatio / windowRatio;
        posX = (1.f - sizeX) * 0.5f;
    }
    else if (windowRatio < virtualRatio)
    {
        // Window is taller -> letterbox (bars top/bottom)
        sizeY = windowRatio / virtualRatio;
        posY = (1.f - sizeY) * 0.5f;
    }

    view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
    return view;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Project Capture");
    window.setKeyRepeatEnabled(false);
    window.setView(makeLetterboxView(window.getSize(), kVirtualSizeF));

    std::vector<std::unique_ptr<Scene>> scenes;
    scenes.push_back(createScene(SceneType::Menu));

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }
            if (const auto* r = event->getIf<sf::Event::Resized>())
            {
                window.setView(makeLetterboxView({ r->size.x, r->size.y }, kVirtualSizeF));
            }

            // Only the top scene gets input
            scenes.back()->handleEvent(window, *event);
        }

        sf::Time dt = clock.restart();
        dt = std::min(dt, sf::seconds(1.f / 20.f)); // clamp big spikes

        // Only the top scene updates (so GameScene freezes when Pause is on top)
        scenes.back()->update(dt);

        if (auto tr = scenes.back()->pollTransition())
        {
            switch (tr->op)
            {
            case TransitionOp::Replace:
                scenes.back() = createScene(tr->next);
                break;

            case TransitionOp::Push:
                scenes.push_back(createScene(tr->next));
                break;

            case TransitionOp::Pop:
                if (scenes.size() > 1)
                    scenes.pop_back();
                break;
            }
        }

        window.clear(sf::Color::Black);

        // Draw from bottom to top so overlays work (Pause drawn over Game)
        for (auto& s : scenes)
            s->draw(window);

        window.display();
    }

    return 0;
}
