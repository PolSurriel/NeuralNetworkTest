#pragma once
#include "GameObject.h"
#include <string>
#include "InputManager.h"


class Button : public GameObject
{
protected:
	Rect rect;

	Font font;

	Text normalText;
	Text hoverText;

	std::string textIdToDraw;

	bool hovered;

public:
	bool pressed;

	Button();
	Button(Font _font, Text _normalText, Text _hoverText);

	virtual void SetRect(Rect _rect);
	bool IsHovered(const Vector2 &mousePos);

	void Update(const InputManager &input);
	void Draw();

private:
	virtual void UpdateInputStats(const InputManager &input);
	virtual void UpdateTextToDraw();
};

class SwitchButton : public Button {
private:

	Rect rectOn;
	Rect rectOff;

	Text normalOffText;
	Text hoverOffText;

	void SwitchStateButton();

	virtual void UpdateInputStats(const InputManager &input);
	virtual void UpdateTextToDraw();

public:
	enum class State{ ON, OFF};
	State state;

	SwitchButton();
	SwitchButton(Font _font, Text _normalText, Text _hoverText, Text _normalOffText, Text _hoverOffText);
	void SetRect(Rect _rect);
	void SetRect(Rect _rectOn, Rect _rectOff);

};