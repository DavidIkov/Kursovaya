#include"Instance.h"
const std::type_index Instance::TypeIndex = std::type_index(typeid(Instance));

void Instance::deleteInChildrenOfParent() {
	auto& parentChildren = Parent->FindTypePtr<Instance>(TypeIndex)->Children;
	for (unsigned int ci = 0; ci < parentChildren.size(); ci++) {
		if (parentChildren[ci] == instancePtr) {
			parentChildren.erase(parentChildren.begin() + ci);
			return;
		}
	}
}
void Instance::deleteChildren() {
	for (unsigned int ci = 0; ci < Children.size(); ci++) {
		Children[ci]->FindTypePtr<Instance>(TypeIndex)->continousDelete();
	}
}
void Instance::continousDelete() {
	deleteChildren();
	
	//if your instance dont have parent then its have to not be casted with "new" so it wont be a pointer and can get deleted at end of scope
	if (Parent != nullptr) instancePtr->ClassDeleteFunction();
}

Instance::Instance(ClassesMap* nPtr) {
	instancePtr = nPtr;
};






void Instance::Delete() {
	if (Parent != nullptr) deleteInChildrenOfParent();
	continousDelete();
}



void Instance::GetAllChildrenWithModifier(const std::type_index& f_TypeIndex, std::vector<ClassesMap*>& f_AllChildrenStorage) const {
	for (unsigned int i = 0; i < Children.size(); i++) {
		if (Children[i]->IsContains(f_TypeIndex)) f_AllChildrenStorage.push_back(Children[i]);
		Children[i]->FindTypePtr<Instance>(TypeIndex)->GetAllChildrenWithModifier(f_TypeIndex, f_AllChildrenStorage);
	}
}
void Instance::GetAllChildrenWithModifier(const std::type_index& f_TypeIndex, unsigned int f_depth, std::vector<ClassesMap*>& f_AllChildrenStorage) const {
	if (f_depth == 0) return;
	for (unsigned int i = 0; i < Children.size(); i++) {
		unsigned int curDepth = f_depth;
		if (Children[i]->IsContains(f_TypeIndex)) {
			f_AllChildrenStorage.push_back(Children[i]);
			curDepth--;
		}
		Children[i]->FindTypePtr<Instance>(TypeIndex)->GetAllChildrenWithModifier(f_TypeIndex, curDepth, f_AllChildrenStorage);
	}
}


void Instance::GetChildrenWithModifier(const std::type_index f_TypeIndex, std::vector<ClassesMap*>& f_ChildrenStorage) const {
	for (unsigned int i = 0; i < Children.size(); i++) {
		if (Children[i]->IsContains(f_TypeIndex)) {
			f_ChildrenStorage.push_back(Children[i]);
		}
	}
}


unsigned int Instance::gChildrenAmount() const { return Children.size(); }


void Instance::sParent(ClassesMap* newPar) {
	if (Parent != nullptr) deleteInChildrenOfParent();
	newPar->FindTypePtr<Instance>(TypeIndex)->Children.push_back(this->instancePtr);

	ParentChange.FireBeforeEventFunctions(newPar);
	ClassesMap* prevPar = Parent;
	Parent = newPar;
	ParentChange.FireAfterEventFunctions(prevPar);

}
ClassesMap* Instance::gParent() const { return Parent; }

void Instance::ClearAllChildren() {
	deleteChildren();
	Children = std::vector<ClassesMap*>();
}
const std::vector<ClassesMap*>& Instance::gChildren() { return Children; }