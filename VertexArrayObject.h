#pragma once 

#include "General.h"
#include <unordered_map>

//VAO
class VertexArrayObject {
private:
    std::unordered_map<uint32_t, VertexAttributeDescriptor> bindingMap_;
public:
    VertexArrayObject() = default;
    ~VertexArrayObject() = default;
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;

    void SetAttribute(uint32_t bindingId, uint32_t vboId, size_t size, size_t stride, size_t offset);
    const std::unordered_map<uint32_t, VertexAttributeDescriptor>& GetBindingMap() const;
    void Print();
};