#include "GlobalVariables.h"
#include <ImGuiManager.h>

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

/// <summary>
/// 毎フレーム処理
/// </summary>
void GlobalVariables::Update() {

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
	     ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;
		
		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		     itItem != group.items.end(); ++itItem) {
			
			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			// float型の値を保持していれば
			if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, -100.0f, 100.0f);
			}

			// Vector3型の値を保持していれば
			if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}

		}


		ImGui::EndMenu();

	}
	
	ImGui::EndMenuBar();
	ImGui::End();

}
