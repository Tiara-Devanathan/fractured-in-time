#pragma once
#include <string>
#include <windows.h>
using namespace std;

class video {
public:
    HWND hWnd;
    void display(string s, HWND hwnd);
    void run();
    void clearup();
    bool init(string t, HWND hwnd);
};
