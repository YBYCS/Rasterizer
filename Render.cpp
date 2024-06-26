#include "Render.h"
#include "Rasterizer.h"
#include <iostream>
#include "WindowController.h"

BaseShader* Render::curShader_ = nullptr;
Matrix4 Render::screenMatrix_ = ScreenMatrix(window->GetWidth(), window->GetHeight());
bool Render::enableBlending_ = false;
float* Render::depthMap_ = new float[window->GetWidth() * window->GetHeight()];

VertexData Render::VertexShader(const Vector3 &position, const Color &color, const Vector3 &normal, const Vector2 &texCoord)
{
    VertexData vertex;
    vertex.position = Vector4(position.x, position.y, position.z, 1.0f);
    vertex.color = ColorToVector4(color);
    vertex.normal = normal;
    vertex.texCoord = texCoord;

    return curShader_->VertexShader(vertex);
}

/// @brief 剔除NDC空间下x、y、z的值小于0或大于1的点
/// @param vertexData 
void Trim(VertexData &vertexData) {
    if (vertexData.position.x < -1.0f) {
        vertexData.position.x = -1.0f;
    }

    if (vertexData.position.x > 1.0f) {
        vertexData.position.x = 1.0f;
    }

    if (vertexData.position.y < -1.0f) {
        vertexData.position.y = -1.0f;
    }

    if (vertexData.position.y > 1.0f) {
        vertexData.position.y = 1.0f;
    }

    if (vertexData.position.z < -1.0f) {
        vertexData.position.z = -1.0f;
    }

    if (vertexData.position.z > 1.0f) {
        vertexData.position.z = 1.0f;
    }
}
void Render::PerspectiveDivision(std::vector<VertexData> &output)
{
    //不仅要对position除以Position.w，对所有需要插值的属性都要除以Position.w以应用透视修正
    //这个Position.w要记录下来，用于透视恢复
    for (auto &vertexData : output) {
        vertexData.oneOverW = 1.0f / vertexData.position.w;
        vertexData.position *= vertexData.oneOverW;
        vertexData.position.w = 1.0f;   //Position.w要恢复为1
        vertexData.color *= vertexData.oneOverW;
        vertexData.normal *= vertexData.oneOverW;
        vertexData.texCoord *= vertexData.oneOverW;

        //透视除法完毕后，需要确保所有的物体都落在 -1到1的NDC空间下
        Trim(vertexData);
    }
}

void Render::PerspectiveCorrection(std::vector<VertexData> &output)
{
    //恢复顶点数据
    for (auto &vertexData : output) {
        vertexData.color /= vertexData.oneOverW;
        vertexData.texCoord /= vertexData.oneOverW;
        vertexData.normal /= vertexData.oneOverW;
    }
}

void Render::ViewPortTransform(std::vector<VertexData> &output)
{
    for (auto &vertexData : output) {
        vertexData.position = screenMatrix_ * vertexData.position;
    }
}

bool Render::DepthTest(const FragmentShaderOutput &output)
{
    auto position = output.position;
    if (position.x == window->GetWidth() || position.y == window->GetHeight()) {
        return false;
    }
    uint32_t index = position.y * window->GetWidth() + position.x;
    float oldDepth = depthMap_[index];
    if (output.depth < oldDepth) {
        depthMap_[index] = output.depth;
        return true;
    }
    return false;
}

bool Render::BackfaceCulling(const VertexData &v0, const VertexData &v1, const VertexData &v2)
{
    //此时处于NDC空间下，NDC坐标系是左手坐标系！
    //因此背面剔除就是剔除掉法线的z值大于0的的三角形
    auto p1 = v1.position - v0.position;
    Vector3 edge1(p1.x, p1.y);
    auto p2 = v2.position - v0.position;
    Vector3 edge2(p2.x, p2.y);

    return Cross(edge1, edge2).z > 0;
}

