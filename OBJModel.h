//
// Created by azuslai on 21/5/2024.
//

#ifndef RASTERIZER_OBJMODEL_H
#define RASTERIZER_OBJMODEL_H
#include <vector>
#include "string"
struct Vec3 {
    float x, y, z;
};

struct Vec2 {
    float u, v;
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;
};
class OBJModel {
public:
    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
    std::vector<Face> faces;
    static bool loadOBJ(const std::string &filename, OBJModel &model);
};


#endif //RASTERIZER_OBJMODEL_H
