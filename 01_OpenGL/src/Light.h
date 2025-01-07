#include "glm/vec3.hpp"
#include "Object.h"

enum LightType
{
    Point = 0,
    Directional,
    Spotlight
};

struct Light
{
    LightType type = LightType::Point;
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 color = glm::vec3(1.0f);

    // for directional
    glm::vec3 lightDir = glm::vec3(1.0f, 0.0f, 0.0f);

    // for spotlight
    float cutoff = 0.0f;

};
