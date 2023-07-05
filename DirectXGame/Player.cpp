#include "Player.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model){

	//nullポインタチェック
	assert(model);

	model_ = model;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();


}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection);

}