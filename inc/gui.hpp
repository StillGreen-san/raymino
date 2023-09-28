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
inline bool GuiSpinner(
    ::Rectangle bounds, const char* text, int& value, int minValue, int maxValue, bool& editMode) noexcept
{
	if(::GuiSpinner(bounds, text, &value, minValue, maxValue, editMode))
	{
		editMode = !editMode;
		return true;
	}
	return false;
}
inline bool GuiDropdownBox(::Rectangle bounds, const char* text, int& active, bool& editMode) noexcept
{
	if(::GuiDropdownBox(bounds, text, &active, editMode))
	{
		editMode = !editMode;
		return true;
	}
	return false;
}
template<typename TContainer>
bool GuiTextBox(::Rectangle bounds, TContainer& container, bool& editMode, int maxLen = INT_MAX) noexcept
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
	explicit ScopedGuiLock(bool lock) noexcept : wasLocked{::GuiIsLocked()}
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
	~ScopedGuiLock() noexcept
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
	void remove(size_t index) noexcept;

	/**
	 * @return item at index or empty view
	 */
	[[nodiscard]] std::string_view get(size_t index) const noexcept;

	/**
	 * @return complete list
	 */
	[[nodiscard]] std::string_view get() const noexcept;

	/**
	 * @return complete list
	 */
	operator const char*() const noexcept;

	/**
	 * @return number of items
	 */
	[[nodiscard]] size_t size() const noexcept;

private:
	std::string list;
};

/**
 * @brief splits like: "Hello2WorldFX4" -> "Hello 2 World FX4"
 */
std::string splitCamel(std::string_view text);

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
	    presets{presets}, valueBuffer{presets.get(active).value}, textList{Range{presets.get()}},
	    activeItem(static_cast<int>(active)),
	    state{presets.size() != 0 && presets.find(customName) == presets.size() - 1 ? State::Custom : State::Normal}
	{
	}
	[[nodiscard]] TValue& getValue() noexcept
	{
		return valueBuffer;
	}
	[[nodiscard]] const App::Presets<TValue>& getPresets() const noexcept
	{
		return presets;
	}
	[[nodiscard]] size_t active() const noexcept
	{
		return static_cast<size_t>(activeItem);
	}
	[[nodiscard]] bool inEditMode() const noexcept
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
	void handleRemoveButton(::Rectangle pos, const char* txt) noexcept
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

/**
 * @brief synced integer & text buffer
 */
class NumberBuffer
{
public:
	using TIntType = ptrdiff_t;
	static constexpr size_t Digits10 = std::numeric_limits<TIntType>::digits10 + 1;
	static constexpr size_t Separators = (Digits10 - 1) / 3;
	static constexpr size_t SignChars = static_cast<const size_t>(std::numeric_limits<TIntType>::is_signed);
	static constexpr size_t TermintorChars = 1;
	static constexpr size_t TotalBufferSize = Digits10 + Separators + SignChars + TermintorChars;
	using TBufferType = std::array<char, TotalBufferSize>;

	NumberBuffer() = delete;
	NumberBuffer(TIntType init, char separator = '.') noexcept;
	NumberBuffer& operator+=(TIntType rhs) noexcept;
	[[nodiscard]] const char* c_str() const noexcept;
	[[nodiscard]] TIntType value() const noexcept;

	/**
	 * @brief parses values into buffer with 1000 separators, puts value at end of buffer
	 * @return start index into buffer
	 */
	static unsigned char backFillSeparated(TBufferType& buffer, TIntType value, char separator);

private:
	TIntType number;
	TBufferType buffer{};
	char separator;
	unsigned char startIdx;
};

struct Box
{
	constexpr Box(float top, float right, float bottom, float left) : top{top}, right{right}, bottom{bottom}, left{left}
	{
	}
	constexpr Box(float top, float rightLeft, float bottom) :
	    top{top}, right{rightLeft}, bottom{bottom}, left{rightLeft}
	{
	}
	constexpr Box(float topBottom, float rightLeft) :
	    top{topBottom}, right{rightLeft}, bottom{topBottom}, left{rightLeft}
	{
	}
	constexpr Box(float all) : top{all}, right{all}, bottom{all}, left{all}
	{
	}
	float top;
	float right;
	float bottom;
	float left;
};
} // namespace raymino
