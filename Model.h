//
// Created by azuslai on 21/5/2024.
//

#ifndef RASTERIZER_MODEL_H
#define RASTERIZER_MODEL_H
#include <vector>
#include "string"
#include "Vector.h"

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;
};

class Model {
public:
    std::vector<Vector3> vertices;
    std::vector<Vector2> uvs;
    std::vector<Vector3> normals;
    std::vector<Face> faces;
    static bool LoadOBJ(const std::string &filename, Model &model);
};


#endif //RASTERIZER_MODEL_H
