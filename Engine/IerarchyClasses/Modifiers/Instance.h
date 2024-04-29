#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<cstddef>
#include"../UsefullStuff/ClassEvent.h"
#include"ClassesMap.h"

class Instance2DData; 

class Instance {
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
	friend class Instance2DData;


protected:
	
	std::unordered_map<std::string, std::byte*> InheritancePointers;

	std::vector<ClassesMap*> Children;
	ClassesMap* Parent = nullptr;
	void deleteInChildrenOfParent();
	void deleteChildren();
	void continousDelete();
	
	

public:
	static const std::type_index TypeIndex;

	Instance* gThis() {
		return this;
	}

	DSEvent<ClassesMap*> ParentChange;
	std::string Name;

	Instance(ClassesMap* nPtr);

	

	void Delete();

	ClassesMap* FindFirstParentWithModifier(const std::type_index& f_TypeIndex) const {
		if (Parent == nullptr) return nullptr;
		if (Parent->IsContains(f_TypeIndex)) return Parent;
		else return Parent->FindTypePtr<Instance>(TypeIndex)->FindFirstParentWithModifier(f_TypeIndex);
	}


	void GetAllChildrenWithModifier(const std::type_index& f_TypeIndex, std::vector<ClassesMap*>& f_AllChildrenStorage) const;
	void GetAllChildrenWithModifier(const std::type_index& f_TypeIndex, unsigned int f_depth, std::vector<ClassesMap*>& f_AllChildrenStorage) const;

	void GetChildrenWithModifier(const std::type_index f_TypeIndex, std::vector<ClassesMap*>& f_ChildrenStorage) const;

	
	unsigned int gChildrenAmount() const;
	
	void sParent(ClassesMap* newPar);
	ClassesMap* gParent() const;

	void ClearAllChildren();
	const std::vector<ClassesMap*>& gChildren();
};