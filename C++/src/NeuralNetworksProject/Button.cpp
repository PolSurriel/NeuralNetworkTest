#include "Button.h"
#include "Renderer.h"

Button::Button() {
	tag = Tag::BUTTON;
}

Button::Button(Font _font, Text _normalText, Text _hoverText) : font(_font), normalText(_normalText), hoverText(_hoverText) {
	tag = Tag::BUTTON;
};

void Button::SetRect(Rect _rect) {
	rect = _rect;
}

bool Button::IsHovered(const Vector2 &mousePos) {
	return mousePos.x > rect.x && mousePos.x < rect.x + rect.w &&
		mousePos.y > rect.y && mousePos.y < rect.y + rect.h;
}

void Button::UpdateInputStats(const InputManager &input) {
	if (IsHovered(input.mousePosition)) {
		hovered = true;
		pressed = input.keysDown[(int)InputManager::Keys::MOUSE_LEFT];
	}
	else {
		hovered = pressed = false;
	}
}

void Button::UpdateTextToDraw() {
	textIdToDraw = hovered ? hoverText.id : normalText.id;
}

void Button::Update(const InputManager &input) {
	UpdateInputStats(input);
	UpdateTextToDraw();
}

void Button::Draw() {
	Renderer::Instance()->PushImage(
		textIdToDraw,
		rect
	);
}

SwitchButton::SwitchButton() {
	tag = Tag::SWITCH_BUTTON;
}

SwitchButton::SwitchButton(Font _font, Text _normalText, Text _hoverText, Text _normalOffText, Text _hoverOffText)  {
	font = _font;
	normalText = _normalText;
	hoverText = _hoverText;
	normalOffText = _normalOffText;
	hoverOffText = _hoverOffText;
	state = State::ON;
	tag = Tag::SWITCH_BUTTON;
};

void SwitchButton::UpdateInputStats(const InputManager &input) {
	if (IsHovered(input.mousePosition)) {
		hovered = true;
		if (input.keysDown[(int)InputManager::Keys::MOUSE_LEFT]) {
			pressed = true;
			SwitchStateButton();
		}
		else {
			pressed = false;
		}
	}
	else {
		hovered = pressed = false;
	}
}

void SwitchButton::SwitchStateButton() {
	switch (state)
	{
	case State::ON:
		state = State::OFF;
		break;
	case State::OFF:
		state = State::ON;
		break;
	}
}

void SwitchButton::UpdateTextToDraw() {
	switch (state)
	{
	case State::ON:
		textIdToDraw = hovered ? hoverText.id : normalText.id;
		rect = rectOn;
		break;
	case State::OFF:
		textIdToDraw = hovered ? hoverOffText.id : normalOffText.id;
		rect = rectOff;
		break;
	}
}

void SwitchButton::SetRect(Rect _rect) {
	rectOn = rectOff = _rect;
}

void SwitchButton::SetRect(Rect _rectOn, Rect _rectOff) {
	rectOn = _rectOn;
	rectOff = _rectOff;
}