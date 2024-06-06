#include"SPCS.h"

SPCS::SPCS() {

}
SPCS::SPCS(float nsX, float nsY, float npX, float npY) {
	Scale.sX(nsX);
	Scale.sY(nsY);
	Pixel.sX(npX);
	Pixel.sY(npY);
}

const Vector2& SPCS::gSV() const { return Scale; }
const Vector2& SPCS::gPV() const { return Pixel; }

void SPCS::sSX(float nsX) { Scale.sX(nsX); }
void SPCS::sSY(float nsY) { Scale.sY(nsY); }
void SPCS::sPX(float npX) { Pixel.sX(npX); }
void SPCS::sPY(float npY) { Pixel.sY(npY); }

float SPCS::gSX() const { return Scale.gX(); }
float SPCS::gSY() const { return Scale.gY(); }
float SPCS::gPX() const { return Pixel.gX(); }
float SPCS::gPY() const { return Pixel.gY(); }


SPCS SPCS::operator+(const SPCS& cords) const {
	return { Scale.gX() + cords.Scale.gX(),Scale.gY() + cords.Scale.gY(),Pixel.gX() + cords.Pixel.gX(),Pixel.gY() + cords.Pixel.gY() };
}
SPCS SPCS::operator-(const SPCS& cords) const {
	return { Scale.gX() - cords.Scale.gX(),Scale.gY() - cords.Scale.gY(),Pixel.gX() - cords.Pixel.gX(),Pixel.gY() - cords.Pixel.gY() };
}