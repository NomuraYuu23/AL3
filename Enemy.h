#pragma once
#include "Model.h"
// #include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

#include "TimedCall.h"

#include "Collider.h"

// 自機クラスの前方宣言
class Player;
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

class Enemy : public Collider {

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
	//発射してリセット
	void FireAndReset();
	// 発射タイマーをセットする
	void SetFireTimer() { timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireAndReset, this), kFireInterval)); }


	Vector3 GetVelocity() { return velocity_; }
	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }

	Vector3 GetWorldTransformTranslation() { return worldTransform_.translation_; }
	void SetWorldTransformTranslation(Vector3 worldTransformTranslation) { worldTransform_.translation_ = worldTransformTranslation; }

	std::list<TimedCall*> GetTimedCalls() { return timedCalls_; }

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	// 発射感覚
	static const int kFireInterval = 60;
	// 半径
	static const int kRadius = 1;

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

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	//自キャラ
	Player* player_ = nullptr;


};
