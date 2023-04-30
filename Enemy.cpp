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
	//bullets_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}


}

// 初期化
void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//初期座標
	worldTransform_.translation_ = {10.0f, 0.0f, 5.0f};

}

// 更新
void Enemy::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		
		}
		return false;
		});

	//行動
	state->Update(this);

	worldTransform_.UpdateMatrix();

	//攻撃処理

	//弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

//ChangeState
void Enemy::ChangeState(BaseEnemyState* newState) {
	
	if (state) {
		delete state;
		state = newState;
		state->Initialize(this);
	}
}

// 弾発射
void Enemy::Fire() {

	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(newBullet);

}

//EnemyStateApproach

void EnemyStateApproach::Initialize(Enemy* pEnemy) {

	pEnemy->SetVelocity(Vector3{0.0f, 0.0f, -0.1f});
	//発射タイマーを初期化
	pEnemy->SetFiringTimer(pEnemy->kFireInterval);

}

void EnemyStateApproach::Update(Enemy* pEnemy) {

	// 移動(ベクトルを加算)
	pEnemy->SetWorldTransformTranslation(
	    Add(pEnemy->GetWorldTransformTranslation(), pEnemy->GetVelocity())); 
	// 規定の位置に到達したら離脱
	if (pEnemy->GetWorldTransformTranslation().z < -15.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}

	//発射タイマーカウントダウン
	pEnemy->SetFiringTimer(pEnemy->GetFiringTimer() - 1);
	//指定時間に達した
	if (pEnemy->GetFiringTimer() <= 0) {
		//弾を発射
		pEnemy->Fire();
		//発射タイマーを初期化
		pEnemy->SetFiringTimer(pEnemy->kFireInterval);
	}

}

// EnemyStateLeave

void EnemyStateLeave::Initialize(Enemy* pEnemy) {
	
	pEnemy->SetVelocity(Vector3{-0.3f, 0.3f, 0.0f});

}

void EnemyStateLeave::Update(Enemy* pEnemy) {

	pEnemy->SetWorldTransformTranslation(
	    Add(pEnemy->GetWorldTransformTranslation(), pEnemy->GetVelocity())); 

}