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

};
