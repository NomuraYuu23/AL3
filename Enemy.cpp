#include "Enemy.h"
#include <iostream>
#include <cassert>

// コンストラクタ
Enemy::Enemy() {

	state = new EnemyStateApproach();

}

// デストラクタ
Enemy::~Enemy() {

	delete state;

}

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

	state->Update(this);

	worldTransform_.UpdateMatrix();

}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

//EnemyStateApproach

void EnemyStateApproach::Update(Enemy* pEnemy) {



}
