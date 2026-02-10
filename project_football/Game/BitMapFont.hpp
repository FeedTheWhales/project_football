#pragma once
#include <SFML/Graphics.hpp>
#include <string_view>

class BitmapFont
{
public:
    // grid font atlas: characters 32..126 laid out left-to-right, top-to-bottom
    bool loadFromFile(const std::string& pngPath, sf::Vector2i glyphSize, int columns = 16)
    {
        if (!texture.loadFromFile(pngPath))
            return false;

        texture.setSmooth(false); // <-- nearest neighbor
        glyph = glyphSize;
        cols = columns;
        return true;
    }

    void setScale(float s) { scale = s; }
    float getScale() const { return scale; }

    // Draw text at position in WORLD (virtual) coords.
    void draw(sf::RenderTarget& target, std::string_view text, sf::Vector2f pos) const
    {
        sf::Sprite spr(texture);
        spr.setScale({ scale, scale });

        sf::Vector2f pen = pos;

        for (char c : text)
        {
            if (c == '\n')
            {
                pen.x = pos.x;
                pen.y += glyph.y * scale;
                continue;
            }

            const unsigned char uc = static_cast<unsigned char>(c);
            if (uc < 32 || uc > 126)
            {
                pen.x += glyph.x * scale;
                continue;
            }

            const int index = (int)uc - 32;
            const int gx = (index % cols) * glyph.x;
            const int gy = (index / cols) * glyph.y;

            spr.setTextureRect(sf::IntRect({ gx, gy }, { glyph.x, glyph.y }));
            spr.setPosition(pen);
			spr.setColor(sf::Color::White);
            target.draw(spr);

            pen.x += glyph.x * scale;
        }
    }

private:
    sf::Texture texture;
    sf::Vector2i glyph{ 8, 8 };
    int cols = 16;
    float scale = 1.f;
};
