#include"Vector2.h"

Vector2 operator+(float num, const Vector2& vec) { return Vector2(vec.gX() + num, vec.gY() + num); };
Vector2 operator-(float num, const Vector2& vec) { return Vector2(vec.gX() - num, vec.gY() - num); };
Vector2 operator*(float num, const Vector2& vec) { return Vector2(vec.gX() * num, vec.gY() * num); };
Vector2 operator/(float num, const Vector2& vec) { return Vector2(vec.gX() / num, vec.gY() / num); };




Vector2::Vector2() {

}
Vector2::Vector2(float nX, float nY) {
	X = nX;
	Y = nY;
	newLen = true;
	newUnit = true;
}
Vector2::Vector2(float nAx) {
	X = nAx;
	Y = nAx;
	newLen = true;
	newUnit = true;
}

float Vector2::SqLength() const {
	if (newLen) {
		SqLen = X * X + Y * Y;
		newLen = false;
	}
	return SqLen;
}
float Vector2::Length() const {
	if (newLen) {
		Len = sqrtf(X * X + Y * Y);
		newLen = false;
	}
	return Len;
}
Vector2 Vector2::Unit() const {
	if (newUnit) {
		if (newLen) Length();
		UnitX = X / Len;
		UnitY = Y / Len;
		newUnit = false;
	}
	return Vector2(UnitX, UnitY);
}

void Vector2::sX(float nX) {
	X = nX;
	newUnit = true;
	newLen = true;
}
float Vector2::gX() const {
	return X;
}
void Vector2::sY(float nY) {
	Y = nY;
	newUnit = true;
	newLen = true;
}
float Vector2::gY() const {
	return Y;
}