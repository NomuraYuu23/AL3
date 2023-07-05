#pragma once
#include "Vector3.h"
#include <vector>
#include "ViewProjection.h"
class CatmullRomSplineCalc {

public:

	// 3次スプライン曲線
	static Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, const float& t);
	// 曲線描画テスト
	static void CatmullRomSplineDraw(
	    const std::vector<Vector3>& controlPoints, size_t segmentCount,
	    const ViewProjection* viewProjection);

};
