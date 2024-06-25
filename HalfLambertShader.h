#include "Vector.h"
#include "BaseShader.h"

/// @brief 平行光
struct DirectionalLight {
    Vector3 direction; //光源方向
    Vector3 color;     //光源颜色
    float intensity;   //光照强度
};
/// @brief 环境光
struct AmbientLight {
    Vector3 color;     //光源颜色
    float intensity;   //光照强度
};

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