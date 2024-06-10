#include "GPU.h"
#include <cassert>

GPU::~GPU()
{
    for (auto& iter : bufferMap_) {
        delete iter.second;
    }
    bufferMap_.clear();

    for (auto& iter : vaoMap_) {
        delete iter.second;
    }
    vaoMap_.clear();
}

GPU &GPU::GetInstance()
{
    static GPU instance;
    return instance;
}

uint32_t GPU::GenerateBufferObject()
{
    bufferCounter_++;
    bufferMap_[bufferCounter_] = new BufferObject();
    return bufferCounter_;
}

void GPU::DeleteBufferObject(const uint32_t &bufferId)
{
    auto iterator = bufferMap_.find(bufferId);
    if (iterator != bufferMap_.end()) {
        delete iterator->second;
        bufferMap_.erase(iterator);
    }
}

uint32_t GPU::GenerateVao()
{
    vaoCounter_++;
    vaoMap_[vaoCounter_] = new VertexArrayObject();
    return vaoCounter_;
}

void GPU::DeleteVao(const uint32_t &vaoId)
{
    auto iterator = vaoMap_.find(vaoId);
    if (iterator != vaoMap_.end()) {
        delete iterator->second;
        vaoMap_.erase(iterator);
    }
}

void GPU::BindBufferObject(const uint32_t &bufferType, const uint32_t &bufferId)
{
    switch (bufferType)
    {
    case ARRAY_BUFFER:
        currentVboId_ = bufferId;
        break;
    case ELEMENT_ARRAY_BUFFER:
        currentEboId_ = bufferId;
        break;
    default:
        break;
    }
}

void GPU::SetBufferObjectData(const uint32_t &bufferType, void *data, size_t dataSize)
{
    uint32_t bufferId = (bufferType == BufferType::ARRAY_BUFFER) ? currentVboId_ : currentEboId_;
    auto iterator = bufferMap_.find(bufferId);
    if (iterator == bufferMap_.end()) {
        throw std::runtime_error("未找到缓冲区ID");
    }
    iterator->second->SetData(data, dataSize);
}

void GPU::UnBindBufferObject(const uint32_t &bufferType)
{
    BindBufferObject(bufferType, 0);
}

void GPU::BindVao(const uint32_t &vaoId)
{
    currentVaoId_ = vaoId;
}

//仿照OpenGL流程，在调用此函数前需要绑定VBO
void GPU::SetCurrentVaoData(uint32_t bindingId, size_t size, size_t stride, size_t offset)
{
    auto iterator = vaoMap_.find(currentVaoId_);
    if (iterator == vaoMap_.end()) {
        assert(false);
    }
    iterator->second->SetAttribute(bindingId, currentVboId_, size, stride, offset);
}

void GPU::UnBindVao()
{
    BindVao(0);
}
