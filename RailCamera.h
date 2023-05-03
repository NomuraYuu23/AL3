#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "AL3Math.h"

class RailCamera {
public:
	//初期化
	void Initialize(const Vector3& position, const Vector3& rotation);

	//更新
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//---ビュープロジェクション---//
	ViewProjection viewProjection_;

	// 移動速度
	Vector3 translationVelocity = {0.0f,0.0f,0.1f};
	// 回転速度
	Vector3 rotationVelocity = {0.0f, 0.0f, 0.1f};

};
