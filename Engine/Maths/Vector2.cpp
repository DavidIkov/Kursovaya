#include"Vector2.h"
#include"Matrix.h"


Vector2 operator+(float num, const Vector2& vec) { return Vector2(vec.gX() + num, vec.gY() + num); };
Vector2 operator-(float num, const Vector2& vec) { return Vector2(vec.gX() - num, vec.gY() - num); };
Vector2 operator*(float num, const Vector2& vec) { return Vector2(vec.gX() * num, vec.gY() * num); };
Vector2 operator/(float num, const Vector2& vec) { return Vector2(vec.gX() / num, vec.gY() / num); };

void Vector2::RotateVectors(const Vector2& xv, const Vector2& yv, float angle, Vector2& xvTOWRITE, Vector2& yvTOWRITE) {
	//a little problem is that function can receive same variables as xv and xvTOWRITE(similar with yv) so we need to do 1 copy to not break everything
	Vector2 savedXv = xv;
	xvTOWRITE = xv * cosf(angle) + yv * sinf(angle);
	yvTOWRITE = yv * cosf(angle) - savedXv * sinf(angle);
}

float Vector2::DotProduct(const Vector2& vec) const {
	return (vec.X * X + vec.Y * Y) / vec.Length() / Length();
}

Vector2::Vector2() {

}
Vector2::Vector2(float nX, float nY) {
	X = nX;
	Y = nY;
}
Vector2::Vector2(float nAx) {
	X = nAx;
	Y = nAx;
}

Vector2::Vector2(const Matrix&& mat) {
	X = mat[0];
	Y = mat[1];
}

Vector2& Vector2::operator=(const Vector2& toCopy) {
	newLen = true;
	newUnit = true;
	X = toCopy.X;
	Y = toCopy.Y;
	return *this;
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

Vector2 Vector2::operator+(float num) const { return Vector2(X + num, Y + num); };
Vector2 Vector2::operator-(float num) const { return Vector2(X - num, Y - num); };
Vector2 Vector2::operator*(float num) const { return Vector2(X * num, Y * num); };
Vector2 Vector2::operator/(float num) const { return Vector2(X / num, Y / num); };

Vector2 Vector2::operator+(const Vector2& vec) const { return Vector2(X + vec.gX(), Y + vec.gY()); };
Vector2 Vector2::operator-(const Vector2& vec) const { return Vector2(X - vec.gX(), Y - vec.gY()); };
Vector2 Vector2::operator*(const Vector2& vec) const { return Vector2(X * vec.gX(), Y * vec.gY()); };
Vector2 Vector2::operator/(const Vector2& vec) const { return Vector2(X / vec.gX(), Y / vec.gY()); };

Vector2 Vector2::operator+(const Matrix& mat) const { return Vector2(X + mat[0], Y + mat[1]); };
Vector2 Vector2::operator-(const Matrix& mat) const { return Vector2(X - mat[0], Y - mat[1]); };
Vector2 Vector2::operator*(const Matrix& mat) const { return Vector2(X * mat[0], Y * mat[1]); };
Vector2 Vector2::operator/(const Matrix& mat) const { return Vector2(X / mat[0], Y / mat[1]); };