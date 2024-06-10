#pragma once

#include <windows.h>

//VBO或EBO
class BufferObject {
private:
    size_t bufferSize_ = 0;
    byte* buffer_ = nullptr;
public:
    BufferObject() = default;
    ~BufferObject();
    BufferObject(const BufferObject&) = delete;
    BufferObject& operator=(const BufferObject&) = delete;

    void SetData(void* data, size_t size);
    byte* GetBuffer() const;
};