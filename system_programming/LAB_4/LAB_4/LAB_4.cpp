#include "MathLibrary.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <chrono>

using namespace std;

int main() {
    cout << "Input your sentence: \n";
    string str = "The burgers are beatiful";
    getline(cin, str);
    //auto start = chrono::high_resolution_clock::now(); // ��������� ���� ����

    cout << "Changed letters: " << DeletedOddLetters(str.c_str()) << endl;

    //auto end = chrono::high_resolution_clock::now(); // ʳ����� ���� ����

    //chrono::duration<double> duration = end - start; // ���������� ��������� ���������

    //cout << "Execution time: " << duration.count() << " seconds" << endl;
    _getch();

    return 0;
}

