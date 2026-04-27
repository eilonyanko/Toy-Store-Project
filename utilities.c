//Eilon Yanko 318879277

#include "utilities.h"

char* dateToStr(time_t d, char* buffer, size_t bufsize)
{
	struct tm* tm_info = localtime(&d);
	strftime(buffer, bufsize, "%d/%m/%Y", tm_info);
	return buffer;
}

int daysInMonth(int month, int year)
{
	const int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	int d = days[month];
	//leap year check for February
	if (month == 1 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
		d = 29;
	return d;
}

time_t strToDate(char* strDate)
{
	struct tm tm_info = { 0 };
	if (sscanf(strDate, "%d/%d/%d", &tm_info.tm_mday, &tm_info.tm_mon, &tm_info.tm_year) != 3)
		return -1;

	//validate ranges
	if (tm_info.tm_mon < 1 || tm_info.tm_mon  > 12)  
		return -1;
	if (tm_info.tm_year < 1900)
		return -1;
	if (tm_info.tm_mday < 1 || tm_info.tm_mday > daysInMonth(tm_info.tm_mon - 1, tm_info.tm_year))
		return -1;

	tm_info.tm_mon -= 1;
	tm_info.tm_year -= 1900;
	tm_info.tm_isdst = -1;

	return mktime(&tm_info);
}

time_t strToDateTime(char* strDate)
{
	struct tm tm_info = { 0 };
	if (sscanf(strDate, "%d/%d/%d %d:%d:%d", &tm_info.tm_mday, &tm_info.tm_mon, &tm_info.tm_year,
			&tm_info.tm_hour, &tm_info.tm_min, &tm_info.tm_sec) != 6)
		return -1;

	tm_info.tm_mon -= 1;
	tm_info.tm_year -= 1900;
	tm_info.tm_isdst = -1;

	return mktime(&tm_info);
}

void cleanBuffer()
{
	int c;
	// Read characters one by one until the buffer is empty
	while ((c = getchar()) != '\n' && c != EOF);
}

void printFixed(char* s, int width)
{
	char fmt[20];
	int len = (int) strlen(s);
	if (len >= width)
		sprintf(fmt, "%%.%ds.. ", width-3);
	else
		sprintf(fmt, "%%-%ds", width);

	printf(fmt, s);
}

int countLinesInTxtFiles(FILE* fp)
{
	int linesCnt = 1;
	char tav;

	if (fgetc(fp) == EOF)
		return 0;

	while ((tav = fgetc(fp)) != EOF)
		if (tav == '\n')
			linesCnt++;

	return linesCnt;
}

void clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void pressKeyToContinue()
{
	printf("\nPress enter to continue...");
	getchar();
}

void XORCipher(char* password)
{
	if (password == NULL) return;

	for (int i = 0; password[i] != '\0'; i++)
	{
		char encrypted = password[i] ^ CIPHER_KEY;

		if (encrypted == DELIMITER || encrypted == '\n' || encrypted == '\r')
			continue;

		password[i] = encrypted;
	}
}

void toLowerCase(char* dest, const char* src)
{
	int i = 0;

	while (src[i])
	{
		dest[i] = tolower((unsigned char)src[i]);
		i++;
	}

	dest[i] = '\0';
}