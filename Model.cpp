//
// Created by azuslai on 21/5/2024.
//

#include "Model.h"
#include "fstream"
#include "iostream"
#include "sstream"
#include "Point.h"

bool Model::LoadOBJ(const std::string &filename, Model &model)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v") {
            Vector3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            model.vertices.push_back(vertex);
        } else if (prefix == "vt") {
            Vector2 uv;
            lineStream >> uv.x >> uv.y;
            model.uvs.push_back(uv);
        } else if (prefix == "vn") {
            Vector3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            model.normals.push_back(normal);
        } else if (prefix == "f") {
            Face face;
            std::string vertexData;
            while (lineStream >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string vertexIndex, uvIndex, normalIndex;
                std::getline(vertexStream, vertexIndex, '/');
                std::getline(vertexStream, uvIndex, '/');
                std::getline(vertexStream, normalIndex, '/');

                face.vertexIndices.push_back(std::stoi(vertexIndex) - 1);
                if (!uvIndex.empty()) {
                    face.uvIndices.push_back(std::stoi(uvIndex) - 1);
                }
                if (!normalIndex.empty()) {
                    face.normalIndices.push_back(std::stoi(normalIndex) - 1);
                }
            }
            model.faces.push_back(face);
        }
    }

    file.close();
    return true;
}
