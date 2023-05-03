#include "RailCamera.h"
#include "ImGuiManager.h"

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
	
	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	float float3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("translation", float3, 0.0f, 1.0f);
	worldTransform_.translation_.x = float3[0];
	worldTransform_.translation_.y = float3[1];
	worldTransform_.translation_.z = float3[2];

	float3[0] = worldTransform_.rotation_.x;
	float3[1] = worldTransform_.rotation_.y;
	float3[2] = worldTransform_.rotation_.z;
	ImGui::SliderFloat3("rotation", float3, 0.0f, 1.0f);
	worldTransform_.rotation_.x = float3[0];
	worldTransform_.rotation_.y = float3[1];
	worldTransform_.rotation_.z = float3[2];

	ImGui::End();
	
	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ =
	    MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
	
}
