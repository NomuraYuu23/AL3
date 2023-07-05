#pragma once
#include "Matrix4x4.h"

class RenderingPipeline {

public:

	// 正射影行列
	static Matrix4x4
	    MakeOrthographicMatrix(
	        float left, float top, float right, float bottom, float nearClip, float farClip);

	// 透視投影行列
	static Matrix4x4
	    MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	// ビューポート変換行列
	static Matrix4x4 MakeViewportMatrix(
	    float left, float top, float width, float height, float minDepth, float maxDepth);

};
