#include "PlayerBullet.h"
#include <cassert>
#include "Enemy.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

}

void PlayerBullet::Update() {

	if (enemy_) {

		// ホーミング
		// 敵キャラのワールド座標を取得する
		Vector3 enemyPos = enemy_->GetWorldPosition();
		// 自弾のワールド座標を取得する
		Vector3 playerBulletPos = GetWorldPosition();
		// 敵弾->自キャラの差分ベクトルを求める
		Vector3 toEnemy = Subtract(enemyPos, playerBulletPos);
		// ベクトルの正規化
		toEnemy = Normalize(toEnemy);
		velocity_ = Normalize(velocity_);
		// 1フレームでの補間割合t
		float t = 0.025f;
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		// 球面線形補間により、今の速度と自キャラのベクトルを内挿し、新たな速度とする
		velocity_ = Multiply(kBulletSpeed, Slerp(velocity_, toEnemy, t));
		// 進行方向に見た目の回転を合わせる
		//  Y軸周りの角度(Θy)
		worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
		// 横軸方向の長さを求める
		float length = Length(Vector3{velocity_.x, 0.0f, velocity_.z});
		// X軸周りの角度(Θx)
		worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

	}

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 行列を更新
	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}


// ワールド座標を取得
Vector3 PlayerBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 衝突を検出したら呼び出されるコールバック関数
void PlayerBullet::OnCollision() {

	isDead_ = true;
	enemy_ = nullptr;

}