void Render::EarClipping(std::vector<VertexData> &res, Model *model, const Face &targetFace)
{
    //这里使用的耳剪法是最简单的版本，时间复杂度为n³，有更好的时间复杂度为n²的版本
    //但是考虑到多边形面的顶点不会很多，故只采用了最简单的版本。（才不是因为懒

    //判断一个顶点是否为耳剪法定义的“凸点”
    auto is_convex = [](const Vector3& p, const Vector3& a, const Vector3& b) -> bool {
        Vector3 ab = b - a;
        Vector3 ap = p - a;
        Vector3 crossProduct = Cross(ab, ap);
        return crossProduct.z >= 0;
    };

    //判断一个顶点是否在三角形内部
    auto is_inside = [](const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c) -> bool {
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        Vector3 ap = p - a;

        // 计算面积
        float abcArea = 0.5f * Cross(ab, ac).Length();
        float abpArea = 0.5f * Cross(ab, ap).Length();
        float acpArea = 0.5f * Cross(ac, ap).Length();
        float bcpArea = 0.5f * Cross((p - b), (c - b)).Length();

        //如果三个小三角形的面积之和等于大三角形的面积，则点在三角形内部
        return std::abs(abpArea + acpArea + bcpArea - abcArea) < 1e-6;
    };

    //多边形索引
    std::vector<int> indices = targetFace.vertexIndices;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    
    size_t n = indices.size();

    // 准备一个多边形顶点序列
    std::vector<Vector3> polygon;
    //注意：这里GPT说obj文件会保证多边形顶点按照顺序存储，如果出现bug可以查看是否是这个原因
    for (size_t i = 0; i < n; ++i) {
        polygon.push_back(model->vertices[indices[i]]);
        normals.push_back(model->normals[targetFace.normalIndices[i]]);
        uvs.push_back(model->uvs[targetFace.uvIndices[i]]);
    }

    // 三角化过程
    while (n > 3) {
        for (size_t i = 0; i < n; ++i) {
            size_t prev = (i == 0) ? n - 1 : i - 1;
            size_t next = (i + 1) % n;

            Vector3 cur = polygon[i];
            Vector3 pre = polygon[prev];
            Vector3 nxt = polygon[next];

            // 判断是否是凸顶点
            if (is_convex(cur, pre, nxt)) {
                bool isEar = true;
                for (size_t j = 0; j < n; ++j) {
                    if (j == prev || j == i || j == next) continue;
                    if (is_inside(polygon[j], pre, cur, nxt)) {
                        isEar = false;
                        break;
                    }
                }

                // 如果是耳朵，移除并添加三角形
                if (isEar) {
                    res.emplace_back(VertexShader(pre, Color(), normals[prev], uvs[prev]));
                    res.emplace_back(VertexShader(cur, Color(), normals[i], uvs[i]));
                    res.emplace_back(VertexShader(nxt, Color(), normals[next], uvs[next]));

                    polygon.erase(polygon.begin() + i);
                    normals.erase(normals.begin() + i);
                    uvs.erase(uvs.begin() + i);
                    --n;
                    break;
                }
            }
        }
    }

    //剩下的最后一个三角形
    if (n == 3) {
        //由于读取的是obj文件，暂时没有考虑颜色
        res.emplace_back(VertexShader(polygon[0], Color(), normals[0], uvs[0]));
        res.emplace_back(VertexShader(polygon[1], Color(), normals[1], uvs[1]));
        res.emplace_back(VertexShader(polygon[2], Color(), normals[2], uvs[2]));
    }
}

void Render::ClipInClipSpace(const DrawMode &drawMode, const std::vector<VertexData> &input, std::vector<VertexData> &output)
{
    //分为对三角形进行剪裁和对直线进行剪裁
    if (drawMode == DrawMode::TRIANGLE) {
        //从input中取出一个三角形的三个顶点，放在这个preTriangle集合里
        std::vector<VertexData> preTriangle;
        //经过剪裁算法后的一个三角形的三个顶点，放在这个res集合里
        std::vector<VertexData> res;

        for (uint32_t i = 0; i < input.size(); i += 3) {
            preTriangle.clear();
            res.clear();
            preTriangle.assign(input.begin() + i, input.begin() + i + 3);
            //剪裁一个三角形
            Sutherland_Hodgman(drawMode, preTriangle, res);

            if (res.empty())
                continue;
            //三角形进行裁剪完后，可能会变成一个四边形
            //这个循环用于将剪裁后的多边形拆分成若干三角形（剪裁完后是三角形的仍然适用）
            for (uint32_t c = 0; c < res.size() - 2; ++c) {
                output.push_back(res[0]);
                output.push_back(res[c + 1]);
                output.push_back(res[c + 2]);
            }
        }
    } 
    else if (drawMode == DrawMode::LINE) {
        //从input中取出一个线段的两个顶点，放在这个preLine集合里
        std::vector<VertexData> preLine;
        //经过剪裁算法后的一个线段的两个顶点，放在这个res集合里
        std::vector<VertexData> res;
        for (uint32_t i = 0; i < preLine.size(); i += 2) {
            preLine.clear();
            res.clear();
            preLine.assign(input.begin() + i, input.begin() + i + 2);
            Sutherland_Hodgman(drawMode, preLine, res);

            output.push_back(res[0]);
            output.push_back(res[1]);
        }
    }
}

