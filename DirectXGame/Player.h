#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <memory>
#include <vector>

class Player {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(std::vector<Model*> models);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatinggimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatinggimmick();

	WorldTransform GetWorldTransform() { return worldTransform_; }

	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	//モデル
	std::vector<Model*> models_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<frame>
	uint16_t floatingPeriod = 1;
	// 浮遊の振幅<m>
	float floatingAmplitude = 0.0f;

};
