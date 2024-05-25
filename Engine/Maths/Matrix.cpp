#include"Matrix.h"
#include"Vector2.h"
#include"Vector3.h"



Matrix Matrix::FromAnglesXYZ(float xr, float yr, float zr) {
	Vector3 rv(1, 0, 0);
	Vector3 tv(0, 1, 0);
	Vector3 fv(0, 0, 1);

	Vector3::RotateVectors(fv, tv, xr, fv, tv);
	Vector3::RotateVectors(rv, fv, yr, rv, fv);
	Vector3::RotateVectors(rv, tv, zr, rv, tv);

	return Matrix(3, { &rv,&tv,&fv });
}

float Matrix::gDeterminant() const {
	if (NeedToUpdateDeterminant) UpdateDeterminant();
	return Determinant;
}
Matrix Matrix::gReversedMatrix() const {
	if (NeedToUpdateReversedMatrix) UpdateReversedMatrix();
	return *ReversedMatrix;
}
const Matrix& Matrix::grReversedMatrix() const {
	if (NeedToUpdateReversedMatrix) UpdateReversedMatrix();
	return *ReversedMatrix;
}

void Matrix::UpdateDeterminant() const {
	NeedToUpdateDeterminant = false;
	Determinant = 0.f;

	if (SizeX != SizeY) {
#if defined _DEBUG
		__debugbreak();//determinant works only for square matrixes
#endif
		return;
	}

	if (SizeX == 1) {
		Determinant = Nums[0];
		return;
	}

	float* tempNums = new float[(SizeX - 1) * (SizeY - 1)];
	int multiplier = 1;
	for (unsigned int ai = 0; ai < SizeX; ai++) {
		for (unsigned int x = 0; x < SizeX; x++) {
			if (x == ai) continue;
			for (unsigned int y = 1; y < SizeY; y++) {
				unsigned int insertInd = y - 1 + (SizeY - 1) * (x > ai ? (x - 1) : x);
				tempNums[insertInd] = Nums[y + x * SizeY];
			}
		}
		Determinant += Nums[ai * SizeY] * multiplier * Matrix(SizeX - 1, SizeX - 1, tempNums).gDeterminant();
		multiplier = -multiplier;
	}
	delete[] tempNums;
}
void Matrix::UpdateReversedMatrix() const {
	NeedToUpdateReversedMatrix = false;
	if (NeedToUpdateDeterminant) UpdateDeterminant();

	if (ReversedMatrix == nullptr) ReversedMatrix = new Matrix(SizeX,SizeY);

	float* tempNums = new float[(SizeX - 1) * (SizeY - 1)];
	for (unsigned int x = 0; x < SizeX; x++) {
		for (unsigned int y = 0; y < SizeY; y++) {
			for (unsigned int lx = 0; lx < SizeX; lx++) {
				if (lx == y) continue;
				for (unsigned int ly = 0; ly < SizeY; ly++) {
					if (ly == x) continue;
					tempNums[((lx > y) ? (lx - 1) : lx) * (SizeX - 1) + ((ly > x) ? (ly - 1) : ly)] = Nums[ly + lx * SizeY];
				}
			}
			int multipler = ((x + y) % 2 == 0) ? 1 : -1;
			ReversedMatrix->Nums[y + x * SizeY] = multipler * Matrix(SizeX - 1, SizeY - 1, tempNums).gDeterminant() / Determinant;
			multipler = -multipler;
		}
	}
}


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
Matrix::Matrix(unsigned int sx, unsigned int sy, const float nums[]) {
	SizeX = sx;
	SizeY = sy;
	Nums = new float[SizeX * SizeY];
	for (unsigned int i = 0; i < SizeX * SizeY; i++) Nums[i] = nums[i];
}
Matrix::Matrix(unsigned int l, const Vector3* (&& nums)[]) {
	SizeX = l;
	SizeY = 3;
	Nums = new float[SizeX * SizeY];
	for (unsigned int i = 0; i < SizeX; i++) {
		Nums[i * SizeX] = nums[i]->gX();
		Nums[i * SizeX + 1] = nums[i]->gY();
		Nums[i * SizeX + 2] = nums[i]->gZ();
	}
}
Matrix::Matrix(unsigned int l, const Vector2* (&& nums)[]) {
	SizeX = l;
	SizeY = 2;
	Nums = new float[SizeX * SizeY];
	for (unsigned int i = 0; i < SizeX; i++) {
		Nums[i * SizeX] = nums[i]->gX();
		Nums[i * SizeX + 1] = nums[i]->gY();
	}
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
	NeedToUpdateDeterminant = true;
	NeedToUpdateReversedMatrix = true;
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
Matrix Matrix::operator+(const Matrix& mat) const {
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
Matrix Matrix::operator+(float num) const {
	Matrix retMat(SizeX, SizeY);
	for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] + num;
	return retMat;
}

Matrix Matrix::operator-(const Matrix& mat) const {
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
Matrix Matrix::operator-(float num) const {
	Matrix retMat(SizeX, SizeY);
	for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] - num;
	return retMat;
}
Matrix Matrix::operator*(const Matrix& mat) const {
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
Matrix Matrix::operator*(float num) const {
	Matrix retMat(SizeX, SizeY);
	for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] * num;
	return retMat;
}

Matrix Matrix::operator*(const Vector2& vec) const {
	return operator*(Matrix(1, 2, { vec.gX(), vec.gY() }));
}
Matrix Matrix::operator*(const Vector3& vec) const {
	return operator*(Matrix(1, 3, { vec.gX(), vec.gY(), vec.gZ() }));
}