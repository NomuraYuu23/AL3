#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {

public:

	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	ViewProjection GetViewProjection() { return viewProjection_; }

	ViewProjection* GetViewProjectionAddress() { return &viewProjection_; }

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

};
