#pragma once
#include<vector>
typedef unsigned int IDBasedVectorElementID;
template<typename Type>
class IDBasedVector {
	std::vector<Type> Vector;
	std::vector<bool> Slots;
	unsigned int FreeMemorySlots = 0;
public:
	IDBasedVectorElementID Insert(const Type& toInsert) {
		if (FreeMemorySlots == 0) {
			Vector.push_back(toInsert);
			Slots.push_back(true);
			return Vector.size() - 1;
		}
		else {
			for (unsigned int i = 0; i < Vector.size(); i++) {
				if (not Slots[i]) {
					Vector[i] = toInsert;
					FreeMemorySlots--;
					Slots[i] = true;
					return i;
				}
			}
		}
		__debugbreak();//very weird bug idk honesty, go to debugging hell
		return -1;
	}
	void Remove(IDBasedVectorElementID id) {

#if defined _DEBUG
		if (id >= Vector.size()) __debugbreak();//out of bounds
#endif
		if (not Slots[id]) return;

		Slots[id] = false;
		FreeMemorySlots++;
	}

	bool Contains(IDBasedVectorElementID id) {
		if (id >= Vector.size() or not Slots[id]) return false;
		return true;
	}
	const Type& operator[](IDBasedVectorElementID id) {
		if (not Slots[id]) __debugbreak();//empty slot
		return Vector[id];
	}
};