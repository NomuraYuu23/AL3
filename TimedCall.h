#pragma once

#include <functional>

//時限発動
class TimedCall {

	public:
		//コンストラクタ
	    TimedCall(std::function<void()> function, uint32_t time);
		//更新
	    void Update();
		//完了ならtrueを返す
	    bool IsFinished() { return true; }

	private:
		//コールバック
	    std::function<void()> function_;
		//残り時間
	    uint32_t time_;
		//完了フラグ
	    bool isFinished = false;

};
