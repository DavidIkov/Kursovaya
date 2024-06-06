#include"InputController.h"
#include<iostream>

bool InputController::gPressableKeyState(PressableKeys key) const {
	return PressableKeysStates[(int)key];
}

void InputController::KeyCallback(int key, int scancode, int action, int mods)
{
	if (key >= GLFW_KEY_A and key <= GLFW_KEY_Z) {
		unsigned int ind = key - GLFW_KEY_A;
		PressableKeysStates[ind] = action != GLFW_RELEASE;
	}
}