#include "TimedCall.h"

// コンストラクタ
TimedCall::TimedCall(std::function<void()> function, uint32_t time) : function_(function), time_(time) {}
// 更新
void TimedCall::Update() {

	if (isFinished) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished = true;
		//コールバック関数の呼び出し
		function_();
	}

}
