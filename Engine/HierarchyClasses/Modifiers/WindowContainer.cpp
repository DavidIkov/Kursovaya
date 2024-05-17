#include"WindowContainer.h"
#include"Graphics/WindowsManager.h"

const std::type_index WindowContainer::TypeIndex = std::type_index(typeid(WindowContainer));

WindowContainer* WindowContainer::gThis() {
	return this;
}
WindowContainer::WindowContainer(ClassesMap* mapPtr, Window* windPtr) {
	InstancePtr = mapPtr;
	WindowPtr = windPtr;
}