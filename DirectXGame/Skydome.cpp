#include "Skydome.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
void Skydome::Initialize(Model* model) {

	// nullポインタチェック
	assert(model);

	model_ = model;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update() {

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Skydome::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection); 
}
