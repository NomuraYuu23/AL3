#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"


//staticで宣言したメンバ関数ポインタテーブルの実体
void (Enemy::*Enemy::pFunc[static_cast<size_t>(Phase::Leave) + 1])() = {

	&Enemy::Approach, //接近
	&Enemy::Leave     //離脱

};

	// 初期化
void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();


}

// 更新
void Enemy::Update() {

	//現在のフェーズの関数を実行
	(this->*pFunc[static_cast<size_t>(phase_)])();
	//行列を更新
	worldTransform_.UpdateMatrix();
	//デバッグ用表示
	ImGui::Begin("EnemyDebug");
	ImGui::Text(
	    "Enemy Pos:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::Text("Enemy Phase: %s", PhaseName[static_cast<size_t> (phase_)]);
	ImGui::End();

}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

// 接近フェーズ
void Enemy::Approach() {

	// 移動(ベクトルを加算)
	velocity.z = -0.1f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < -15.0f) {
		phase_ = Phase::Leave;
	}

}

// 離脱フェーズ
void Enemy::Leave() {
	
	// 移動(ベクトルを加算)
	velocity.x = -0.3f;
	velocity.y = 0.3f;
	velocity.z = 0.0f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

}
