#include "Player.h"
#include <cassert>
#include <input.h>
#include <Xinput.h>

#include"Vector3Calc.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model){

	//nullポインタチェック
	assert(model);

	model_ = model;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

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

		worldTransform_.translation_ = Vector3Calc::Add(worldTransform_.translation_, move);

	}

	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();


}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection);

}