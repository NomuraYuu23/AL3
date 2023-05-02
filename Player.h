#pragma once
#include "Model.h"
//#include "WorldTransform.h"
#include <Input.h>
#include "PlayerBullet.h"
#include <list>

//---自キャラクラス---//
class Player {

	public:
		//デストラクタ
	    ~Player();
		//初期化
	    void Initialize(Model* model, uint32_t textureHandle);
		//更新
	    void Update();
		//描画
	    void Draw(ViewProjection viewProjection);
		
		//旋回
	    void Rotate();
		//攻撃
	    void Attack();

		//ワールド座標を取得
	    Vector3 GetWorldPosition();

		//衝突を検出したら呼び出されるコールバック関数
	    void OnCollision();

		//弾リストを取得
	    const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

		//半径
	    static const int kRadius = 1;

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

		//キーボード入力
	    Input* input_ = nullptr;

		//弾
	    std::list<PlayerBullet*> bullets_;

};
