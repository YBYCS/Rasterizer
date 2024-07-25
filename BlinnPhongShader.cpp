#include "BlinnPhongShader.h"
#include "Rasterizer.h"

VertexData BlinnPhongShader::VertexShader(const VertexData &input)
{
    Vector4 curPosition = projectionMatrix_ * viewMatrix_ * modelMatrix_ * input.position;
    // 变换法线向量
    Vector4 curNormal(input.normal.x, input.normal.y, input.normal.z, 0.0f);
    curNormal = Transpose(Inverse(modelMatrix_)) * curNormal; // 计算模型矩阵的逆转置矩阵
    Normalize(curNormal);
    return VertexData(curPosition, input.color, Vector3(curNormal.x, curNormal.y, curNormal.z), input.texCoord);
}

bool BlinnPhongShader::FragmentShader(const VertexData &input, FragmentShaderOutput &output, const Image *image)
{
    //再次归一化法线
    Vector3 normal = Normalize(input.normal);
    //光照方向
    Vector3 lightDir = Normalize(directionalLight.direction);
    //应用光照前，物体原本的颜色
    Vector4 originalColor =  input.color;
    //计算漫反射
    float diff = std::max(Dot(normal, -lightDir), 0.0f);
    Vector4 diffuse = originalColor * diff * Vector4(directionalLight.color.x, directionalLight.color.y, directionalLight.color.z, 1.0f) * directionalLight.intensity;
    //计算环境光
    Vector4 ambient = originalColor * Vector4(ambientLight.color.x, ambientLight.color.y, ambientLight.color.z, 1.0f) * ambientLight.intensity;
    //计算镜面反射
    //观察方向
    auto normalViewDir = Normalize(input.position);
    Vector3 viewDir(normalViewDir.x, normalViewDir.y, normalViewDir.z);
    //半程向量
    Vector3 halfVector = Normalize(lightDir + viewDir);
    float spec = std::max(Dot(normal, halfVector), 0.0f);
    spec = pow(spec, 128.0f); // 高光强度和指数
    Vector4 specular = originalColor * Vector4(directionalLight.color.x, directionalLight.color.y, directionalLight.color.z, 1.0f) * directionalLight.intensity * spec;

    //计算最终颜色
    output.color = Vector4ToColor(diffuse + ambient + specular);
    output.position = Vector2((int)input.position.x, (int)input.position.y);
    output.depth = input.position.z;
    return true;
}