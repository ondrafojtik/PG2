#include "RandomMap.h"

static float real_y_position(float y)
{
    float value = y;
    y = y * SCALE_Y;
    y = y + MAP_HEIGHT;

    return y;
}

// returns 0 if theres no such point
static bool find_from_coords(std::vector<Point> points, Point point_to_test)
{
    for (Point p : points)
        if (p.x == point_to_test.x && p.z == point_to_test.z)
            return 1;

    return 0;
}

static Point find_point(std::vector<Point> points, float x, float z)
{
    for (Point p : points)
        if (p.x == x && p.z == z)
            return p;
    std::cout << "(Method: find_point - RandomMap.h)Couldnt find point with given coords" << std::endl;
}

// standart inverse lerp..
static float inverse_lerp(float value, float min, float max)
{
    float val = (value - min) / (max - min);
    return val;
}

void RandomMap::Init()
{
    std::ifstream file("src/vendor/pi.txt");
    std::getline(file, seed_origin);
    file.close();

    layout.Push<float>(3);
    layout.Push<float>(3);

    scale = glm::vec3(20.0f, SCALE_Y, 20.0f);
    Recalc();

}

void RandomMap::Recalc()
{
    //clear
    buffer.erase(buffer.begin(), buffer.begin() + buffer.size());
    vertices.erase(vertices.begin(), vertices.begin() + vertices.size());
    indices.erase(indices.begin(), indices.begin() + indices.size());
    seed = 0;
    step = 0;


    srand((unsigned)std::time(NULL));
    float random = (float)rand() / RAND_MAX;
    random = random - Random::Float();
    random = glm::abs(random);
    seed = random * seed_origin.size();
    std::cout << "seed: " << seed << std::endl;

    float difference = 0.5f;

    unsigned int index0;
    unsigned int index1;
    unsigned int index2;
    unsigned int index3;


    for (int z = 0; z < Z_DIMENSION; z++)
        for (int x = 0; x < X_DIMENSION; x++)
        {
            if (seed + step > seed_origin.size())
                seed = 0;

            // do the 4 points
            //  *--*
            //  |  |
            //  *--*
            // increase value, so that I get random value for each
            // point(vertex)


            float y = (char)seed_origin[seed + step] - '0';
            Point point = Point(x - difference, real_y_position(y), z - difference);
            if (!find_from_coords(buffer, point))
            {
                point.index = step;
                buffer.push_back(point);
                index0 = step;
                step += 1;
            }
            else
            {
                index0 = find_point(buffer, point.x, point.z).index;
            }

            y = (char)seed_origin[seed + step] - '0';
            point = Point(x - difference, real_y_position(y), z + difference);
            if (!find_from_coords(buffer, point))
            {
                point.index = step;
                buffer.push_back(point);
                index1 = step;
                step += 1;
            }
            else
            {
                index1 = find_point(buffer, point.x, point.z).index;
            }

            y = (char)seed_origin[seed + step] - '0';
            point = Point(x + difference, real_y_position(y), z + difference);
            if (!find_from_coords(buffer, point))
            {
                point.index = step;
                buffer.push_back(point);
                index2 = step;
                step += 1;
            }
            else
            {
                index2 = find_point(buffer, point.x, point.z).index;
            }

            y = (char)seed_origin[seed + step] - '0';
            point = Point(x + difference, real_y_position(y), z - difference);
            if (!find_from_coords(buffer, point))
            {
                point.index = step;
                buffer.push_back(point);
                index3 = step;
                step += 1;
            }
            else
            {
                index3 = find_point(buffer, point.x, point.z).index;
            }

            // add normal triangles here to render and do the "hard ones" later? 
            indices.push_back(index0);
            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index2);
            indices.push_back(index3);
            indices.push_back(index0);

            // calculate normal for each vertex

            // first triangle (normals)
            glm::vec3 v1 = { buffer.at(index2).x - buffer.at(index1).x,
                             buffer.at(index2).y - buffer.at(index1).y,
                             buffer.at(index2).z - buffer.at(index1).z };
            glm::vec3 v2 = { buffer.at(index0).x - buffer.at(index1).x,
                             buffer.at(index0).y - buffer.at(index1).y,
                             buffer.at(index0).z - buffer.at(index1).z };
            v1 = glm::normalize(v1);
            v2 = glm::normalize(v2);
            glm::vec3 dot_product = glm::cross(v1, v2);
            buffer.at(index0).normals.push_back(dot_product);
            buffer.at(index1).normals.push_back(dot_product);
            buffer.at(index2).normals.push_back(dot_product);

            // second triangle (normals)
            v1 = { buffer.at(index0).x - buffer.at(index3).x,
                   buffer.at(index0).y - buffer.at(index3).y,
                   buffer.at(index0).z - buffer.at(index3).z };
            v2 = { buffer.at(index2).x - buffer.at(index3).x,
                   buffer.at(index2).y - buffer.at(index3).y,
                   buffer.at(index2).z - buffer.at(index3).z };
            v1 = glm::normalize(v1);
            v2 = glm::normalize(v2);
            dot_product = glm::cross(v1, v2);
            buffer.at(index2).normals.push_back(dot_product);
            buffer.at(index3).normals.push_back(dot_product);
            buffer.at(index0).normals.push_back(dot_product);

        }
    // find maximum values (so that u can transform all vertex info into values
    // from 0 to 1
    float min_x = 0 - difference;
    float max_x = X_DIMENSION - difference;
    float min_z = 0 - difference;
    float max_z = Z_DIMENSION - difference;
    float min_y = real_y_position(0);
    float max_y = real_y_position(9);

    //fill the "vertices" with bufferData

    for (Point p : buffer)
    {
        VertexInfo vertex;
        vertex.position.x = inverse_lerp(p.x, min_x, max_x) * 2.0f - 1.0f;
        vertex.position.y = inverse_lerp(p.y, min_y, max_y) * 2.0f - 1.0f;
        vertex.position.z = inverse_lerp(p.z, min_z, max_z) * 2.0f - 1.0f;

        // calculate average normal
        glm::vec3 sum = glm::vec3(0, 0, 0);
        for (glm::vec3 normal : p.normals)
            sum += normal;

        sum = glm::normalize(sum);
        vertex.normal.x = sum.x;
        vertex.normal.y = sum.y;
        vertex.normal.z = sum.z;

        vertices.push_back(vertex);
    }
    // creating render data
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    vb = new VertexBuffer(&vertices[0], vertices.size() * sizeof(VertexInfo));

    va = new VertexArray();
    va->AddBuffer(*vb, layout);
    ib = new IndexBuffer(&indices[0], indices.size());

}
