#include "TextFormat.h"

char BASE_CHARACTER[2];
char BLACK[4];
char DARK_BLUE[4];
char DARK_GREEN[4];
char DARK_AQUA[4];
char DARK_RED[4];
char DARK_PURPLE[4];
char GOLD[4];
char GRAY[4];
char DARK_GRAY[4];
char BLUE[4];
char GREEN[4];
char AQUA[4];
char RED[4];
char LIGHT_PURPLE[4];
char YELLOW[4];
char WHITE[4];

char OBFUSCATED[4];
char BOLD[4];
char STRIKETHROUGH[4];
char UNDERLINE[4];
char ITALIC[4];
char RESET[4];

void initColor(char* variable, char color) {
	variable[0] = BASE_CHARACTER[0];
	variable[1] = BASE_CHARACTER[1];
	variable[2] = color;
	variable[3] = 0x0;
}

void init() {
	BASE_CHARACTER[0] = (char)(0xC2);
	BASE_CHARACTER[1] = (char)(0xA7);
	initColor(BLACK, '0');
	initColor(DARK_BLUE, '1');
	initColor(DARK_GREEN, '2');
	initColor(DARK_AQUA, '3');
	initColor(DARK_RED, '4');
	initColor(DARK_PURPLE, '5');
	initColor(GOLD, '6');
	initColor(GRAY, '7');
	initColor(DARK_GRAY, '8');
	initColor(BLUE, '9');
	initColor(GREEN, 'a');
	initColor(AQUA, 'b');
	initColor(RED, 'c');
	initColor(LIGHT_PURPLE, 'd');
	initColor(YELLOW, 'e');
	initColor(WHITE, 'f');

	initColor(OBFUSCATED, 'k');
	initColor(BOLD, 'l');
	initColor(STRIKETHROUGH, 'm');
	initColor(UNDERLINE, 'n');
	initColor(ITALIC, 'o');
	initColor(RESET, 'r');
}
