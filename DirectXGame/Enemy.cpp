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

	// 移動用
	// 速度
	Velocity_ = {0.0f, 0.0f, 0.0f};
	// 速さ
	kMoveSpeed = 0.0f;

	// 回転用
	// 回転速度
	kRotateSpeed = 0.0f;

	// 腕回転ギミック初期化
	InitializeArmRotationgimmick();

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	//ImGui
	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmL Rotation", &worldTransformL_arm_.rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Rotation", &worldTransformR_arm_.rotation_.x, -10.0f, 10.0f);
	ImGui::End();

	// 回転
	Rotation();

	// 移動
	Move();

	// 腕回転ギミック
	UpdateArmRotationgimmick();

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

	// ImGui
	 ImGui::Begin("Enemy");
	 ImGui::SliderFloat("kMoveSpeed", &kMoveSpeed, 0.0f, 1.0f);
	 ImGui::End();

	//移動速度
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

	// ImGui
	ImGui::Begin("Enemy");
	ImGui::SliderFloat("kRotateSpeed", &kRotateSpeed, 0.0f, 0.1f);
	ImGui::End();

	worldTransform_.rotation_.y += kRotateSpeed;
	if (worldTransform_.rotation_.y >= 2.0f * float(std::numbers::pi)) {
		worldTransform_.rotation_.y -= 2.0f * float(std::numbers::pi);
	}

}

/// <summary>
/// 腕回転ギミック初期化
/// </summary>
void Enemy::InitializeArmRotationgimmick() {

	// 腕回転ギミックの媒介変数
	armRotationParameter_ = 0.0f;
	// 腕回転ギミックのサイクル<frame>
	armRotationPeriod_ = 1;

}

/// <summary>
/// 腕回転ギミック
/// </summary>
void Enemy::UpdateArmRotationgimmick() {

	// ImGui
	ImGui::Begin("Enemy");
	ImGui::SliderInt("armRotationPeriod", reinterpret_cast<int*>(&armRotationPeriod_), 1, 120);
	ImGui::End();

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / armRotationPeriod_;

	armRotationParameter_ += step;
	armRotationParameter_ = std::fmod(armRotationParameter_, 2.0f * float(std::numbers::pi));

	worldTransformL_arm_.rotation_.x = armRotationParameter_;
	worldTransformR_arm_.rotation_.x = armRotationParameter_;

}
