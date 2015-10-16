#pragma once

#ifdef CLASSGAME_EXPORTS
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif

#pragma warning(disable:4251)
