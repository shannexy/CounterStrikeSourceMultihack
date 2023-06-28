#pragma once
#include "imgui/imgui.h"
#include <functional>
#include <string>
#include <vector>

class components {
public:
	template<class T>
	static void combo_list(int& selected_index, std::string_view label, std::string_view none, std::vector<T> items, std::function<std::string_view(T current)> item_callback)
	{
		if (ImGui::BeginCombo(label.data(),
			selected_index == -1 ? none.data() :
			static_cast<std::string_view>(item_callback(items.at(selected_index))).data()))
		{
			if (ImGui::Selectable(none.data(), selected_index == -1))
				selected_index = -1;

			for (size_t i = 0; i < items.size(); i++)
			{
				auto text = static_cast<std::string_view>(item_callback(items.at(i))).data();
				if (ImGui::Selectable(text, selected_index == i))
					selected_index = i;

				if (selected_index == i)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	template<class T>
	static void combo_enum(T& selected_enum, std::string_view label, std::vector<T> items, std::function<std::string_view(T current)> item_callback) {
		if (
			ImGui::BeginCombo(
				label.data(),
				static_cast<std::string_view>(item_callback(selected_enum)).data()
			)
		) {
			for (size_t i = 0; i < items.size(); i++)
			{
				auto text = static_cast<std::string_view>(item_callback((T)i)).data();
				if (ImGui::Selectable(text, selected_enum == i))
					selected_enum = (T)i;

				if (selected_enum == i)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
};