#include "Vector.h"
#include "BaseShader.h"

/// @brief 此Shader应用半兰伯特模型。
class HalfLambertShader : public BaseShader {
public:
    HalfLambertShader() = default;
    virtual ~HalfLambertShader() = default;

    VertexData VertexShader(const VertexData &input) override;
    bool FragmentShader(const VertexData& input, FragmentShaderOutput &output, const Image *image = nullptr) override;
};