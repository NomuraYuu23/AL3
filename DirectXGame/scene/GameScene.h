#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "AL3Math.h"
#include <list>
#include <sstream>

#include <memory>
#include "Player.h"
#include "Skydome.h"

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
	uint32_t enemyTextureHandle_ = 0;

	//---3Dモデル---//

	//---ビュープロジェクション---//
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayer_ = nullptr;

	//スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> modelSkydome_ = nullptr;

};
