#include"TranslatorFrom3D.h"

const std::type_index TranslatorFrom3D::TypeIndex = std::type_index(typeid(TranslatorFrom3D));

TranslatorFrom3D* TranslatorFrom3D::gThis() {
	return this;
}
TranslatorFrom3D::TranslatorFrom3D(ClassesMap* mapPtr) {
	instancePtr = mapPtr;
}