void Render:: Sutherland_Hodgman(const DrawMode &drawMode, const std::vector<VertexData> &input, std::vector<VertexData> &output)
{
    // Sutherland_Hodgman 裁剪算法在注释里三言两语讲不清除，可以去飞书看看具体原理

    //判断一个点是否在一个平面的可见的那一侧
    auto inside = [](const Vector4 &point, const Vector4 &plane) -> bool {
	    return Dot(point, plane) >= 0.0f;
    };

    //当一个线段的两个点分别处于检测平面的两侧时，求出这个线段与检测平面的交点
    auto intersect = [](const VertexData &currentPoint, const VertexData &nextPoint, const Vector4 &clipPlane) -> VertexData {
        VertexData res;

        float distanceCurrent = Dot(currentPoint.position, clipPlane);
        float distanceNext = Dot(nextPoint.position, clipPlane);
        float weight = distanceCurrent / (distanceCurrent - distanceNext);

        res.position = currentPoint.position * (1 - weight) + nextPoint.position * weight;
        res.color = currentPoint.color * (1 - weight) + nextPoint.color * weight;
        res.texCoord = currentPoint.texCoord * (1 - weight) + nextPoint.texCoord * weight;
        res.normal = currentPoint.normal * (1 - weight) + nextPoint.normal * weight;
        res.oneOverW = currentPoint.oneOverW * (1 - weight) + nextPoint.oneOverW * weight;

        return res;
    };

    //剪裁平面
    std::vector<Vector4> clipPlanes = {
        //剪裁空间下的w分量要大于0
        Vector4(0.0f, 0.0f, 0.0f, 1.0f),
        //近平面
        Vector4(0.0f, 0.0f, 1.0f, 1.0f),
        //远平面
        Vector4(0.0f, 0.0f, -1.0f, 1.0f),
        //左平面
        Vector4(1.0f, 0.0f, 0.0f, 1.0f),
        //右平面
        Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
        //上平面
        Vector4(0.0f, -1.0f, 0.0f, 1.0f),
        //下平面
        Vector4(0.0f, 1.0f, 0.0f, 1.0f),
    };
        
    output = input;
    std::vector<VertexData> temp;

    //依次用六个平面裁剪
    for (int i = 0; i < clipPlanes.size(); i++) {
        //用上一次的输出点集作为这一次的输入点集
        temp = output;
        output.clear();

        for (int j = 0; j < temp.size(); j++) {
            VertexData currentPoint = temp[j];
            VertexData nextPoint = temp[(j + 1) % temp.size()];

            //当前点在检测平面的内部（即可见侧）
            if (inside(currentPoint.position, clipPlanes[i])) {
                //当前点只要在内部，都会输出
                output.emplace_back(currentPoint);
                if (drawMode != DrawMode::LINE || j != temp.size() - 1) {
                    //下一个点不在内部，输出交点
                    if (!inside(nextPoint.position, clipPlanes[i])) {
                        //求交点
                        auto intersectPoint = intersect(currentPoint, nextPoint, clipPlanes[i]);
                        output.emplace_back(intersectPoint);
                    }
                    //下一个点在内部时，到它的回合它自己会输出它自己
                } 
            }
            else {
                if (drawMode != DrawMode::LINE || j != temp.size() - 1) {
                    //当前点不在内部且下一个点在内部,输出交点
                    if (inside(nextPoint.position, clipPlanes[i])) {
                        auto intersectPoint = intersect(currentPoint, nextPoint, clipPlanes[i]);
                        output.emplace_back(intersectPoint);
                    }
                    //两个点都不在内部时，啥都不输出
                }
            }
        }
    }
}

