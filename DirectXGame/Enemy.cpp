#include "Player.h"
#include <cassert>
#include "input.h"
#include <Xinput.h>
#include <cmath>
#include <numbers>
#include "ImGuiManager.h"


#include "Vector3Calc.h"
#include "Matrix4x4Calc.h"

#include "Enemy.h"
/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Enemy::Initialize(const std::vector<Model*>& models) {

	// nullポインタチェック
	assert(models.front());

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.x -= 2.0f;
	worldTransformL_arm_.translation_.y += 1.0f;
	worldTransformL_arm_.rotation_.x += float(std::numbers::pi) / 2.0f;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.x += 2.0f;
	worldTransformR_arm_.translation_.y += 1.0f;
	worldTransformR_arm_.rotation_.x += float(std::numbers::pi) / 2.0f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	//回転
	Rotation();

	//移動
	Move();

	//ImGui
	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmL Rotation", &worldTransformL_arm_.rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Rotation", &worldTransformR_arm_.rotation_.x, -10.0f, 10.0f);
	ImGui::End();

	//ワールド変換データ更新
	worldTransform_.UpdateMatrix();

	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();



}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	 models_[0]->Draw(worldTransformBody_, viewProjection);
	 models_[1]->Draw(worldTransformL_arm_, viewProjection);
	 models_[2]->Draw(worldTransformR_arm_, viewProjection);

}

/// <summary>
/// 移動
/// </summary>
void Enemy::Move() {

	//移動速度
	const float kMoveSpeed = 0.3f;
	Vector3 velocity(0.0f,0.0f,kMoveSpeed);

	//速度ベクトルを向きに合わせて回転させる
	Velocity_ = Matrix4x4Calc::TransformNormal(velocity, worldTransform_.matWorld_);

	//移動
	worldTransform_.translation_.x += Velocity_.x;
	worldTransform_.translation_.y += Velocity_.y;
	worldTransform_.translation_.z += Velocity_.z;

}

/// <summary>
/// 回転
/// </summary>
void Enemy::Rotation() {

	//回転速度
	const float kRotateSpeed = 0.02f;

	worldTransform_.rotation_.y += kRotateSpeed;
	if (worldTransform_.rotation_.y >= 2.0f * float(std::numbers::pi)) {
		worldTransform_.rotation_.y -= 2.0f * float(std::numbers::pi);
	}

}
