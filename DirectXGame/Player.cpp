#include "Player.h"
#include <cassert>
#include "input.h"
#include <Xinput.h>
#include <cmath>
#include <numbers>
#include "ImGuiManager.h"


#include "Vector3Calc.h"
#include "Matrix4x4Calc.h"

#include "GlobalVariables.h"

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

	worldTransformWeapon_.Initialize();
	worldTransformWeapon_.translation_.y += 3.0f;
	worldTransformWeapon_.parent_ = &worldTransform_;

	//浮遊ギミック
	InitializeFloatinggimmick();

	//ぶらぶらギミック
	InitializeSwinggimmick();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingPeriod", floatingPeriod_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
	globalVariables->AddItem(groupName, "swingPeriod", swingPeriod_);
	globalVariables->AddItem(groupName, "behaviorAttackPeriod", behaviorAttackPeriod_);

	ApplyGlobalVariables();

}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {

	if (behaviorRequest_) {
		//振るまいを変更する
		behavior_ = behaviorRequest_.value();
		//各振るまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		//振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 振るまい
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	//GlobalVariables::SaveFile確認
	//if (Input::GetInstance()->PushKey(DIK_S)) {
	//	GlobalVariables::GetInstance()->GlobalVariables::SaveFile("Player");
	//}

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
	models_[int(playerModelIndex::kModelIndexBody)]->Draw(worldTransformBody_, viewProjection);
	models_[int(playerModelIndex::kModelIndexHead)]->Draw(worldTransformHead_, viewProjection);
	models_[int(playerModelIndex::kModelIndexL_arm)]->Draw(worldTransformL_arm_, viewProjection);
	models_[int(playerModelIndex::kModelIndexR_arm)]->Draw(worldTransformR_arm_, viewProjection);
	models_[int(playerModelIndex::kModelIndexWeapon)]->Draw(worldTransformWeapon_, viewProjection);

}

/// <summary>
/// 通常行動初期化
/// </summary>
void Player::BehaviorRootInitialize() {

	// 浮遊ギミック
	InitializeFloatinggimmick();

	// ぶらぶらギミック
	InitializeSwinggimmick();
	
	//武器角度
	worldTransformWeapon_.rotation_.x = 0.0f;

}

/// <summary>
/// 通常行動更新
/// </summary>
void Player::BehaviorRootUpdate() {

	XINPUT_STATE joyState;

	//移動
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {float(joyState.Gamepad.sThumbLX), 0.0f, float(joyState.Gamepad.sThumbLY)};
		// 移動量に速さを反映
		move = Vector3Calc::Multiply(speed, Vector3Calc::Normalize(move));

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = Matrix4x4Calc::MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateMatrixY = Matrix4x4Calc::MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateMatrixZ = Matrix4x4Calc::MakeRotateZMatrix(viewProjection_->rotation_.z);

		Matrix4x4 rotateMatrix = Matrix4x4Calc::Multiply(
		    rotateMatrixX, Matrix4x4Calc::Multiply(rotateMatrixY, rotateMatrixZ));

		// 移動ベクトルをカメラの角度だけ回転する
		move = Matrix4x4Calc::TransformNormal(move, rotateMatrix);

		// 移動
		worldTransform_.translation_ = Vector3Calc::Add(worldTransform_.translation_, move);

		// 移動方向に見た目を合わせる(Y軸)
		if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
			worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
		}
	}

	//攻撃
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::kAttack;
	}

	// 浮遊ギミック
	UpdateFloatinggimmick();

	// ぶらぶらギミック
	UpdateSwinggimmick();

}

/// <summary>
/// 攻撃行動初期化
/// </summary>
void Player::BehaviorAttackInitialize() {

	// 攻撃行動用の媒介変数
	behaviorAttackParameter_ = 0.0f;

}

/// <summary>
/// 攻撃行動更新
/// </summary>
void Player::BehaviorAttackUpdate() {

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / behaviorAttackPeriod_;

	// パラメータを1ステップ分加算
	behaviorAttackParameter_ += step;
	if (behaviorAttackParameter_ >= 0.8f * float(std::numbers::pi) &&
	    behaviorAttackParameter_ <= 1.5f * float(std::numbers::pi)) {

		worldTransformL_arm_.rotation_.x = behaviorAttackParameter_;
		worldTransformR_arm_.rotation_.x = behaviorAttackParameter_;
		worldTransformWeapon_.rotation_.x = behaviorAttackParameter_ + float(std::numbers::pi);

		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {0.0f, 0.0f, 1.0f};
		// 移動量に速さを反映
		move = Vector3Calc::Multiply(speed, Vector3Calc::Normalize(move));

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = Matrix4x4Calc::MakeRotateXMatrix(worldTransform_.rotation_.x);
		Matrix4x4 rotateMatrixY = Matrix4x4Calc::MakeRotateYMatrix(worldTransform_.rotation_.y);
		Matrix4x4 rotateMatrixZ = Matrix4x4Calc::MakeRotateZMatrix(worldTransform_.rotation_.z);

		Matrix4x4 rotateMatrix = Matrix4x4Calc::Multiply(
		    rotateMatrixX, Matrix4x4Calc::Multiply(rotateMatrixY, rotateMatrixZ));

		// 移動ベクトルをカメラの角度だけ回転する
		move = Matrix4x4Calc::TransformNormal(move, rotateMatrix);

		// 移動
		worldTransform_.translation_ = Vector3Calc::Add(worldTransform_.translation_, move);

		// 移動方向に見た目を合わせる(Y軸)
		if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
			worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
		}
	} else if (behaviorAttackParameter_ < 0.8f * float(std::numbers::pi)) {
		//	振りかぶり
		worldTransformL_arm_.rotation_.x = -behaviorAttackParameter_;
		worldTransformR_arm_.rotation_.x = -behaviorAttackParameter_;
	
	} else if (behaviorAttackParameter_ >= 2.0f * float(std::numbers::pi)) {
		// 2πを超えたら振るまい変更
		behaviorRequest_ = Behavior::kRoot;
	}

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
	ImGui::SliderInt("swingPeriod", reinterpret_cast<int*>(&swingPeriod_), 1, 120);
	ImGui::SliderInt("behaviorAttackPeriod_", reinterpret_cast<int*>(&behaviorAttackPeriod_), 1, 120);
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

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / swingPeriod_;
	// パラメータを1ステップ分加算
	swingParameter_ += step;
	// 2πを超えたら0に戻す
	swingParameter_ = std::fmod(swingParameter_, 2.0f * float(std::numbers::pi));

	worldTransformL_arm_.rotation_.x = std::sinf(swingParameter_) / 2.0f;
	worldTransformR_arm_.rotation_.x = std::sinf(swingParameter_) / 2.0f;

}

/// <summary>
/// 調整項目の適用
/// </summary>
void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	worldTransformHead_.translation_ =
	    globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmR Translation");

	floatingPeriod_ = globalVariables->GetIntValue(groupName, "floatingPeriod");
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");

	swingPeriod_ = globalVariables->GetIntValue(groupName, "swingPeriod");
	behaviorAttackPeriod_ = globalVariables->GetIntValue(groupName, "behaviorAttackPeriod");

}