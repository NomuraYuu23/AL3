#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <fstream>

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
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	// 敵bullets_の解放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

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
	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldMatrix());
	//自キャラに敵キャラのアドレスを渡す
	player_->SetEnemies(enemies_);

	//敵キャラの生成
	Enemy* enemy = new Enemy();
	//敵キャラの初期化
	enemy->Initialize(model_, enemyTextureHandle_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	//敵キャラにゲームシーンのアドレスを渡す
	enemy->SetGameScene(this);

	enemies_.push_back(enemy);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	// 衝突マネージャー
	collisionManager.reset(new CollisionManager);

	//敵発生データの読み込み
	LoadEnemyPopData();

	//レティクルのテクスチャ
	TextureManager::Load("./Resources/Reticle.png");

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
		railCamera_->Update(controlPoints_);

		// レールカメラのビュー行列をコピー
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		// レールカメラのプロジェクション行列
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュー行列の転送
		viewProjection_.TransferMatrix();
	}


	//自キャラの更新
	player_->Update(viewProjection_);

	//敵発生コマンドの更新
	UpdateEnemyPopComands();
	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	//敵弾の処理
	// デスフラグの立った敵弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->SetPlayer(player_);
		bullet->Update();
	}


	// リストをクリア
	collisionManager->ListClear();
	// コライダーをリストに登録
	// 自機について
	collisionManager->ListRegister(player_);
	// 敵全てについて
	for (Enemy* enemy : enemies_) {
		collisionManager->ListRegister(enemy);
	}
	// 自弾全てについて
	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		collisionManager->ListRegister(playerBullet);
	}
	// 敵弾全てについて
	for (EnemyBullet* enemyBullet : enemyBullets_) {
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
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	// 敵弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	//天球の描画
	skydome_->Draw(viewProjection_);

	//デバッグレール描画
	CatmullRomSplineDraw(controlPoints_, 100, &viewProjection_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	//リストに登録する
	enemyBullets_.push_back(enemyBullet);

}

void GameScene::LoadEnemyPopData() {

	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopComands() {

	//待機処理
	if (commandIsWait) {
		commandWaitTimer--;
		if (commandWaitTimer <= 0) {
			//待機完了
			commandIsWait = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
		
			//敵を発生させる
			// 敵キャラの生成
			Enemy* enemy = new Enemy();
			// 敵キャラの初期化
			enemy->Initialize(model_, enemyTextureHandle_);
			// 敵キャラに自キャラのアドレスを渡す
			enemy->SetPlayer(player_);
			// 敵キャラにゲームシーンのアドレスを渡す
			enemy->SetGameScene(this);
			//敵キャラに座標を渡す
			enemy->SetWorldTransformTranslation(Vector3(x, y, z));

			enemies_.push_back(enemy);

		}

		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			commandIsWait = true;
			commandWaitTimer = waitTime;

			//コマンドループを抜ける
			break;
		
		}

	}

}
