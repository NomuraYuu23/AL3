#include "Player.h"
#include <cassert>
#include "input.h"
#include <Xinput.h>
#include <cmath>
#include <numbers>
#include "ImGuiManager.h"


#include "Vector3Calc.h"
#include "Matrix4x4Calc.h"

enum playerModelIndex {
	kModelIndexBody,
	kModelIndexHead,
	kModelIndexL_arm,
	kModelIndexR_arm
};

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(const std::vector<Model*>& models) {

	//nullポインタチェック
	assert(models.front());

	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_.y += 3.5f;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.y += 2.5f;
	worldTransformL_arm_.translation_.x -= 1.0f;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.y += 2.5f;
	worldTransformR_arm_.translation_.x += 1.0f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	//浮遊ギミック
	InitializeFloatinggimmick();

	//ぶらぶらギミック
	InitializeSwinggimmick();
	
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {

	XINPUT_STATE joyState;
	
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;

		//移動量
		Vector3 move = { float(joyState.Gamepad.sThumbLX), 0.0f, float(joyState.Gamepad.sThumbLY) };
		//移動量に速さを反映
		move = Vector3Calc::Multiply(speed, Vector3Calc::Normalize(move));

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = Matrix4x4Calc::MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateMatrixY = Matrix4x4Calc::MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateMatrixZ = Matrix4x4Calc::MakeRotateZMatrix(viewProjection_->rotation_.z);

		Matrix4x4 rotateMatrix = Matrix4x4Calc::Multiply(
		    rotateMatrixX, Matrix4x4Calc::Multiply(rotateMatrixY, rotateMatrixZ));

		//移動ベクトルをカメラの角度だけ回転する
		move = Matrix4x4Calc::TransformNormal(move, rotateMatrix);

		//移動
		worldTransform_.translation_ = Vector3Calc::Add(worldTransform_.translation_, move);

		//移動方向に見た目を合わせる(Y軸)
		if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
			worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
		}

	}

	// 浮遊ギミック
	UpdateFloatinggimmick();
	
	//ぶらぶらギミック
	UpdateSwinggimmick();

	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(const ViewProjection& viewProjection) {

	//model_->Draw(worldTransform_, viewProjection);

	//for (Model* model : models_) {
	//	model->Draw(worldTransform_, viewProjection);
	//}
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

}

/// <summary>
/// 浮遊ギミック初期化
/// </summary>
void Player::InitializeFloatinggimmick() {

	// 浮遊ギミックの媒介変数
	floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<frame>
	floatingPeriod_ = 1;
	// 浮遊の振幅<m>
	floatingAmplitude_ = 0.0f;

}

/// <summary>
/// 浮遊ギミック初期化
/// </summary>
void Player::UpdateFloatinggimmick() {
	
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderInt("floatingPeriod", reinterpret_cast<int*>(&floatingPeriod_), 1, 120);
	ImGui::SliderFloat("floatingAmplitude", &floatingAmplitude_, 0.0f, 10.0f);
	ImGui::End();

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / floatingPeriod_;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(std::numbers::pi));


	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_ + 1.0f;

}

/// <summary>
/// 浮遊ギミック初期化
/// </summary>
void Player::InitializeSwinggimmick() {

	// ぶらぶらアニメーションの媒介変数
	swingParameter_ = 0.0f;
	// ぶらぶらアニメーションのサイクル<frame>
	swingPeriod_ = 1;
}

/// <summary>
/// 浮遊ギミック更新
/// </summary>
void Player::UpdateSwinggimmick() {

	ImGui::Begin("Player");
	ImGui::SliderInt("swingPeriod", reinterpret_cast<int*>(&swingPeriod_), 1, 120);
	ImGui::End();

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / swingPeriod_;
	// パラメータを1ステップ分加算
	swingParameter_ += step;
	// 2πを超えたら0に戻す
	swingParameter_ = std::fmod(swingParameter_, 2.0f * float(std::numbers::pi));

	worldTransformL_arm_.rotation_.x = std::sinf(swingParameter_) / 2.0f;
	worldTransformR_arm_.rotation_.x = std::sinf(swingParameter_) / 2.0f;

}
