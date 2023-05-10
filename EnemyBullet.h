#pragma once
#include "Model.h"
#include "WorldTransform.h"

#include "Collider.h"

// 自機クラスの前方宣言
class Player;

class EnemyBullet : public Collider {
public:
	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	// 自キャラ
	Player* player_ = nullptr;

};
