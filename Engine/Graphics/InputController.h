#pragma once
#include"DLLStuff.h"
#include "Dependencies/GLFW/include/GLFW/glfw3.h"

enum class PressableKeys {
	A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,

	ENUMEND
};

class InputController {
	friend class Window;

	bool PressableKeysStates[(unsigned int)PressableKeys::ENUMEND - 1];

public:
	void KeyCallback(int key, int scancode, int action, int mods);

	DLL_TREATMENT bool gPressableKeyState(PressableKeys key) const;

};
