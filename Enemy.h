#pragma once
#include "Model.h"
// #include "WorldTransform.h"

//前方宣言
class Enemy;

class BaseEnemyState {
public:
	virtual void Update() = 0;

};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update();
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update();
};

class Enemy {

	public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);

	void ChangeState(BaseEnemyState* newState);

	private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Vector3 velocity = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

};
