//Eilon Yanko 318879277

#ifndef UTILITIES_H
#define UTILITIES_H
#define CIPHER_KEY 'E'
#define DELIMITER ','

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <assert.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "log.h"

char* dateToStr(time_t d, char* buffer, size_t bufsize);
time_t strToDate(char* strDate);
time_t strToDateTime(char* strDate);
void cleanBuffer();
void printFixed(char* s, int width);
int countLinesInTxtFiles(FILE* fp);
void clearScreen();
void pressKeyToContinue();
void XORCipher(char* password);
void toLowerCase(char* dest, const char* src);

#endif