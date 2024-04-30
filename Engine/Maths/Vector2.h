#pragma once
#include<math.h>



class Vector2 {
	float X = 0.f;
	float Y = 0.f;
	mutable bool newLen = false;
	mutable float Len = 0.f;
	mutable float SqLen = 0.f;
	mutable bool newUnit = false;
	mutable float UnitX = 0.f;
	mutable float UnitY = 0.f;
public:
	Vector2();
	Vector2(float nX, float nY);
	Vector2(float nAx);
	

	float SqLength() const;
	float Length() const;
	Vector2 Unit() const;

	void sX(float nX);
	float gX() const;
	void sY(float nY);
	float gY() const;


	Vector2 operator+(float num) const { return Vector2(X + num, Y + num); };
	Vector2 operator-(float num) const { return Vector2(X - num, Y - num); };
	Vector2 operator*(float num) const { return Vector2(X * num, Y * num); };
	Vector2 operator/(float num) const { return Vector2(X / num, Y / num); };

	Vector2 operator+(const Vector2& vec) const { return Vector2(X + vec.gX(), Y + vec.gY()); };
	Vector2 operator-(const Vector2& vec) const { return Vector2(X - vec.gX(), Y - vec.gY()); };
	Vector2 operator*(const Vector2& vec) const { return Vector2(X * vec.gX(), Y * vec.gY()); };
	Vector2 operator/(const Vector2& vec) const { return Vector2(X / vec.gX(), Y / vec.gY()); };
};

