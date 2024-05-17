#pragma once
#if defined DLL
#define DLL_TREATMENT __declspec(dllexport)
#else
#define DLL_TREATMENT __declspec(dllimport)
#endif