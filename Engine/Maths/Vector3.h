#pragma once
#include"DLLStuff.h"
#include<math.h>
class Matrix;
class Vector2;


class Vector3 {
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	mutable bool newLen = true;
	mutable float Len = 0.f;
	mutable float SqLen = 0.f;
	mutable bool newUnit = true;
	mutable float UnitX = 0.f;
	mutable float UnitY = 0.f;
	mutable float UnitZ = 0.f;
public:

	DLL_TREATMENT static void RotateVectors(const Vector3& xv, const Vector3& yv, float angle, Vector3& xvTOWRITE, Vector3& yvTOWRITE);

	DLL_TREATMENT Vector3();
	DLL_TREATMENT Vector3(float nX, float nY, float nZ);
	DLL_TREATMENT Vector3(float nn);
	DLL_TREATMENT Vector3(const Matrix&& mat);
	
	DLL_TREATMENT Vector3& operator=(const Vector3& toCopy);

	DLL_TREATMENT float SqLength() const;
	DLL_TREATMENT float Length() const;
	DLL_TREATMENT Vector3 Unit() const;

	DLL_TREATMENT Vector3 Cross(const Vector3& vec) const;

	DLL_TREATMENT void sX(float nX);
	DLL_TREATMENT float gX() const;
	DLL_TREATMENT void sY(float nY);
	DLL_TREATMENT float gY() const;
	DLL_TREATMENT void sZ(float nZ);
	DLL_TREATMENT float gZ() const;

	DLL_TREATMENT Vector2 gvXY() const;
	DLL_TREATMENT Vector2 gvYX() const;
	DLL_TREATMENT Vector2 gvXZ() const;
	DLL_TREATMENT Vector2 gvZX() const;
	DLL_TREATMENT Vector2 gvYZ() const;
	DLL_TREATMENT Vector2 gvZY() const;


	DLL_TREATMENT Vector3 operator+(float num) const;
	DLL_TREATMENT Vector3 operator-(float num) const;
	DLL_TREATMENT Vector3 operator*(float num) const;
	DLL_TREATMENT Vector3 operator/(float num) const;

	DLL_TREATMENT Vector3 operator+(const Vector3& vec) const;
	DLL_TREATMENT Vector3 operator-(const Vector3& vec) const;
	DLL_TREATMENT Vector3 operator*(const Vector3& vec) const;
	DLL_TREATMENT Vector3 operator/(const Vector3& vec) const;

	DLL_TREATMENT Vector3 operator+(const Matrix& mat) const;
	DLL_TREATMENT Vector3 operator-(const Matrix& mat) const;
	DLL_TREATMENT Vector3 operator*(const Matrix& mat) const;
	DLL_TREATMENT Vector3 operator/(const Matrix& mat) const;
};

