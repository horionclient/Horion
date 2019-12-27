#pragma once

class C_HIDController {
private:
	char pad_0x0000[0x50];  //0x0000
public:
	union {
		struct {
			bool leftClickDown;   //0x0050
			bool rightClickDown;  //0x0051
			bool wheelDown;       //0x0052
			bool mouse4Down;      //0x0053
			bool mouse5Down;      //0x0054
		};
		bool clickMap[5];
	};
};
