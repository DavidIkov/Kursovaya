#pragma once
#include"Vector2.h"
///stands for SCALE or PIXEL COORDINATE SYSTEM
class SPCS {
	Vector2 Scale = Vector2(1.f);
	Vector2 Pixel = Vector2(0.f);
public:
	SPCS() {

	}
	SPCS(float nsX, float nsY, float npX, float npY) {
		Scale.sX(nsX);
		Scale.sY(nsY);
		Pixel.sX(npX);
		Pixel.sY(npY);
	}

	const Vector2& gSV() const { return Scale; }
	const Vector2& gPV() const { return Pixel; }

	void sSX(float nsX) { Scale.sX(nsX); }
	void sSY(float nsY) { Scale.sY(nsY); }
	void sPX(float npX) { Pixel.sX(npX); }
	void sPY(float npY) { Pixel.sY(npY); }

	float gSX() const { return Scale.gX(); }
	float gSY() const { return Scale.gY(); }
	float gPX() const { return Pixel.gX(); }
	float gPY() const { return Pixel.gY(); }
};