#pragma once

#include "Texture.h"
#include "glm/vec2.hpp"

struct FontSheet
{
    Texture* texture = nullptr;

    FontSheet(const char* file_path)
    {
        texture = new Texture(file_path);
    }

    glm::vec2* get_coords(char32_t c)
    {
        glm::vec2 sprite_size = glm::vec2(32.0f, 32.0f);

        int32_t x = c - ' ';

        // magic is used for "dealing with weird black lines" when rendering font. My file isnt really well done, so there's always some part of like "next char in the line".. this way,
        // I can make chars "less wide" and therefore deal with the weird black 10;
        float _magic = 0.999f;

        glm::vec2* coords = new glm::vec2[4];
        coords[0] = { (x * sprite_size.x) / (texture->GetWidth()), 0.0f };
        coords[1] = { ((x + 1) * sprite_size.x) / (texture->GetWidth()) * _magic, 0.0f };
        coords[2] = { ((x + 1) * sprite_size.x) / (texture->GetWidth()) * _magic, 1.0f };
        coords[3] = { (x * sprite_size.x) / (texture->GetWidth()), 1.0f };

        return coords;
    }


};
