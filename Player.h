#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "AL3Math.h"
#include <Input.h>
#include "PlayerBullet.h"
#include <list>

#include "Collider.h"

//---自キャラクラス---//
class Player : public Collider{

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
	    Vector3 GetWorldPosition() override;

		//衝突を検出したら呼び出されるコールバック関数
	    void OnCollision() override;

		//弾リストを取得
	    const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

		//親となるワールドトランスフォームをセット
	    void SetParent(const WorldTransform* parent);

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
