#pragma once
#include <unordered_map>
#include "BufferObject.h"
#include "VertexArrayObject.h"

#define gpu GPU::GetInstance()

enum BufferType {
	ARRAY_BUFFER,	//VBO
	ELEMENT_ARRAY_BUFFER,	//EBO
};

class GPU {
private:
	static GPU* instance_;

	uint32_t currentVboId_ = 0;
	uint32_t currentEboId_ = 0;
	uint32_t bufferCounter_ = 0;
	//存储VBO和EBO的Map
	std::unordered_map<uint32_t, BufferObject*> bufferMap_;

	uint32_t currentVaoId_ = 0;
	uint32_t vaoCounter_ = 0;
	//存储VAO的Map
	std::unordered_map<uint32_t, VertexArrayObject*> vaoMap_;

	GPU() = default;
public:
	GPU(const GPU&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU(GPU&&) = delete;
	~GPU();

	static GPU &GetInstance();
	//生成一个VBO或EBO
	uint32_t GenerateBufferObject();
	//删除指定ID的VBO或EBO
	void DeleteBufferObject(const uint32_t& bufferId);
	//生成一个VAO
	uint32_t GenerateVao();
	//删除指定ID的VAO
	void DeleteVao(const uint32_t& vaoId);
	//绑定VBO或EBO
	void BindBufferObject(const uint32_t& bufferType, const uint32_t& bufferId);
	//设置VBO或EBO数据
	void SetBufferObjectData(const uint32_t& bufferType, void* data, size_t dataSize);
	//解绑VBO或EBO
	void UnBindBufferObject(const uint32_t& bufferType);
	//绑定VAO
	void BindVao(const uint32_t& vaoId);
	//设置当前VAO数据
	void SetCurrentVaoData(uint32_t bindingId, size_t size, size_t stride, size_t offset);
	//解绑VAO
	void UnBindVao();
};