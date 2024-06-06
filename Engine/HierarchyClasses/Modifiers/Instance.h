#pragma once
#include"DLLStuff.h"
#include<string>
#include<vector>
#include<unordered_map>
#include<cstddef>
#include"UsefullStuff/ClassEvent.h"
#include"ClassesMap.h"
#include<typeindex>

class Instance {
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
	friend class Object2DData;
	friend class TranslatorFrom2D;
	friend class Object3DData;
	friend class TranslatorFrom3D;

	std::unordered_map<std::string, std::byte*> InheritancePointers;

	unsigned int ChildrenExpansionAmount = 0;

	unsigned int AllChildrenAmount = 0;

	std::vector<ClassesMap*> Children;
	ClassesMap* Parent = nullptr;
	void deleteInChildrenOfParent();
	void deleteChildren();
	void continousDelete();
	void recursive_AddTo_AllChildrenAmount(int add);

public:
	static const std::type_index TypeIndex;

	Instance* gThis() {
		return this;
	}

	DSEvent<ClassesMap*> ParentChange;
	std::string Name;

	DLL_TREATMENT Instance(ClassesMap* nPtr);

	/*resize children vector, should be used when you want to add a lot of children instantly*/
	DLL_TREATMENT void ExpandChildrenAmount(unsigned int expandAmount);


	DLL_TREATMENT void Delete();

	DLL_TREATMENT ClassesMap* FindFirstParentWithModifier(const std::type_index& f_TypeIndex) const {
		if (Parent == nullptr) return nullptr;
		if (Parent->IsContains(f_TypeIndex)) return Parent;
		else return Parent->FindTypePtr<Instance>(TypeIndex)->FindFirstParentWithModifier(f_TypeIndex);
	}


	DLL_TREATMENT void GetAllChildrenWithModifier(const std::type_index& f_TypeIndex, std::vector<ClassesMap*>& f_AllChildrenStorage) const;
	DLL_TREATMENT void GetAllChildrenWithModifier(const std::type_index& f_TypeIndex, unsigned int f_depth, std::vector<ClassesMap*>& f_AllChildrenStorage) const;

	DLL_TREATMENT void GetChildrenWithModifier(const std::type_index f_TypeIndex, std::vector<ClassesMap*>& f_ChildrenStorage) const;

	
	DLL_TREATMENT unsigned int gChildrenAmount() const;
	
	DLL_TREATMENT void sParent(ClassesMap* newPar);
	DLL_TREATMENT ClassesMap* gParent() const;

	DLL_TREATMENT void ClearAllChildren();
	DLL_TREATMENT const std::vector<ClassesMap*>& gChildren() const;
};