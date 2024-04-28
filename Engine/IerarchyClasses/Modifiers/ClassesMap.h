#pragma once
#include<unordered_map>
#include<typeinfo>
#include<typeindex>
#include<functional>
#define ClassesMapClassDeleteFunction [this]() {delete this; }
#define ClassesMapType std::unordered_map<std::type_index,void*>

struct ClassesMap {

	

	template<typename curT, typename...otherT>
	static ClassesMapType* CreateMap(ClassesMapType* mapPtr, curT* ptr, otherT*...otherPtrs) {
		if (mapPtr == nullptr) mapPtr = new ClassesMapType;
		mapPtr->insert({ std::type_index(typeid(curT)),(void*)ptr });
		if constexpr (sizeof...(otherT) != 0) CreateMap<otherT...>(mapPtr, otherPtrs...);
		return mapPtr;
	}

	std::function<void()> ClassDeleteFunction;
	ClassesMapType* MapPtr;


	bool IsContains(const std::type_index& typInd) {
		return MapPtr->contains(typInd);
	}

	template<typename typ>
	typ* FindTypePtr(const std::type_index& typInd) {
		auto foundPair = MapPtr->find(typInd);
		if (foundPair == MapPtr->end()) return nullptr;
		return (typ*)(foundPair->second);
	}

	ClassesMap(const std::function<void()>&& delFunc, ClassesMapType* ptr) {
		ClassDeleteFunction = delFunc;
		MapPtr = ptr;
	}
	~ClassesMap() {
		delete MapPtr;
	}
};

#undef ClassesMapType