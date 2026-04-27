//Eilon Yanko 318879277

#include "emploees.h"

int loadEmploeesFromFile(Employee** emploees, const char* filename)
{
	FILE* fp = fopen(filename, "r");
	Employee* temp = NULL;
	int numOfEmploees;
	if (!fp)
	{
		printf("Failed to open employees file! Creating admin user\n\n");
		LOG_ERROR("Failed to open employees file! Creating admin user");

		temp = (Employee*)malloc(sizeof(Employee));

		strncpy(temp->username, "admin", USERNAME_SIZE);
		strncpy(temp->name, "manager", NAME_SIZE);
		strncpy(temp->password, "12345678", PASSWORD_SIZE);
		temp->permissionLvl = 1;
		temp->isActive = true;

		*emploees = temp;
		Sleep(2000);
		clearScreen();
		return 1;
	}

	else
	{
		numOfEmploees = countLinesInTxtFiles(fp);
		temp = (Employee*)malloc(sizeof(Employee)*numOfEmploees);
		int count = 0;
		char line[256];

		rewind(fp);
		
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			// skip empty lines
			if (line[0] == '\n' || line[0] == '\r') continue;

			// remove trailing newline
			line[strcspn(line, "\r\n")] = '\0';

			int perm;
			char username[USERNAME_SIZE];
			char name[NAME_SIZE];
			char password[PASSWORD_SIZE];

			if (sscanf(line, "%19[^,],%39[^,],%19[^,],%d", username, name, password, &perm) == 4)
			{
				strncpy(temp[count].username, username, USERNAME_SIZE - 1);
				strncpy(temp[count].name, name, NAME_SIZE - 1);
				XORCipher(password);
				strncpy(temp[count].password, password, PASSWORD_SIZE - 1);
				temp[count].permissionLvl = perm;
				temp[count].isActive = true;
				count++;
			}
			else
				printf("Skipping invalid line: %s\n", line);
		}

		*emploees = temp;
		fclose(fp);

		return count;
	}
}

void saveEmploeesToFile(Employee* emploees, char* fileName, int numOfEmploees)
{
	FILE* fp = fopen(fileName, "w");
	int i;

	assert(fp != NULL);

	for (i = 0; i < numOfEmploees; i++)
		if (emploees[i].isActive)
		{
			XORCipher(emploees[i].password);
			fprintf(fp, "%s,%s,%s,%d\n", emploees[i].username, emploees[i].name, emploees[i].password, emploees[i].permissionLvl);
		}

	fclose(fp);
}

void printEmployee(Employee* employee)
{
	printFixed(employee->username, USERNAME_SIZE);
	printFixed(employee->name, 20);
	printFixed(employee->password, PASSWORD_SIZE);
	printf("%d\n", employee->permissionLvl);
}

void printEmploeesList(Employee* emploees, int numOfEmploees)
{
	int i;

	printFixed("Username", USERNAME_SIZE);
	printFixed("Name", 20);
	printFixed("Password", PASSWORD_SIZE);
	printf("Permission Level\n");

	for (i = 0; i < numOfEmploees; i++)
		if (emploees[i].isActive)
			printEmployee(&emploees[i]);
}

int login(Employee* emploees, int numOfEmploees)
{
	int i, try;
	char usernameBuffer[USERNAME_SIZE];
	char passwordBuffer[PASSWORD_SIZE];

	printf("Welcome to Eilon's Toy Store!\n");
	printf("=====================================\n\n");

	for (try = 0; try < 3; try++)
	{
		printf("Username: ");
		scanf("%s", &usernameBuffer);
		cleanBuffer();
		printf("Password: ");
		scanf("%s", &passwordBuffer);
		cleanBuffer();
		printf("\n");

		for (i = 0; i < numOfEmploees; i++)
		{
			if (!strcmp(emploees[i].username, usernameBuffer) && !strcmp(emploees[i].password, passwordBuffer))
			{
				printf("Welcome %s, you're logged in!\n\n", emploees[i].name);
				LOG_LOGIN(emploees[i].name);
				Sleep(2000);
				return emploees[i].permissionLvl;
			}
		}

		printf("You enterd wrong credentials!\n");
		if(try < 2)
			printf("Please try again (you have % d tries left)\n\n", 2 - try);
	}

	LOG_ERROR("3 failed tries to enter credentials, goodbye!");
	printf("3 failed tries to enter credentials, goodbye!\n\n");

	return 0;
}

int findUser(Employee* emploees, int numOfEmploees, char* username)
{
	int i;

	for (i = 0; i < numOfEmploees; i++)
		if (!strcmp(emploees[i].username, username))
			return i;

	return -1;
}

int addUser(Employee** emploees, int numOfEmploees)
{
	Employee new;
	Employee* temp;

	printf("Enter a new username: ");
	scanf("%s", new.username);
	cleanBuffer();
	if (findUser(*emploees, numOfEmploees, new.username) != -1)
	{
		printf("User already exists! Unable to add\n");
		return numOfEmploees;
	}

	printf("Enter a name: ");
	scanf("%[^\n]", new.name);
	cleanBuffer();

	printf("Enter a password: ");
	scanf("%s", new.password);
	cleanBuffer();

	printf("Enter a permission level: ");
	scanf("%d", &new.permissionLvl);
	cleanBuffer();

	new.isActive = true;

	temp = (Employee*)realloc(*emploees, (numOfEmploees + 1) * sizeof(Employee));

	*emploees = temp;
	(*emploees)[numOfEmploees] = new;

	LOG_ADD_EMPLOYEE(new.name);

	return numOfEmploees + 1;
}

void deleteUser(Employee* emploees, int numOfEmploees)
{
	char buffer[40];
	int userIdx;

	printf("Enter username to delete: ");
	scanf("%s", buffer);
	cleanBuffer();
	userIdx = findUser(emploees, numOfEmploees, buffer);
	if (userIdx == -1)
		printf("User doesn't exist! Unable to delete\n");

	else
	{
		printf("User is now inactive and will be deleted on close\n");
		LOG_DELETE_EMPLOYEE(emploees[userIdx].name);
		emploees[userIdx].isActive = false;
	}
}

void editUser(Employee* emploees, int numOfEmploees)
{
	char buffer[40];
	int userIdx;

	printf("Enter username to edit: ");
	scanf("%s", buffer);
	cleanBuffer();
	userIdx = findUser(emploees, numOfEmploees, buffer);
	if (userIdx == -1)
		printf("User doesn't exist! Unable to edit\n");

	else
	{
		printf("Edit fields. x to keep existing\n");
		
		printf("Name [%s]: ", emploees[userIdx].name);
		scanf("%[^\n]",buffer);
		cleanBuffer();
		if (strcmp(buffer, "x"))
			strcpy(emploees[userIdx].name, buffer);

		printf("Password [%s]: ", emploees[userIdx].password);
		scanf("%s", buffer);
		cleanBuffer();
		if (strcmp(buffer, "x"))
			strcpy(emploees[userIdx].password, buffer);


		printf("Permission Level [%d]: ", emploees[userIdx].permissionLvl);
		scanf("%s", buffer);
		cleanBuffer();
		if (strcmp(buffer, "x"))
			sscanf(buffer, "%u", &emploees[userIdx].permissionLvl);

		LOG_EDIT_EMPLOYEE(emploees[userIdx].name);

		emploees[userIdx].isActive = true;
	}
}