#pragma once

#include <list>
#include "Vector3.h"
#include "AL3Math.h"
#include "Collider.h"

class CollisionManager {
public:

	// リストのクリア
	void ListClear();
	
	// リスト登録
	void ListRegister(Collider* collider);
	
	// 衝突判定と応答
	void CheakAllCollision();

private:
	// コライダーリスト
	std::list<Collider*> colliders_;

	// コライダー2つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

};
