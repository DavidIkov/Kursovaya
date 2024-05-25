#pragma once
#include"DLLStuff.h"
#include<functional>
#include<vector>

template<typename...>
class DSEvent;

template<typename...data>
class Event {
	friend DSEvent;
	std::vector<std::function<void(data...)>> ConnectedFuncs;

public:

	DLL_TREATMENT auto Connect(std::function<void(data...)> func) {
		ConnectedFuncs.push_back(func);
		return func;
	}
	DLL_TREATMENT void Remove(std::function<void(data...)> func) {
		for (unsigned int i = 0; i < ConnectedFuncs.size(); i++) {
			if (ConnectedFuncs[i] == func) {
				ConnectedFuncs.erase(ConnectedFuncs.begin() + i);
				return;
			}
		}
		__debugbreak();//not found function to delete, maybe its already deleted?
	}
	DLL_TREATMENT void FireEvent(data... fireData) {
		for (unsigned int i = 0; i < ConnectedFuncs.size(); i++) {
			ConnectedFuncs[i](fireData...);
		}
	}
};


template<typename...data>
//double sided event, means that you can connect before and after event
class DSEvent {
	Event<data...> EventBefore;
	Event<data...> EventAfter;

public:

	DLL_TREATMENT auto ConnectBeforeEvent(std::function<void(data...)> func) {
		return EventBefore.Connect(func);
	}
	DLL_TREATMENT auto ConnectAfterEvent(std::function<void(data...)> func) {
		return EventAfter.Connect(func);
	}

	DLL_TREATMENT void Remove(std::function<void(data...)> func) {
		for (unsigned int i = 0; i < EventBefore.ConnectedFuncs.size(); i++) {
			if (EventBefore.ConnectedFuncs[i] == func) {
				EventBefore.ConnectedFuncs.erase(EventBefore.ConnectedFuncs.begin() + i);
				return;
			}
		}
		for (unsigned int i = 0; i < EventAfter.ConnectedFuncs.size(); i++) {
			if (EventAfter.ConnectedFuncs[i] == func) {
				EventAfter.ConnectedFuncs.erase(EventAfter.ConnectedFuncs.begin() + i);
				return;
			}
		}
		__debugbreak();//not found function to delete, maybe its already deleted?
	}
	DLL_TREATMENT void FireBeforeEventFunctions(data... fireData) {
		for (unsigned int i = 0; i < EventBefore.ConnectedFuncs.size(); i++) {
			EventBefore.ConnectedFuncs[i](fireData...);
		}
	}
	DLL_TREATMENT void FireAfterEventFunctions(data... fireData) {
		for (unsigned int i = 0; i < EventAfter.ConnectedFuncs.size(); i++) {
			EventAfter.ConnectedFuncs[i](fireData...);
		}
	}
};