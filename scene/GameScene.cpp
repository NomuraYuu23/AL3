#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	//---3Dモデル---//
	delete model_;
	delete modelSkydome_;
		
	// デバッグカメラ
	delete debugCamera_;
	//レールカメラ
	delete railCamera_;

	//自キャラの解放
	delete player_;

	//敵キャラの解放
	delete enemy_;

	// 天球の解放
	delete skydome_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//---テクスチャハンドル---//
	playerTextureHandle_ = TextureManager::Load("AL3_01.png");
	enemyTextureHandle_ = TextureManager::Load("koke.png");

	//---3Dモデル---//
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//---ビュープロジェクション---//
	//	viewProjection_.farZ;
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f});

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, playerTextureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_, enemyTextureHandle_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	// 衝突マネージャー
	collisionManager.reset(new CollisionManager);

}

void GameScene::Update() {

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
	#endif

	//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		
		//デバッグカメラのビュー行列をコピー
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		//デバッグカメラのプロジェクション行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュー行列の転送
		viewProjection_.TransferMatrix();

	} else {
		//ビュープロジェクション行列の更新と転送
		//viewProjection_.UpdateMatrix();

		// レールカメラ更新
		railCamera_->Update();

		// レールカメラのビュー行列をコピー
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		// レールカメラのプロジェクション行列
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュー行列の転送
		viewProjection_.TransferMatrix();
	}


	//自キャラの更新
	player_->Update();
	// 敵キャラの更新
	if (enemy_) {
		enemy_->Update();
	}

	// リストをクリア
	collisionManager->ListClear();
	// コライダーをリストに登録
	collisionManager->ListRegister(player_);
	collisionManager->ListRegister(enemy_);
	// 自弾全てについて
	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		collisionManager->ListRegister(playerBullet);
	}
	// 敵弾全てについて
	for (EnemyBullet* enemyBullet : enemy_->GetBullets()) {
		collisionManager->ListRegister(enemyBullet);
	}
	//当たり判定
	collisionManager->CheakAllCollision();


}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}
	//天球の描画
	skydome_->Draw(viewProjection_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
