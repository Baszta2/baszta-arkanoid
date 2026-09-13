#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
// #include <SFML/Window/VideoMode.hpp>
#include <time.h>
#include <filesystem>
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    fs::path exePath = fs::absolute(argv[0]).parent_path();
    fs::path imagePath = exePath / "mahjong2.png";
    // create the window
    RenderWindow window(VideoMode({500, 500}), "Arkanoid");  
    Texture background;
    if (!background.loadFromFile(imagePath.string()))
    {
        return -1;
    }

    Sprite sprite(background);
    FloatRect spriteBounds = sprite.getLocalBounds();

    View baseView(
        { spriteBounds.size.x / 2.f, spriteBounds.size.y / 2.f },
        { spriteBounds.size.x, spriteBounds.size.y }
    );
    window.setView(baseView);


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* r = event->getIf<sf::Event::Resized>())
            {
                // Current window size
                unsigned int newW = r->size.x;
                unsigned int newH = r->size.y;

                float targetRatio = spriteBounds.size.x / spriteBounds.size.y;
                float currentRatio = static_cast<float>(newW) / static_cast<float>(newH);

                unsigned int enforcedW = newW;
                unsigned int enforcedH = newH;

                if (currentRatio > targetRatio)
                {
                    // Window too wide → adjust width
                    enforcedW = static_cast<unsigned int>(newH * targetRatio);
                }
                else if (currentRatio < targetRatio)
                {
                    // Window too tall → adjust height
                    enforcedH = static_cast<unsigned int>(newW / targetRatio);
                }

                // Apply corrected window size
                window.setSize({ enforcedW, enforcedH });

                // Pixel-perfect view
                sf::View v;
                v.setCenter({ enforcedW * 0.5f, enforcedH * 0.5f });
                v.setSize({ static_cast<float>(enforcedW), static_cast<float>(enforcedH) });
                window.setView(v);

                // Scale sprite to fill window exactly
                sprite.setScale({
                    enforcedW / spriteBounds.size.x,
                    enforcedH / spriteBounds.size.y
                });
                sprite.setPosition({0.f, 0.f});
            }

        }

        // clear the window with black color
        window.clear();

        // draw everything here...
        // window.draw(...);
        window.draw(sprite);

        // end the current frame
        window.display();
    }
    return 0;
}
