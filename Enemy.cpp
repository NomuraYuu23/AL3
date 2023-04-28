#include "Enemy.h"
#include <iostream>
#include <cassert>

// コンストラクタ
Enemy::Enemy() {

	state = new EnemyStateApproach();

}

// デストラクタ
Enemy::~Enemy() {

	if (state) {
		delete state;
	}

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

//ChangeState
void Enemy::ChangeState(BaseEnemyState* newState) {
	
	if (state) {
		delete state;
		state = newState;
	}
}

//EnemyStateApproach
void EnemyStateApproach::Update(Enemy* pEnemy) {

	// 移動(ベクトルを加算)
	pEnemy->SetVelocity(Vector3{0.0f, 0.0f, -0.1f});
	pEnemy->SetWorldTransformTranslation(
	    Add(pEnemy->GetWorldTransformTranslation(), pEnemy->GetVelocity())); 
	// 規定の位置に到達したら離脱
	if (pEnemy->GetWorldTransformTranslation().z < -15.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}

}

// EnemyStateLeave
void EnemyStateLeave::Update(Enemy* pEnemy) {

	pEnemy->SetVelocity(Vector3{-0.3f, 0.3f, 0.0f});
	pEnemy->SetWorldTransformTranslation(
	    Add(pEnemy->GetWorldTransformTranslation(), pEnemy->GetVelocity())); 

}