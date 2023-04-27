#pragma once
#include "Model.h"
// #include "WorldTransform.h"

class Enemy {

	public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);

	//接近フェーズ
	void Approach();
	//離脱フェーズ
	void Leave();

	private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//行動フェーズ
	enum class Phase {
		Approach, //接近する
		Leave,    //離脱する
	};

	// 行動フェーズ、デバッグ用名前
	const char PhaseName[static_cast<size_t>(Phase::Leave) + 1][16]{
	    "Approach", // 接近する
	    "Leave" // 離脱する
	};

	Phase phase_ = Phase::Approach;

	//メンバ関数ポインタ
	static void (Enemy::*pFunc[static_cast<size_t>(Phase::Leave) + 1])();

	Vector3 velocity = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

};
