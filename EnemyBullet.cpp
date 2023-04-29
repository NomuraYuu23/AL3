#include "EnemyBullet.h"
#include <cassert>

// 初期化
void EnemyBullet::Initialize(Model* model, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red1x1x.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}

// 更新
void EnemyBullet::Update() {

	worldTransform_.UpdateMatrix();

}

// 描画
void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
