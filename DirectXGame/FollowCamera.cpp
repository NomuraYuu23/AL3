#include "FollowCamera.h"
#include"Vector3Calc.h"
#include "Matrix4x4Calc.h"
#include "input.h"
#include <Xinput.h>

void FollowCamera::Initialize() {

	viewProjection_.Initialize();

}

void FollowCamera::Update() {

	XINPUT_STATE joyState;

	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -20.0f};

		//カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = Matrix4x4Calc::MakeRotateXMatrix(viewProjection_.rotation_.x);
		Matrix4x4 rotateMatrixY = Matrix4x4Calc::MakeRotateYMatrix(viewProjection_.rotation_.y);
		Matrix4x4 rotateMatrixZ = Matrix4x4Calc::MakeRotateZMatrix(viewProjection_.rotation_.z);

		Matrix4x4 rotateMatrix = Matrix4x4Calc::Multiply(
		    rotateMatrixX, Matrix4x4Calc::Multiply(rotateMatrixY, rotateMatrixZ));

		//オフセットをカメラの回転に合わせて回転させる
		offset = Matrix4x4Calc::TransformNormal(offset, rotateMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Vector3Calc::Add(target_->translation_, offset);

	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	
		const float RotateSpeed = 0.000001f;

		viewProjection_.rotation_.y += float(joyState.Gamepad.sThumbRX) * RotateSpeed;
	
	}

	//ビュー更新
	viewProjection_.UpdateMatrix();

}
