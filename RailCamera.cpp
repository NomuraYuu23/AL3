#include "RailCamera.h"

// 初期化
void RailCamera::Initialize(const Vector3& position, const Vector3& rotation) {

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

// 更新
void RailCamera::Update() {

	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_ = Add(worldTransform_.translation_, translationVelocity);
	//ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotationVelocity);
	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
	
}
