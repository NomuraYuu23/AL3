#include "EnemyBullet.h"
#include <cassert>
#include <cmath>

// 初期化
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red1x1.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	
	//Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	//横軸方向の長さを求める
	float length = Length(Vector3{velocity_.x, 0.0f, velocity_.z});
	//X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

}

// 更新
void EnemyBullet::Update() {

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	//行列を更新
	worldTransform_.UpdateMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

// 描画
void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
