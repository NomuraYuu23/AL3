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
#include "Collider.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "CollisionManager.h"
#include <list>
#include <sstream>

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
	
	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopComands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//---テクスチャハンドル---//
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	//---3Dモデル---//
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//---ビュープロジェクション---//
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;
	//敵の発生コマンド
	std::stringstream enemyPopCommands;
	//敵の発生コマンドの待機中フラグ
	bool commandIsWait = false;
	//敵の発生コマンドの待機中タイマー(フレーム)
	size_t commandWaitTimer = 0;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	//天球
	Skydome* skydome_ = nullptr;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager;

	std::vector<Vector3> controlPoints_ = {
	    {0,  0,   0},
        {10, 10, 10},
        {10, 15, 10},
        {20, 15, 10},
        {20, 0,  20},
        {30, 0,  20},
	};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
