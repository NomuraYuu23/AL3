#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"
#include <vector>
#include "ViewProjection.h"

// 自分で追加した分

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ（ノルム）
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);

// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
// 球面線形補間
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

// 加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 座標変換
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);



// 3次スプライン曲線
Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, const float& t);
// 曲線描画テスト
void CatmullRomSplineDraw(
    const std::vector<Vector3>& controlPoints, size_t segmentCount,
    const ViewProjection* viewProjection);


// 正射影行列
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);
