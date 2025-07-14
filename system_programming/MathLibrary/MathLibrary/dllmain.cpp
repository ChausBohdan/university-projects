// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#define WIN32_LEAN_AND_MEAN

#include "pch.h"
#include "MathLibrary.h"
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) char* DeletedOddLetters(const char* lhs)
{
    char* oddCharacters = (char*)malloc(strlen(lhs) + 1); // Виділяємо пам'ять для результуючого рядка
    int counter = 0;
    int j = 0; // Індекс для результуючого рядка

    for (int i = 0; i < strlen(lhs); i++) {
        if (isalpha(lhs[i])) {
            if (counter == 1) {
                counter = 0;
                oddCharacters[j++] = lhs[i];
            }
            else {
                counter = 1;
            }
        }
        else {
            oddCharacters[j++] = lhs[i];
            counter = 0;
        }
    }

    oddCharacters[j] = '\0'; // Додаємо завершуючий нуль

    return oddCharacters;
}


