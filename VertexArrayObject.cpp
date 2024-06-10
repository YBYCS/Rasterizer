#include "VertexArrayObject.h"
#include <iostream>

void VertexArrayObject::SetAttribute(uint32_t bindingId, uint32_t vboId, size_t size, size_t stride, size_t offset)
{
    auto iterator = bindingMap_.find(bindingId);
    if (iterator == bindingMap_.end()) {
        iterator = bindingMap_.insert(std::make_pair(bindingId, VertexAttributeDescriptor())).first;
    }

    auto& res = iterator->second;
    res.vboId = vboId;
    res.size = size;
    res.stride = stride;
    res.offset = offset;
}

const std::unordered_map<uint32_t, VertexAttributeDescriptor> &VertexArrayObject::GetBindingMap() const
{
    return bindingMap_;
}