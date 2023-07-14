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

	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmL Rotation", &worldTransformL_arm_.rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Rotation", &worldTransformR_arm_.rotation_.x, -10.0f, 10.0f);
	ImGui::End();

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
