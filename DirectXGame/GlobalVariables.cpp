#include "GlobalVariables.h"

/// <summary>
/// インスタンスの取得
/// </summary>
/// <returns></returns>
GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;
	return &instance;
}

/// <summary>
/// グループの作成
/// </summary>
/// <param name="groupName">グループ名</param>
void GlobalVariables::CreateGroup(const std::string& groupName) {

	//指定名のオブジェクトがなければ追加する
	datas_[groupName];

}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName">グループ名</param>
/// <param name="key">項目名</param>
/// <param name="value">値</param>
void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, int32_t value) {
	
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;

}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName">グループ名</param>
/// <param name="key">項目名</param>
/// <param name="value">値</param>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;

}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName">グループ名</param>
/// <param name="key">項目名</param>
/// <param name="value">値</param>
void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, const Vector3& value) {

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;

}
