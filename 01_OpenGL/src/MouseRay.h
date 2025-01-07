#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Camera.h"
#include "math.h"

#include "glm/ext/matrix_projection.hpp"

static glm::vec3 normalize(glm::vec3 vec)
{
    float XY = sqrt((vec.x * vec.x) + (vec.y * vec.y));
    float final = sqrt((XY * XY) + (vec.z * vec.z));
    return glm::vec3(vec.x / final, vec.y / final, vec.z / final);
};

struct MouseRay
{
    glm::vec3 originPoint;
    glm::vec3 destPoint;

    glm::vec3 get_normalized_ray() { return normalize(destPoint - originPoint); }
    
    Camera* camera = nullptr;
    MouseRay();

    void init();
    
    void set_mouse_position(glm::dvec2 pos);
    

    void update_camera(Camera* camera) { this->camera = camera; }

    void update();
    
private:
    glm::vec2 mousePos; 
    glm::vec3 mouseRay; // this is the final product after all conversions

    glm::mat4 view;
    glm::mat4 projection;
    
    


};
