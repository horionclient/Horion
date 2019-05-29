#pragma once
#include<Windows.h>

class C_WindowsUI {
public:
	uintptr_t** vTable; //0x0000
	char pad_0x0008[0x578]; //0x0008
	int MouseX; //0x0580 // In screen coordinates
	int MouseY; //0x0584 
};

