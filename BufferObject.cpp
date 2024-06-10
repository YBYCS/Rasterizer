#include "BufferObject.h"

BufferObject::~BufferObject()
{
    if (buffer_) {
        delete[] buffer_;
    }
}

void BufferObject::SetData(void *data, size_t size)
{
    if (buffer_ && bufferSize_ < size) {
        delete[] buffer_;
        buffer_ = nullptr;
    }

    if (!buffer_) {
        buffer_ = new byte[size];
    }

    memcpy(buffer_, data, size);
}

byte *BufferObject::GetBuffer() const
{
    return buffer_;
}
