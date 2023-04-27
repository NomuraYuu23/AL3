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

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		//接近フェーズ
		Approach();
		break;
	case Enemy::Phase::Leave:
		// 離脱フェーズ
		Leave();
		break;
	}

	worldTransform_.UpdateMatrix();

}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

// 接近フェーズ
void Enemy::Approach() {

	// 移動(ベクトルを加算)
	velocity.z = -0.2f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < -9.0f) {
		phase_ = Phase::Leave;
	}

}

// 離脱フェーズ
void Enemy::Leave() {
	
	// 移動(ベクトルを加算)
	velocity.x = -0.3f;
	velocity.y = 0.3f;
	velocity.z = 0.0f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

}
