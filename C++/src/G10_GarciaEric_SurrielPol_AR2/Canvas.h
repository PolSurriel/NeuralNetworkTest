#pragma once
#include "Types.h"
#include "Button.h"
#include "Consts.h"
#include "InputManager.h"


class Canvas {
public:
	Font textFont;

	Sprite* backgroundSprite;
	Rect canvasRect;

	Text runText[RUN_TEXT_CANVAS_LINES];
	Rect runTextRect[RUN_TEXT_CANVAS_LINES];

	virtual void Init();
	virtual void InitFont();
	virtual void InitText();
	virtual void InitRects();
	virtual void Draw();
};

class ButtonCanvas : public Canvas {
private:
	Font stopFont;
	Text stopText;
	Rect stopTextRect;
	SwitchButton soundButton;

public:

	ButtonCanvas();
	void Init();
	void InitButton();
	void InitFont();
	void InitText();
	void InitRects();
	void Update(const InputManager &input);
	void Draw();
};