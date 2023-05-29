#include "RailCamera.h"
#include "ImGuiManager.h"
#include <Input.h>
#include <numbers>

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
void RailCamera::Update() {

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// キャラクターの回転ベクトル
	Vector3 rotate = {0, 0, 0};

	// キャラクターの回転速さ
	const float kRotateSpeed = 0.02f;

	// キャラクターの移動ベクトル
	Vector3 move = {1.0f, 1.0f, 1.0f};

	// キャラクターの移動速さ
	const float kMoveSpeed = 0.1f;

	// ワールド座標
	Vector3 worldPos;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		rotate.y += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kRotateSpeed;
		rotate.x -= (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kRotateSpeed;
	}
	
	// ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate);
	
	if (worldTransform_.rotation_.y < -float(std::numbers::pi)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y + float(std::numbers::pi) * 2.0f;
	} else if (worldTransform_.rotation_.y > float(std::numbers::pi)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y - float(std::numbers::pi) * 2.0f;
	}

	if (worldTransform_.rotation_.x < -float(std::numbers::pi) / 2.0f) {
		worldTransform_.rotation_.x = -float(std::numbers::pi) / 2.0f;
	} else if (worldTransform_.rotation_.x > float(std::numbers::pi) / 2.0f) {
		worldTransform_.rotation_.x = float(std::numbers::pi) / 2.0f;
	}

	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 距離
	const float kMovePointDistance = 30.0f;
	// オフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// ワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ワールド座標を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	// ベクトルの長さを整える
	offset = Multiply(kMovePointDistance, Normalize(offset));
	// 座標を指定
	Vector3 MovePoint = Add(offset, worldPos);

	// 速度ベクトルを回転させる
	move = Subtract(MovePoint, worldPos);
	move = Multiply(kMoveSpeed, Normalize(move));

	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	//worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	
	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


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

	/*

	//tを進める
	t += velocity;
	if (t >= 1.0f) {
		t = 1.0f;
	} else {
		eye = worldTransform_.translation_;
		float targetT = t + velocity * targetDistance;
		if (targetT >= 1.0f) {
			targetT = 1.0f;
		}
		target = CatmullRomSpline(controlPoints, targetT);

		// eyeをワールドトランスフォームに
		//worldTransform_.translation_ = eye;
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

	*/

	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ =
	    MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// debug
	//worldTransform_.matWorld_ = MakeAffineMatrix(
	  //  worldTransform_.scale_, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, -100.0f});
	// debugここまで

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
	
}
