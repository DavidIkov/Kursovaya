#include"SPCS.h"

SPCS::SPCS() {

}
SPCS::SPCS(float nsX, float nsY, float npX, float npY) {
	Scale.sX(nsX);
	Scale.sY(nsY);
	Pixel.sX(npX);
	Pixel.sY(npY);
}

Vector2 SPCS::gSV() const { return Scale; }
const Vector2& SPCS::grSV() const { return Scale; }
Vector2 SPCS::gPV() const { return Pixel; }
const Vector2& SPCS::grPV() const { return Pixel; }

void SPCS::sSX(float nsX) { Scale.sX(nsX); }
void SPCS::sSY(float nsY) { Scale.sY(nsY); }
void SPCS::sPX(float npX) { Pixel.sX(npX); }
void SPCS::sPY(float npY) { Pixel.sY(npY); }

float SPCS::gSX() const { return Scale.gX(); }
float SPCS::gSY() const { return Scale.gY(); }
float SPCS::gPX() const { return Pixel.gX(); }
float SPCS::gPY() const { return Pixel.gY(); }