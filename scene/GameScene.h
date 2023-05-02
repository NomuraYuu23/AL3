#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
//#include "ViewProjection.h"
//#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Collider.h"

#include "Player.h"
#include "Enemy.h"

#include "CollisionManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//---テクスチャハンドル---//
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	//---3Dモデル---//
	Model* model_ = nullptr;

	//---ビュープロジェクション---//
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
