#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <locale>
#include <unordered_map>



struct line {
	int a, b, c;

	line() : a(0), b(0), c(0) {};
	line(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {};
};

struct color {
	int r, g, b, a;

	color() {};
	color(int _r, int _g, int _b, int _a) : r(_r), g(_g), b(_b), a(_a) {};
};

using Font = struct {
	std::string id;
	std::string path;
	int size;
};

struct Text {
	std::string id;
	std::string text;
	color color;
	int w;
	int h;

	void ToUpperCase() {
		std::locale loc;
		for (std::string::size_type i = 0; i < text.length(); ++i)
			text[i] = std::toupper(text[i], loc);
	}

	void ToLowerCase() {
		std::locale loc;
		for (std::string::size_type i = 0; i < text.length(); ++i)
			text[i] = std::tolower(text[i], loc);
	}
};


class Vector2 {
public:
	float x;
	float y;

	Vector2() : x(0), y(0) {};
	Vector2(float _x, float _y) : x(_x), y(_y) {};

	template <class NumberT>
	Vector2 operator *(const NumberT& number) const { return Vector2(x * number, y * number); };
	template <class NumberT>
	Vector2 operator /(const NumberT& number) const { return Vector2(x / number, y / number); };
	float operator *(const Vector2& b) const { return x*b.x + y*b.y; };
	Vector2 operator +(const Vector2& b) const { return Vector2(x + b.x, y + b.y); };
	Vector2 operator -(const Vector2& b) const { return Vector2(x - b.x, y - b.y); };
	bool operator ==(const Vector2& b) const { return x == b.x && y == b.y; };
	bool operator !=(const Vector2& b) const { return x != b.x || y != b.y; };

	static Vector2 Perpendicular(const Vector2& v) {
		return { -v.y, v.x };
	}

	float GetMagnitude() {
		return sqrt(x*x + y*y);
	}

	void Normalize() {
		/*float mg = GetMagnitude();
		x /= mg;
		y /= mg;*/
		*this = *this / GetMagnitude();
	}

	Vector2 ProjectionOnto(Vector2& v) {
		float vMagnitude = v.GetMagnitude();
		return v * (((*this) * v) / (vMagnitude * vMagnitude));
	}

	float GetAngleInRadians() {
		return atanf(y/x);
	}

	float DistanceTo(const Vector2& position) {
		return sqrt(pow(position.x - x, 2) + pow(position.y - y, 2));
	}
};

struct Rect {
	int x, y;
	int w, h;

	Rect() : x(0), y(0), w(0), h(0) {}
	Rect(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
	Rect(Vector2 position, Vector2 size) : x(position.x), y(position.y), w(size.x), h(size.y) {}

	void SetPosition(const Vector2 &v) {
		x = v.x;
		y = v.y;
	}

	void SetSize(const Vector2 &v) {
		w = v.x;
		h = v.y;
	}
};


