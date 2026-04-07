#ifndef DEFS_H
#define DEFS_H
#define _CRT_SECURE_NO_WARNINGS
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define STRCPY(dest, src)             \
	strncpy(dest, src, sizeof(dest)); \
	dest[sizeof(dest) - 1] = '\0'
#define STRNCPY(dest, src, n) \
	strncpy(dest, src, n);    \
	dest[n - 1] = '\0'

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define LOGIC_RATE (FPS / 1000)

#define MAX_FILENAME_LENGTH 256
#define MAX_NAME_LENGTH		32

#define MAX_KEYBOARD_KEYS 350

enum
{
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};

#endif // !DEFS_H
