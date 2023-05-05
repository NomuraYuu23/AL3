#include "RailCamera.h"
#include "ImGuiManager.h"

// 初期化
void RailCamera::Initialize(const Vector3& position, const Vector3& rotation) {

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	t = 0.0f;
	velocity = 0.001f;

}

// 更新
void RailCamera::Update(const std::vector<Vector3>& controlPoints) {

	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	//worldTransform_.translation_ = Add(worldTransform_.translation_, translationVelocity);
	//ワールドトランスフォームの角度の数値を加算したりする(回転)
	//worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotationVelocity);
	
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
	
	//tを進める
	t += velocity;
	if (t >= 1.0f) {
		t = 1.0f;
	} else {
		eye = CatmullRomSpline(controlPoints, t);
		float targetT = t + velocity * targetDistance;
		if (targetT >= 1.0f) {
			targetT = 1.0f;
		}
		target = CatmullRomSpline(controlPoints, targetT);

		// eyeをワールドトランスフォームに
		worldTransform_.translation_ = eye;
		// eyeとtargetの差分ベクトル
		Vector3 forward = Subtract(target, eye);
		// ベクトルの正規化
		forward = Normalize(forward);
		// 進行方向に見た目の回転を合わせる
		// Y軸周りの角度(Θy)
		worldTransform_.rotation_.y = std::atan2f(forward.x, forward.z);
		// 横軸方向の長さを求める
		float length = Length(Vector3{forward.x, 0.0f, forward.z});
		// X軸周りの角度(Θx)
		worldTransform_.rotation_.x = std::atan2f(-forward.y, length);
	}

	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ =
	    MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
	
}
