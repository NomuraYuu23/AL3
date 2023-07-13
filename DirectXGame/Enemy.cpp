#include "Player.h"
#include <cassert>
#include "input.h"
#include <Xinput.h>
#include <cmath>
#include <numbers>
#include "ImGuiManager.h"


#include "Vector3Calc.h"
#include "Matrix4x4Calc.h"

#include "Enemy.h"
/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Enemy::Initialize(const std::vector<Model*>& models) {

	// nullポインタチェック
	assert(models.front());

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	worldTransform_.UpdateMatrix();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	 for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	 }

}
