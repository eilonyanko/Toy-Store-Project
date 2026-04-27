//Eilon Yanko 318879277

#ifndef EMPLOEES_H
#define EMPLOEES_H

#define _CRT_SECURE_NO_WARNINGS

#include "utilities.h"

#define USERNAME_SIZE 20
#define NAME_SIZE 40
#define PASSWORD_SIZE 20

typedef struct employee {
	char username[USERNAME_SIZE];
	char name[NAME_SIZE];
	char password[PASSWORD_SIZE];
	int permissionLvl;
	bool isActive;
}Employee;

int loadEmploeesFromFile(Employee** emploees, const char* filename);
void saveEmploeesToFile(Employee* emploees, char* fileName, int numOfEmploees);
void printEmploeesList(Employee* emploees, int numOfEmploees);
int login(Employee* emploees, int numOfEmploees);
int addUser(Employee** emploees, int numOfEmploees);
void deleteUser(Employee* emploees, int numOfEmploees);
void editUser(Employee* emploees, int numOfEmploees);


#endif