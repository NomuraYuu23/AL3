#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	//自キャラ生成
	player_ = std::make_unique<Player>();
	// 自キャラのモデル
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));

	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), 
		modelFighterHead_.get(), 
		modelFighterL_arm_.get(),
	    modelFighterR_arm_.get()
	};
	//自キャラの初期化
	player_->Initialize(playerModels);

	// スカイドーム生成
	skydome_ = std::make_unique<Skydome>();
	// スカイドームのモデル
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_.get());

	// グラウンド生成
	ground_ = std::make_unique<Ground>();
	// グラウンドのモデル
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	// グラウンドの初期化
	ground_->Initialize(modelGround_.get());

	// 追従カメラ生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransformAddress());

	//追従カメラのビュープロジェクションを自キャラにセット
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());

	//エネミーの生成
	enemy_ = std::make_unique<Enemy>();
	//エネミーのモデル
	modelEnemyBody_.reset(Model::CreateFromOBJ("enemy_Body", true));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("enemy_Arm", true));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("enemy_Arm", true));

	std::vector<Model*> enemyModels = {
	    modelEnemyBody_.get(),
		modelEnemyL_arm_.get(),
		modelEnemyR_arm_.get()
	};
	//エネミーの初期化
	enemy_->Initialize(enemyModels);


}

void GameScene::Update() {

		#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();

		// デバッグカメラのビュー行列をコピー
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		// デバッグカメラのプロジェクション行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュー行列の転送
		viewProjection_.TransferMatrix();

	} else {
		//追従カメラの更新
		followCamera_->Update();

		// デバッグカメラのビュー行列をコピー
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		// デバッグカメラのプロジェクション行列
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		// ビュー行列の転送
		viewProjection_.TransferMatrix();
	}

	player_->Update();

	enemy_->Update();

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

	//スカイドームの描画
	skydome_->Draw(viewProjection_);

	//グラウンドの描画
	ground_->Draw(viewProjection_);

	//自キャラの描画
	player_->Draw(viewProjection_);

	//エネミーの描画
	enemy_->Draw(viewProjection_);

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
