#include"Camera3D.h"

const std::type_index Camera3D::TypeIndex = std::type_index(typeid(Camera3D));

Camera3D* Camera3D::gThis() {
	return this;
}
Camera3D::Camera3D(ClassesMap* mapPtr) {
	instancePtr = mapPtr;
}