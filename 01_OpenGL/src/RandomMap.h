#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>

#include "glm/gtx/compatibility.hpp"

#include "Random.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Object.h"

#define MAP_HEIGHT 0
#define X_DIMENSION 10
#define Z_DIMENSION 10

#define SCALE_Y 1.5

struct Point
{
    float x, y, z;
    unsigned int index;

    std::vector<glm::vec3> normals;
    
    Point(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z) { normals = std::vector<glm::vec3>(); } 
};

struct VertexInfo
{
    glm::vec3 position;
    glm::vec3 normal;
    
};

struct RandomMap 
{
    
    int seed = 0;
    int step = 0;
    glm::vec3 scale;

    std::vector<Point> buffer;
    std::vector<VertexInfo> vertices;
    std::vector<unsigned int> indices;
    std::string seed_origin;

    // render data
    unsigned int vao;
    VertexBuffer* vb = nullptr;
    VertexBufferLayout layout;
    VertexArray* va = nullptr;
    IndexBuffer* ib = nullptr;

    void Init();

    void Recalc();
    
};
