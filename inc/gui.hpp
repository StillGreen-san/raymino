#pragma once

#include "app.hpp"
#include "types.hpp"

#include <raygui.h>

#include <optional>
#include <random>
#include <string>
#include <string_view>

namespace raymino
{
inline bool GuiSpinner(::Rectangle bounds, const char* text, int& value, int minValue, int maxValue, bool& editMode)
{
	if(::GuiSpinner(bounds, text, &value, minValue, maxValue, editMode))
	{
		editMode = !editMode;
		return true;
	}
	return false;
}
inline bool GuiDropdownBox(::Rectangle bounds, const char* text, int& active, bool& editMode)
{
	if(::GuiDropdownBox(bounds, text, &active, editMode))
	{
		editMode = !editMode;
		return true;
	}
	return false;
}
template<typename TContainer>
bool GuiTextBox(::Rectangle bounds, TContainer& container, bool& editMode, int maxLen = INT_MAX)
{
	if(::GuiTextBox(bounds, container.data(), std::min(static_cast<int>(container.size()), maxLen), editMode))
	{
		editMode = !editMode;
		return true;
	}
	return false;
}

/**
 * @brief changes gui lock status in it scope, resetting it to its previous state on destruction
 */
class ScopedGuiLock
{
public:
	explicit ScopedGuiLock(bool lock) : wasLocked{::GuiIsLocked()}
	{
		if(lock)
		{
			::GuiLock();
		}
		else
		{
			::GuiUnlock();
		}
	}
	ScopedGuiLock(const ScopedGuiLock&) = delete;
	ScopedGuiLock(ScopedGuiLock&&) = default;
	ScopedGuiLock& operator=(const ScopedGuiLock&) = delete;
	ScopedGuiLock& operator=(ScopedGuiLock&&) = default;
	~ScopedGuiLock()
	{
		if(wasLocked)
		{
			::GuiLock();
		}
		else
		{
			::GuiUnlock();
		}
	}

private:
	bool wasLocked;
};

/**
 * @brief a ';' seperated list of strings
 */
class TextList
{
public:
	static constexpr char delimiter = ';';

	/**
	 * @brief empty list
	 */
	TextList() = default;

	/**
	 * @brief list with items
	 * @param items to add (0 or more, seperated by ';')
	 */
	explicit TextList(std::string_view items) : list{items}
	{
	}

	template<typename TIterator>
	explicit TextList(Range<TIterator> range)
	{
		for(const auto& item : range)
		{
			add(item);
		}
	}

	/**
	 * @param items to add (0 or more, seperated by ';')
	 */
	void add(std::string_view items);

	/**
	 * @brief remove item at index if present
	 */
	void remove(size_t index);

	/**
	 * @return item at index or empty view
	 */
	[[nodiscard]] std::string_view get(size_t index) const;

	/**
	 * @return complete list
	 */
	[[nodiscard]] std::string_view get() const;

	/**
	 * @return number of items
	 */
	[[nodiscard]] size_t size() const;

private:
	std::string list;
};

/**
 * @brief gui element to handle Presets collection
 * @tparam TValue type of Presets::Item value
 */
template<typename TValue>
class PresetSelect
{
private:
	enum class State : uint8_t
	{
		Normal,
		Custom,
		Naming,
	};

public:
	/**
	 * @pre presets was managed by PresetSelect
	 */
	PresetSelect(const App::Presets<TValue>& presets, size_t active) :
	    presets{presets}, valueBuffer{presets.get(active).value}, textList{Range{presets.get()}}, activeItem(active),
	    state{presets.size() != 0 && presets.find(customName) == presets.size() - 1 ? State::Custom : State::Normal}
	{
	}
	[[nodiscard]] TValue& getValue()
	{
		return valueBuffer;
	}
	[[nodiscard]] const App::Presets<TValue>& getPresets() const
	{
		return presets;
	}
	[[nodiscard]] size_t active() const
	{
		return static_cast<size_t>(activeItem);
	}
	[[nodiscard]] bool inEditMode() const
	{
		return editMode;
	}
	void updateState()
	{
		if(const size_t index = presets.find(valueBuffer); index != activeItem)
		{
			activeItem = static_cast<int>(index);
			if(index == presets.size())
			{
				if(state == State::Custom)
				{
					activeItem -= 1;
					presets.tryGet(activeItem)->value = valueBuffer;
				}
				else
				{
					textList.add(customName);
					presets.add({customName, valueBuffer});
					state = State::Custom;
				}
			}
		}
	}
	bool handleSelectionBox(::Rectangle pos)
	{
		if(state == State::Naming)
		{
			GuiTextBox(pos, textBuffer, editMode);
			if(!editMode)
			{
				addItem();
			}
		}
		else if(GuiDropdownBox(pos, textList.get().data(), activeItem, editMode) && !editMode)
		{
			valueBuffer = presets.get(activeItem).value;
			return true;
		}
		return false;
	}
	void handleSaveButton(::Rectangle pos, const char* txt)
	{
		if(state != State::Normal && activeItem == presets.size() - 1 && ::GuiButton(pos, txt))
		{
			if(state == State::Naming)
			{
				addItem();
			}
			else
			{
				state = State::Naming;
				std::string presetName(customName);
				presetName.append(1, ' ');
				presetName.append(std::to_string(std::random_device{}()));
				std::copy_n(presetName.begin(), std::min(textBuffer.size() - 1, presetName.size()), textBuffer.begin());
				editMode = true;
			}
		}
	}
	void handleRemoveButton(::Rectangle pos, const char* txt)
	{
		if(activeItem >= presets.fixed() && ::GuiButton(pos, txt))
		{
			if(activeItem == presets.size() - 1)
			{
				state = State::Normal;
			}
			presets.remove(activeItem);
			textList.remove(activeItem);
			activeItem = std::min<int>(static_cast<int>(presets.size() - 1), activeItem);
			valueBuffer = presets.get(activeItem).value;
		}
	}

private:
	void addItem()
	{
		const typename App::Presets<TValue>::Item item{textBuffer, valueBuffer};
		presets.remove(activeItem);
		textList.remove(activeItem);
		presets.add(item);
		textList.add(item);
		activeItem = static_cast<int>(presets.size() - 1);
		state = State::Normal;
	}

	App::Presets<TValue> presets;
	TValue valueBuffer;
	App::Presets<>::NameT textBuffer{};
	TextList textList;
	int activeItem;
	bool editMode = false;
	State state = State::Normal;
	static constexpr std::string_view customName = "Custom";
};
} // namespace raymino
