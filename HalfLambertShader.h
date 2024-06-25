#include "Vector.h"
#include "BaseShader.h"

/// @brief 此Shader应用半兰伯特模型。为了简单起见，光照暂时就在这里存储
class HalfLambertShader : public BaseShader {
public:
    DirectionalLight directionalLight;
    AmbientLight ambientLight;   //环境光
public:
    HalfLambertShader() = default;
    virtual ~HalfLambertShader() = default;

    VertexData VertexShader(const VertexData &input) override;
    bool FragmentShader(const VertexData& input, FragmentShaderOutput &output, const Image *image = nullptr) override;
};