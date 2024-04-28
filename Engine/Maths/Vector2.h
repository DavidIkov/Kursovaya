#pragma once
#include<math.h>



class Vector2 {
	float X = 0.f;
	float Y = 0.f;
	bool newLen = false;
	float Len = 0.f;
	bool newUnit = false;
	float UnitX = 0.f;
	float UnitY = 0.f;
public:
	Vector2();
	Vector2(float nX, float nY);
	Vector2(float nAx);
	
	float Length();
	Vector2 Unit();

	void sX(float nX);
	float gX() const;
	void sY(float nY);
	float gY() const;


	Vector2 operator+(float num) { return Vector2(X + num, Y + num); };
	Vector2 operator-(float num) { return Vector2(X - num, Y - num); };
	Vector2 operator*(float num) { return Vector2(X * num, Y * num); };
	Vector2 operator/(float num) { return Vector2(X / num, Y / num); };

	Vector2 operator+(const Vector2& vec) { return Vector2(X + vec.gX(), Y + vec.gY()); };
	Vector2 operator-(const Vector2& vec) { return Vector2(X - vec.gX(), Y - vec.gY()); };
	Vector2 operator*(const Vector2& vec) { return Vector2(X * vec.gX(), Y * vec.gY()); };
	Vector2 operator/(const Vector2& vec) { return Vector2(X / vec.gX(), Y / vec.gY()); };
};

