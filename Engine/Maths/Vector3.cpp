#include"Vector3.h"
#include"Vector2.h"
#include"Matrix.h"


Vector3 operator+(float num, const Vector3& vec) { return Vector3(vec.gX() + num, vec.gY() + num, vec.gZ() + num); };
Vector3 operator-(float num, const Vector3& vec) { return Vector3(vec.gX() - num, vec.gY() - num, vec.gZ() - num); };
Vector3 operator*(float num, const Vector3& vec) { return Vector3(vec.gX() * num, vec.gY() * num, vec.gZ() * num); };
Vector3 operator/(float num, const Vector3& vec) { return Vector3(vec.gX() / num, vec.gY() / num, vec.gZ() / num); };

void Vector3::RotateVectors(const Vector3& xv, const Vector3& yv, float angle, Vector3& xvTOWRITE, Vector3& yvTOWRITE) {
	//a little problem is that function can receive same variables as xv and xvTOWRITE(similar with yv) so we need to do 1 copy to not break everything
	Vector3 savedXv = xv;
	xvTOWRITE = xv * cosf(angle) + yv * sinf(angle);
	yvTOWRITE = yv * cosf(angle) - savedXv * sinf(angle);
}

Vector2 Vector3::gvXY() const {
	return Vector2(X, Y);
}
Vector2 Vector3::gvYX() const {
	return Vector2(Y, X);
}
Vector2 Vector3::gvXZ() const {
	return Vector2(X, Z);
}
Vector2 Vector3::gvZX() const {
	return Vector2(Z, X);
}
Vector2 Vector3::gvYZ() const {
	return Vector2(Y, Z);
}
Vector2 Vector3::gvZY() const {
	return Vector2(Z, Y);
}

Vector3::Vector3() {

}
Vector3::Vector3(float nX, float nY, float nZ) {
	X = nX;
	Y = nY;
	Z = nZ;
}
Vector3::Vector3(float nn) {
	X = nn;
	Y = nn;
	Z = nn;
}

Vector3::Vector3(const Matrix&& mat) {
	X = mat[0];
	Y = mat[1];
	Z = mat[2];
}

Vector3& Vector3::operator=(const Vector3& toCopy) {
	newLen = true;
	newUnit = true;
	X = toCopy.X;
	Y = toCopy.Y;
	Z = toCopy.Z;
	return *this;
}

float Vector3::SqLength() const {
	if (newLen) {
		SqLen = X * X + Y * Y + Z * Z;
		newLen = false;
	}
	return SqLen;
}
float Vector3::Length() const {
	if (newLen) {
		Len = sqrtf(X * X + Y * Y + Z * Z);
		newLen = false;
	}
	return Len;
}
Vector3 Vector3::Unit() const {
	if (newUnit) {
		if (newLen) Length();
		UnitX = X / Len;
		UnitY = Y / Len;
		UnitZ = Z / Len;
		newUnit = false;
	}
	return Vector3(UnitX, UnitY, UnitZ);
}

Vector3 Vector3::Cross(const Vector3& vec) const {
	return Vector3(Y * vec.Z - vec.Y * Z, Z * vec.X - vec.Z * X, X * vec.Y - vec.X * Y);
}

void Vector3::sX(float nX) {
	X = nX;
	newUnit = true;
	newLen = true;
}
float Vector3::gX() const {
	return X;
}
void Vector3::sY(float nY) {
	Y = nY;
	newUnit = true;
	newLen = true;
}
float Vector3::gY() const {
	return Y;
}
void Vector3::sZ(float nZ) {
	Z = nZ;
	newUnit = true;
	newLen = true;
}
float Vector3::gZ() const {
	return Z;
}

Vector3 Vector3::operator+(float num) const { return Vector3(X + num, Y + num, Z + num); };
Vector3 Vector3::operator-(float num) const { return Vector3(X - num, Y - num, Z - num); };
Vector3 Vector3::operator*(float num) const { return Vector3(X * num, Y * num, Z * num); };
Vector3 Vector3::operator/(float num) const { return Vector3(X / num, Y / num, Z / num); };

Vector3 Vector3::operator+(const Vector3& vec) const { return Vector3(X + vec.gX(), Y + vec.gY(), Z + vec.gZ()); };
Vector3 Vector3::operator-(const Vector3& vec) const { return Vector3(X - vec.gX(), Y - vec.gY(), Z - vec.gZ()); };
Vector3 Vector3::operator*(const Vector3& vec) const { return Vector3(X * vec.gX(), Y * vec.gY(), Z * vec.gZ()); };
Vector3 Vector3::operator/(const Vector3& vec) const { return Vector3(X / vec.gX(), Y / vec.gY(), Z / vec.gZ()); };

Vector3 Vector3::operator+(const Matrix& mat) const { return Vector3(X + mat[0], Y + mat[1], Z + mat[2]); };
Vector3 Vector3::operator-(const Matrix& mat) const { return Vector3(X - mat[0], Y - mat[1], Z - mat[2]); };
Vector3 Vector3::operator*(const Matrix& mat) const { return Vector3(X * mat[0], Y * mat[1], Z * mat[2]); };
Vector3 Vector3::operator/(const Matrix& mat) const { return Vector3(X / mat[0], Y / mat[1], Z / mat[2]); };