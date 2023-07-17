#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <memory>
#include <vector>
#include <optional>

#include "BaseCharacter.h"

// モデル
enum class playerModelIndex {
	kModelIndexBody = 0,
	kModelIndexHead = 1,
	kModelIndexL_arm = 2,
	kModelIndexR_arm = 3,
	kModelIndexWeapon = 4
};

// 振るまい
enum class Behavior {
	kRoot, // 通常状態
	kAttack, //攻撃中
};

class Player : public BaseCharacter {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*>& models) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatinggimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatinggimmick();

	/// <summary>
	/// ぶらぶらギミック初期化
	/// </summary>
	void InitializeSwinggimmick();

	/// <summary>
	/// ぶらぶらギミック更新
	/// </summary>
	void UpdateSwinggimmick();

	WorldTransform GetWorldTransform() { return worldTransform_; }

	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

	//ワールド変換データ

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

	//振るまい
	Behavior behavior_ = Behavior::kRoot;
	//次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<frame>
	uint16_t floatingPeriod_ = 1;
	// 浮遊の振幅<m>
	float floatingAmplitude_ = 0.0f;

	//ぶらぶらギミックの媒介変数
	float swingParameter_ = 0.0f;
	//ぶらぶらギミックのサイクル<frame>
	uint16_t swingPeriod_ = 1;

	//攻撃行動用の媒介変数
	float behaviorAttackParameter_ = 0.0f;
	// 攻撃行動用のサイクル<frame>
	uint16_t behaviorAttackPeriod_ = 60;

};
