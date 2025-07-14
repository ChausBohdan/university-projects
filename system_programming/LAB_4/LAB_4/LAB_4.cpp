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
    //auto start = chrono::high_resolution_clock::now(); // Початкова мітка часу

    cout << "Changed letters: " << DeletedOddLetters(str.c_str()) << endl;

    //auto end = chrono::high_resolution_clock::now(); // Кінцева мітка часу

    //chrono::duration<double> duration = end - start; // Обчислення тривалості виконання

    //cout << "Execution time: " << duration.count() << " seconds" << endl;
    _getch();

    return 0;
}

