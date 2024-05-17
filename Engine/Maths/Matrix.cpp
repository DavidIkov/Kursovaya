#include"Matrix.h"
#include"Vector2.h"

unsigned int Matrix::gSizeX() const {
	return SizeX;
}
unsigned int Matrix::gSizeY() const {
	return SizeY;
}

float Matrix::operator[](unsigned int i) const {
	return Nums[i];
}

Matrix::Matrix() {

}
Matrix::Matrix(unsigned int sx, unsigned int sy, const float(&& nums)[]) {
	SizeX = sx;
	SizeY = sy;
	Nums = new float[SizeX * SizeY];
	for (unsigned int i = 0; i < SizeX * SizeY; i++) Nums[i] = nums[i];
}
Matrix::Matrix(unsigned int sx, unsigned int sy) {
	SizeX = sx;
	SizeY = sy;
	Nums = new float[SizeX * SizeY];
}
Matrix::Matrix(const Matrix& mat) {
	SizeX = mat.SizeX;
	SizeY = mat.SizeY;
	Nums = new float[SizeX * SizeY];
	for (unsigned int i = 0; i < SizeX * SizeY; i++) Nums[i] = mat.Nums[i];
}
Matrix& Matrix::operator=(const Matrix& mat) {
	SizeX = mat.SizeX;
	SizeY = mat.SizeY;
	Nums = new float[SizeX * SizeY];
	for (unsigned int i = 0; i < SizeX * SizeY; i++) Nums[i] = mat.Nums[i];
	return *this;
}
Matrix::~Matrix() {
	delete[] Nums;
}

bool Matrix::IsIndInMatrix(unsigned int i) const {
	return i < SizeX * SizeY;
}
unsigned int Matrix::GetIndByXY(unsigned int x, unsigned int y) const {
	return x * SizeY + y;
}
bool Matrix::IsXYInMatrix(unsigned int x, unsigned int y) const {
	return IsIndInMatrix(GetIndByXY(x, y));
}
Matrix Matrix::operator+(const Matrix& mat) {
	if (mat.SizeX == SizeX and mat.SizeY == SizeY) {
		Matrix retMat(SizeX, SizeY);
		for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] + mat.Nums[i];
		return retMat;
	}
	else {//expand to big matrix
		Matrix retMat(SizeX > mat.SizeX ? SizeX : mat.SizeX, SizeY > mat.SizeY ? SizeY : mat.SizeY);
		for (unsigned int y = 0; y < retMat.SizeY; y++) {
			for (unsigned int x = 0; x < retMat.SizeX; x++) {
				retMat.Nums[retMat.GetIndByXY(x, y)] =
					((y < SizeY and x < SizeX) ? Nums[GetIndByXY(x, y)] : 0.f) + ((y < mat.SizeY and x < mat.SizeX) ? mat.Nums[mat.GetIndByXY(x, y)] : 0.f);
			}
		}
		return retMat;
	}
}
Matrix Matrix::operator+(float num) {
	Matrix retMat(SizeX, SizeY);
	for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] + num;
	return retMat;
}

Matrix Matrix::operator-(const Matrix& mat) {
	if (mat.SizeX == SizeX and mat.SizeY == SizeY) {
		Matrix retMat(SizeX, SizeY);
		for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] - mat.Nums[i];
		return retMat;
	}
	else {//expand to big matrix
		Matrix retMat(SizeX > mat.SizeX ? SizeX : mat.SizeX, SizeY > mat.SizeY ? SizeY : mat.SizeY);
		for (unsigned int y = 0; y < retMat.SizeY; y++) {
			for (unsigned int x = 0; x < retMat.SizeX; x++) {
				retMat.Nums[retMat.GetIndByXY(x, y)] =
					((y < SizeY and x < SizeX) ? Nums[GetIndByXY(x, y)] : 0.f) - ((y < mat.SizeY and x < mat.SizeX) ? mat.Nums[mat.GetIndByXY(x, y)] : 0.f);
			}
		}
		return retMat;
	}
}
Matrix Matrix::operator-(float num) {
	Matrix retMat(SizeX, SizeY);
	for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] - num;
	return retMat;
}

Matrix Matrix::operator*(const Matrix& mat) {
	if (SizeX == mat.SizeY) {
		Matrix retMat(mat.SizeX, SizeY);
		for (unsigned int y = 0; y < retMat.SizeY; y++) {
			for (unsigned int x = 0; x < retMat.SizeX; x++) {
				float sum = 0.f;
				for (unsigned of = 0; of < SizeX; of++) {
					sum += Nums[GetIndByXY(of, y)] * mat.Nums[GetIndByXY(x, of)];
				}
				retMat.Nums[GetIndByXY(x, y)] = sum;
			}
		}
		return retMat;
	}
	__debugbreak();//wrong matrixes size, cant multiply
	return Matrix();
}
Matrix Matrix::operator*(float num) {
	Matrix retMat(SizeX, SizeY);
	for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] * num;
	return retMat;
}

Matrix Matrix::operator*(const Vector2& vec) {
	return operator*(Matrix(1, 2, { vec.gX(), vec.gY() }));
}