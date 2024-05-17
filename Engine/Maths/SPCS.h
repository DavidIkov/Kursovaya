#pragma once
#include"DLLStuff.h"
#include"Vector2.h"

///stands for SCALE or PIXEL COORDINATE SYSTEM
class SPCS {
	Vector2 Scale;
	Vector2 Pixel;
public:
	DLL_TREATMENT SPCS();
	DLL_TREATMENT SPCS(float nsX, float nsY, float npX, float npY);

	DLL_TREATMENT Vector2 gSV() const;
	DLL_TREATMENT const Vector2& grSV() const;
	DLL_TREATMENT Vector2 gPV() const;
	DLL_TREATMENT const Vector2& grPV() const;

	DLL_TREATMENT void sSX(float nsX);
	DLL_TREATMENT void sSY(float nsY);
	DLL_TREATMENT void sPX(float npX);
	DLL_TREATMENT void sPY(float npY);

	DLL_TREATMENT float gSX() const;
	DLL_TREATMENT float gSY() const;
	DLL_TREATMENT float gPX() const;
	DLL_TREATMENT float gPY() const;
};