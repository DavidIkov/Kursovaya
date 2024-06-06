#include"Instance.h"
const std::type_index Instance::TypeIndex = std::type_index(typeid(Instance));


void Instance::recursive_AddTo_AllChildrenAmount(int add) {
	AllChildrenAmount += add;
	if (Parent != nullptr)
		Parent->FindTypePtr<Instance>(TypeIndex)->recursive_AddTo_AllChildrenAmount(add);
}

void Instance::deleteInChildrenOfParent() {
	Instance* parentInst = Parent->FindTypePtr<Instance>(TypeIndex);
	auto& parentChildren = parentInst->Children;
	for (unsigned int ci = 0; ci < parentChildren.size(); ci++) {
		if (parentChildren[ci] == instancePtr) {
			parentChildren.erase(parentChildren.begin() + ci);
			parentInst->recursive_AddTo_AllChildrenAmount(-1);
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


void Instance::ExpandChildrenAmount(unsigned int expandAmount) {
	Children.reserve(Children.size() + expandAmount);
	ChildrenExpansionAmount = expandAmount;
}



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

	Instance* newParInst = newPar->FindTypePtr<Instance>(TypeIndex);

	
	ParentChange.FireBeforeEventFunctions(newPar);
	ClassesMap* prevPar = Parent;
	Parent = newPar;
	newParInst->Children.push_back(this->instancePtr);
	newParInst->recursive_AddTo_AllChildrenAmount(1);
	ParentChange.FireAfterEventFunctions(prevPar);

}
ClassesMap* Instance::gParent() const { return Parent; }

void Instance::ClearAllChildren() {
	deleteChildren();
	Children = std::vector<ClassesMap*>();
}
const std::vector<ClassesMap*>& Instance::gChildren() const { return Children; }