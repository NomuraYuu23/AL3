#pragma once
#include "Model.h"
// #include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

//前方宣言
class Enemy;

class BaseEnemyState {
public:
	BaseEnemyState() {}
	virtual~BaseEnemyState() {}
	virtual void Initialize(Enemy* pEnemy) = 0;
	virtual void Update(Enemy* pEnemy) = 0;

};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void Update(Enemy* pEnemy);
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void Update(Enemy* pEnemy);
};

class Enemy {

	public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);
	//ChangeState
	void ChangeState(BaseEnemyState* newState);
	//弾発射
	void Fire();

	Vector3 GetVelocity() { return velocity_; }
	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }

	Vector3 GetWorldTransformTranslation() { return worldTransform_.translation_; }
	void SetWorldTransformTranslation(Vector3 worldTransformTranslation) { worldTransform_.translation_ = worldTransformTranslation; }

	int32_t GetFiringTimer() { return firingTimer_; }
	void SetFiringTimer(int32_t firingTimer) { firingTimer_ = firingTimer; }

	// 発射感覚
	static const int kFireInterval = 60;

	private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	BaseEnemyState* state;

	Vector3 velocity_ = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

	//弾
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t firingTimer_ = 0;


};
