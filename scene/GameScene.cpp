#include "GameScene.h"
//#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	//---3Dモデル---//
	delete model_;
		
	// デバッグカメラ
	delete debugCamera_;

	//自キャラの解放
	delete player_;

	//敵キャラの解放
	delete enemy_;

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

	//---ビュープロジェクション---//
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

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
		viewProjection_.UpdateMatrix();
	}

	//自キャラの更新
	player_->Update();
	// 敵キャラの更新
	if (enemy_) {
		enemy_->Update();
	}

	//当たり判定
	CheakAllCollision();

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

void GameScene::CheakAllCollision() {

	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		//座標AとBの距離を求める
		float distance = std::sqrtf(
		    std::powf(posB.x - posA.x, 2.0f) + std::powf(posB.y - posA.y, 2.0f) +
		    std::powf(posB.z - posA.z, 2.0f));
		//球と球の交差判定
		if (distance <= player_->kRadius + bullet->kRadius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();
	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float distance = std::sqrtf(
		    std::powf(posB.x - posA.x, 2.0f) + std::powf(posB.y - posA.y, 2.0f) +
		    std::powf(posB.z - posA.z, 2.0f));
		// 球と球の交差判定
		if (distance <= enemy_->kRadius + bullet->kRadius) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		// 自弾全てと敵弾全ての当たり判定
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float distance = std::sqrtf(
			    std::powf(posB.x - posA.x, 2.0f) + std::powf(posB.y - posA.y, 2.0f) +
			    std::powf(posB.z - posA.z, 2.0f));
			// 球と球の交差判定
			if (distance <= playerBullet->kRadius + enemyBullet->kRadius) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion

}
