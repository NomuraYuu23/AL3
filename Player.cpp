#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include "Enemy.h"

		// デストラクタ
Player::~Player() {

	//bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}


	//スプライトの解放
	delete sprite2DReticle_;

}

void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.z = 50.0f;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("./Resources/Reticle.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle, Vector2(0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));

}

void Player::Update(ViewProjection viewProjection) {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//サイズベクトル
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	
	// 旋回
	Rotate();
	 
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//移動限界座標
	const float kMoveLimitX = 32.0f;
	const float kMoveLimitY = 17.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	ImGui::Begin("PlayerDebug");
	ImGui::Text(
	    "Player Pos:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::Text(
	    "Player Rot:%f,%f,%f", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z);
	ImGui::End();

	worldTransform_.UpdateMatrix();

	// 3Dレティクルの配置
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 30.0f;
	// 自機から3Dレティクルへのオフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	//3Dレティクルの座標を指定
	worldTransform3DReticle_.translation_ = Add(offset, worldTransform_.translation_);
	worldTransform3DReticle_.UpdateMatrix();

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionRecticle = Get3DReticleWorldPosition();
	
	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//ビュー行列とプロジェクション行列,ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	//ワールドスクリーン座標変換(ここで3Dから2Dになる)
	positionRecticle = Transform(positionRecticle, matViewProjectionViewport);

	//敵とレティクルの当たり判定
	positionRecticle = SingleLockon(matViewProjectionViewport, positionRecticle);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionRecticle.x, positionRecticle.y));

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	//3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection);

}

// 旋回
void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}
// 攻撃
void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, 0);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Subtract(lockonPosition, GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		newBullet->SetEnemy(lockonEnemy);

		//弾を登録する
		bullets_.push_back(newBullet);

	}

}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision() {

	//何もしない

}

	// 親となるワールドトランスフォームをセット
void Player::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_ = parent; 
}

// 3DReticleのワールド座標を取得
Vector3 Player::Get3DReticleWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;

}

// UI描画
void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

// シングルロックオン
Vector3 Player::SingleLockon(Matrix4x4 matViewProjectionViewport, Vector3 positionRecticle) {

	for (Enemy* enemy : enemies_) {
		Vector3 positionEnemy = enemy->GetWorldPosition();
		positionEnemy = Transform(positionEnemy, matViewProjectionViewport);

		float distance = std::sqrtf(
		    std::powf(positionEnemy.x - positionRecticle.x, 2.0f) +
		    std::powf(positionEnemy.y - positionRecticle.y, 2.0f)); 

		if (distance < sprite2DReticle_->GetSize().x + enemy->GetRadius()) {
		
			//敵をロックオン
			positionRecticle = positionEnemy;
			lockonPosition = enemy->GetWorldPosition();
			lockonPositionStart = lockonPosition;
			lockonT = 0;
			lockonEnemy = enemy;
			sprite2DReticle_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			return positionRecticle;

		}

	}

	//線形補間で移動
	lockonPosition = Lerp(lockonPositionStart, Get3DReticleWorldPosition(), lockonT);
	if (lockonT >= 1.0f) {
		lockonT = 1.0f;
	} else {
		lockonT += 0.025f;
	}
	sprite2DReticle_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	lockonEnemy = nullptr;

	return Transform(lockonPosition, matViewProjectionViewport);

}