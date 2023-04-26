#include "Enemy.h"
#include <cassert>

	// 初期化
void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();


}

// 更新
void Enemy::Update() {

	const Vector3 velocity = {
	    0.0f,
	    0.0f,
	    -0.2f,
	};

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

	worldTransform_.UpdateMatrix();

}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
