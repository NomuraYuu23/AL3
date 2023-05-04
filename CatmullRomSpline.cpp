#include "Vector3.h"
#include "AL3Math.h"
#include <cmath>
#include <vector>
#include "PrimitiveDrawer.h"

// 3次スプライン曲線
Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, const float& t) {

	Vector3 result;

	uint32_t point = int(float(controlPoints.size() - 1) * t);
	float t_ = float(controlPoints.size() - 1) * t - float(point);
	if (point == controlPoints.size() - 1) {
		point = int(float(controlPoints.size() - 1));
		t_ = 1.0f;
	}

	Vector3 p0, p1, p2, p3;

	if (point <= 0) {
		
		p0 = controlPoints[0];
		p1 = controlPoints[0];
		p2 = controlPoints[1];
		p3 = controlPoints[2];

	} else if (point >= controlPoints.size() - 2) {
	
		p0 = controlPoints[controlPoints.size() - 3];
		p1 = controlPoints[controlPoints.size() - 2];
		p2 = controlPoints[controlPoints.size() - 1];
		p3 = controlPoints[controlPoints.size() - 1];

	} else {

		p0 = controlPoints[--point];
		p1 = controlPoints[++point];
		p2 = controlPoints[++point];
		p3 = controlPoints[++point];

	
	}

		result.x = 1.0f / 2.0f *
	           ((-1.0f * p0.x + 3.0f * p1.x + -3.0f * p2.x + p3.x) * std::powf(t_, 3.0f) +
	            (2.0f * p0.x + -5.0f * p1.x + 4.0f * p2.x + -1.0f * p3.x) * std::powf(t_, 2.0f) +
	            (-1.0f * p0.x + p2.x) * t_ + 2.0f * p1.x);

		result.y = 1.0f / 2.0f *
	        ((-1.0f * p0.y + 3.0f * p1.y + -3.0f * p2.y + p3.y) * std::powf(t_, 3.0f) +
	         (2.0f * p0.y + -5.0f * p1.y + 4.0f * p2.y + -1.0f * p3.y) * std::powf(t_, 2.0f) +
	         (-1.0f * p0.y + p2.y) * t_ + 2.0f * p1.y);

		result.z = 1.0f / 2.0f *
	        ((-1.0f * p0.z + 3.0f * p1.z + -3.0f * p2.z + p3.z) * std::powf(t_, 3.0f) +
	         (2.0f * p0.z + -5.0f * p1.z + 4.0f * p2.z + -1.0f * p3.z) * std::powf(t_, 2.0f) +
	         (-1.0f * p0.z + p2.z) * t_ + 2.0f * p1.z);

		return result;

}

// 曲線描画テスト
void CatmullRomSplineDraw(const std::vector<Vector3>& controlPoints, size_t segmentCount,const ViewProjection* viewProjection) {

	//線分で描画する用の頂点リスト
	std::vector<Vector3> pointDrawing;
	//線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomSpline(controlPoints, t);
		//描画リストに追加
		pointDrawing.push_back(pos);
	}

	PrimitiveDrawer::GetInstance()->SetViewProjection(viewProjection);


	for (std::vector<Vector3>::iterator itr = pointDrawing.begin(); itr < pointDrawing.end() - 1;
	     ++itr) {

		PrimitiveDrawer::GetInstance()->DrawLine3d(*itr, *(itr + 1), {1.0f, 0.0f, 0.0f, 1.0f});

	}

}
