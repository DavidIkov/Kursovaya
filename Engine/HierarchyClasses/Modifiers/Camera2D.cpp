#include"Camera2D.h"

const std::type_index Camera2D::TypeIndex = std::type_index(typeid(Camera2D));

Camera2D* Camera2D::gThis() {
	return this;
}
Camera2D::Camera2D(ClassesMap* mapPtr) {
	instancePtr = mapPtr;
}