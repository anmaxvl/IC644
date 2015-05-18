#include "types.h"

void drawBox (float lenX, float lenY, float lenZ);

void mousePressHandler (int button, int state, int x, int y);

void keyboardPressHandler(unsigned char key, int x, int y);

void mouseMoveHandler (int x, int y);

void keyBoardFunction(unsigned char key, int x, int y);

Image * loadTexture(char *filename);

int ImageLoad(char *filename, Image *image);

void specialKeyFunction(int key, int x, int y);