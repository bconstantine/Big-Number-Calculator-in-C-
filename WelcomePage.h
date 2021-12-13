#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

using namespace std;

void printWelcomePage();
