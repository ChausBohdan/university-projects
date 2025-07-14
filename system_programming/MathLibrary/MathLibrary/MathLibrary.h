#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include "pch.h"
#include <string>
#include <vector>

using namespace std;
extern "C" __declspec(dllexport) char* DeletedOddLetters(const char* lhs);