/// @brief 颜色混合
/// @param output 
/// @return 
Color Render::BlendColor(const FragmentShaderOutput &output)
{
    Color res;
    Color preColor = window->GetColor(output.position.x, output.position.y);
    Color curColor = output.color;

    float weight = (float)curColor.a / 255.0f;
    res.r = (float)curColor.r * weight + (float)preColor.r * (1.0f - weight);
    res.g = (float)curColor.g * weight + (float)preColor.g * (1.0f - weight);
    res.b = (float)curColor.b * weight + (float)preColor.b * (1.0f - weight);
    res.a = (float)curColor.a * weight + (float)preColor.a * (1.0f - weight);

    return res;
}

void Render::SetBlending(bool enabled)
{
    enableBlending_ = enabled;
}

bool Render::IsBlendingEnabled()
{
    return enableBlending_;
}

void Render::SetShader(BaseShader *shader)
{
    curShader_ = shader;
}

void Render::InitializeDepthMap(float value)
{
    std::fill_n(depthMap_, window->GetWidth() * window->GetHeight(), value);
}

void Render::RenderModel(Model *model, Image *textureImage)
{
    if (curShader_ == nullptr) {
        std::__throw_runtime_error("shader is null !");
    }

    //首先将模型的所有顶点经过 VertexShader 处理后，装入到一个vector中
    //在这一步就已经进行完了MVP变换
    std::vector<VertexData> vertexesData;
    std::vector<int> vertexesIndex;
    int i1 = 0, i2 = 0;
    for (uint32_t i = 0; i < model->faces.size(); i++) {
        auto face = model->faces[i];
        //如果一个面的顶点个数超过三个，使用耳剪法进行切分
        int n = face.vertexIndices.size();
        if (n > 3) {
            EarClipping(vertexesData, model, face);
        } else {
            for (int j = 0; j < 3; j++) {
                int vertexIndex = face.vertexIndices[j];
                Vector3 position = model->vertices[face.vertexIndices[j]];
                Vector3 normal = model->normals[face.normalIndices[j]];
                Vector2 uv = model->uvs[face.uvIndices[j]];
                //obj文件只用纹理获取颜色，默认底色为白色
                vertexesData.emplace_back(VertexShader(position, Color(), normal, uv));
            }
        }
    }
    
    if (vertexesData.empty()) {
        std::__throw_runtime_error("model is null !");
    }

    //裁剪阶段，在剪裁空间下进行
    std::vector<VertexData> clipOutputs;
    ClipInClipSpace(DrawMode::TRIANGLE, vertexesData, clipOutputs);
    if (clipOutputs.empty()) return;
    vertexesData.clear();

    //将裁剪处理后的顶点数据，从剪裁空间转换到NDC空间
    PerspectiveDivision(clipOutputs);

    //背面剔除
    std::vector<VertexData> cullOutputs;
    for (uint32_t i = 0; i < clipOutputs.size() - 2; i += 3) {
        if (BackfaceCulling(clipOutputs[i], clipOutputs[i + 1], clipOutputs[i + 2])) {
            cullOutputs.insert(cullOutputs.end(), clipOutputs.begin() + i, clipOutputs.begin() + i + 3);
        }
    }

    //应用视口变换映射到屏幕空间
    //需要注意的是，这里从宽高从[-1, 1]变换到了[0, width]或[0, height]
    //但是在设置点的颜色位置和深度图中的位置范围是[0, width - 1]和[0, height - 1]
    //因此在画点和深度测试时，需要剔除掉这两个边界条件
    ViewPortTransform(cullOutputs);

    //光栅化处理
    std::vector<VertexData> rasterOutputs = Rasterizer::Rasterize(DrawMode::TRIANGLE, cullOutputs, textureImage);
    if (rasterOutputs.empty())
        return;
    
    //恢复透视修正
    PerspectiveCorrection(rasterOutputs);

    //应用 FragmentShader 
    FragmentShaderOutput fsOutput;
    for (uint32_t i = 0; i < rasterOutputs.size(); ++i) {
        curShader_->FragmentShader(rasterOutputs[i], fsOutput, textureImage);
        //深度测试
        if (!DepthTest(fsOutput)) 
            continue;
        window->DrawPoint(fsOutput.position.x, fsOutput.position.y, enableBlending_ ? BlendColor(fsOutput) : fsOutput.color);
    }
}