#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "AL3Math.h"
#include <vector>

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

	//ワールド行列を取得
	const WorldTransform& GetWorldMatrix() { return worldTransform_; }

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//---ビュープロジェクション---//
	ViewProjection viewProjection_;

	// 移動速度
	Vector3 translationVelocity = {0.0f,0.0f,0.1f};
	// 回転速度
	Vector3 rotationVelocity = {0.0f, 0.0f, 0.0f};

	//視点
	Vector3 eye = {0.0f, 0.0f, 0.0f};
	//注視点
	Vector3 target = {0.0f, 0.0f, 0.0f};
	//進んだ距離(割合)
	float t;
	// 速度(割合)
	float velocity;

	float targetDistance = 60.0f;


};
