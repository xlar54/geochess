//
//	This file was generated by the GEOS Resource Compiler
//
//	DO NOT EDIT! Any changes will be lost!
//
//	Edit proper resource file instead.
//

void newgame (void);
void switch4080 (void);
void EnterDeskTop (void);

const void subMenu1 = {
	(char)12, (char)54,
	(int)0, (int)66,
	(char)(3 | VERTICAL),
	"new game", (char)MENU_ACTION, (int)newgame,
	"switch 40/80", (char)MENU_ACTION, (int)switch4080,
	"quit", (char)MENU_ACTION, (int)EnterDeskTop,
};

void set40col (void);
void set80col (void);

const void subMenu2 = {
	(char)16, (char)44,
	(int)25, (int)60,
	(char)(2 | VERTICAL),
	"40 col", (char)MENU_ACTION, (int)set40col,
	"80 col", (char)MENU_ACTION, (int)set80col,
};


const void mainMenu = {
	(char)0, (char)15,
	(int)0, (int)58,
	(char)(2 | HORIZONTAL),
	"geos", (char)SUB_MENU, (int)&subMenu1,
	"width", (char)SUB_MENU, (int)&subMenu2,
};

