#include "FollowCamera.h"
#include"Vector3Calc.h"

void FollowCamera::Initialize() {

	viewProjection_.Initialize();

}

void FollowCamera::Update() {

	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -20.0f};

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Vector3Calc::Add(target_->translation_, offset);

	}

	//ビュー更新
	viewProjection_.UpdateMatrix();

}
