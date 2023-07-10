#include "Player.h"
#include <cassert>
#include "input.h"
#include <Xinput.h>
#include <cmath>

#include "Vector3Calc.h"
#include "Matrix4x4Calc.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(std::vector<Model*> models) {

	//nullポインタチェック
	assert(models.front());

	//モデル読み込み
	models_ = models;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_.y += 3.5f;
	worldTransformHead_.parent_ = &worldTransform_;
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.y += 2.5f;
	worldTransformL_arm_.translation_.x -= 1.0f;
	worldTransformL_arm_.parent_ = &worldTransform_;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.y += 2.5f;
	worldTransformR_arm_.translation_.x += 1.0f;
	worldTransformR_arm_.parent_ = &worldTransform_;
	
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
void Player::Draw(ViewProjection viewProjection){

	//model_->Draw(worldTransform_, viewProjection);

	//for (Model* model : models_) {
	//	model->Draw(worldTransform_, viewProjection);
	//}
	models_.at(0)->Draw(worldTransformBody_, viewProjection);
	models_.at(1)->Draw(worldTransformHead_, viewProjection);
	models_.at(2)->Draw(worldTransformL_arm_, viewProjection);
	models_.at(3)->Draw(worldTransformR_arm_, viewProjection);

}