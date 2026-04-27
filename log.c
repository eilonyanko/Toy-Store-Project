//Eilon Yanko 318879277

#include "log.h"
#include "customers.h"
#include "items.h"

void storeLog(int count, ...)
{
    FILE* fp = fopen(LOG_FILE_NAME, "a");
    assert(fp != NULL);
    va_list args;
    time_t rawTime;
    struct tm* localTime;

    va_start(args, count);

    time(&rawTime);

    localTime = localtime(&rawTime);

    localTime->tm_mon += 1;
    localTime->tm_year += 1900;
    localTime->tm_isdst = -1;

    fprintf(fp,"%02d/%02d/%d %02d:%02d:%02d,", localTime->tm_mday, localTime->tm_mon, localTime->tm_year,
        localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    for (int i = 0; i < count; i++)
    {
        char* word = va_arg(args, char*);
        if(i == count - 1)
            fprintf(fp, "%s\n", word);
        else
            fprintf(fp, "%s,", word);
    }

    va_end(args);

    fclose(fp);
}

bool parsePurcahseLine(char* line, char* customerName, unsigned int* id, float* price, char* action, char* time, char* itemName)
{
    char customerNameBuffer[CUSTOMER_NAME_SIZE];

    if (sscanf(line, "%19[^,],%9[^,],%24[^,],%u,%49[^,],%f", time, action, customerNameBuffer, id, itemName, price) == 6)
    {
        if (strcmp(action, "Sale") && strcmp(action, "Return"))
            return false;

        if (strcmp(customerNameBuffer, customerName))
            return false;

        return true;
    }

    else
        return false;
}

bool customerPurcahseHistory(char* customerName)
{
    FILE* fp = fopen(LOG_FILE_NAME, "r");
    char line[256];
    unsigned int id;
    float price;
    char action[10];
    char timeBuffer[20];
    char itemNameBuffer[ITEM_NAME_SIZE];
    bool purchased = false;

    if (!fp)
    {
        printf("Failed to open log file!\n");

        LOG_ERROR("Failed to open log file!");

        return false;
    }

    else
    {
        printf("\n%s purcahse history\n\n", customerName);
        printFixed("Time", 25);
        printFixed("Action", 10);
        printf("Item ID\t");
        printFixed("Item Name", 25);
        printf("Price\n");

        while (fgets(line, sizeof(line), fp) != NULL)
        {
            // skip empty lines
            if (line[0] == '\n' || line[0] == '\r') continue;

            // remove trailing newline
            line[strcspn(line, "\r\n")] = '\0';
            
            if (parsePurcahseLine(line, customerName, &id, &price, action, timeBuffer, itemNameBuffer))
            {
                purchased = true;
                printFixed(timeBuffer, 25);
                printFixed(action, 10);
                printf("%u\t\t", id);
                printFixed(itemNameBuffer, 25);
                printf("%.2f\n", price);
            }
        }

        fclose(fp);
        return purchased;
    }
}

bool saleReturnCheck(char* customerName, unsigned int id, float* price)
{
    FILE* fp = fopen(LOG_FILE_NAME, "r");
    char line[256];
    unsigned int idLog;
    float priceLog;
    char action[10];
    char timeBuffer[20];
    char itemNameBuffer[ITEM_NAME_SIZE];
    unsigned int saleCnt = 0;
    time_t currentTime;
    bool saleFoundIn14 = false;
    bool saleFoundOut14 = false;
    char strID[10];
    char strPrice[10];

    time(&currentTime);

    if (!fp)
    {
        printf("Failed to open log file!\n");

        LOG_ERROR("Failed to open log file!");

        return false;
    }

    else
    {
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            // skip empty lines
            if (line[0] == '\n' || line[0] == '\r') continue;

            // remove trailing newline
            line[strcspn(line, "\r\n")] = '\0';

            if (parsePurcahseLine(line, customerName, &idLog, &priceLog, action, timeBuffer, itemNameBuffer))
            {
                if (idLog == id)
                {
                    if (difftime(currentTime, strToDateTime(timeBuffer)) < MAX_SECONDS_TO_RETURN)
                    {
                        if (!strcmp(action, "Sale"))
                        {
                            saleCnt++;   
                            *price = priceLog;
                            saleFoundIn14 = true;
                        }
                        if (!strcmp(action, "Return"))
                            saleCnt--;
                    }
                    else
                    {
                        if (!strcmp(action, "Sale"))
                            saleFoundOut14 = true;
                    }
                }
            }
        }

        fclose(fp);

        if (saleFoundIn14)
        {
            if (saleCnt <= 0)
            {
                printf("\nUnbable to return item, you have returned all items that purchaed in last 14 days\n\n");
                return false;
            }

            else
            {
                sprintf(strPrice, "%.2f", *price);
                sprintf(strID, "%u", id);
                LOG_RETURN(customerName, strID, itemNameBuffer, strPrice);
                printf("\nItem was returned succesfully!\n\n");
                return true;
            }
        }

        else if (saleFoundOut14)
        {
            printf("You purchased the item more than 14 days ago, according to our policy, you can't return the item!\n\n");
            return false;
        }

        else
        {
            printf("You didn't purchase that item!");
            return false;
        }
    }

    return false;
}

void showLog()
{
    FILE* fp = fopen(LOG_FILE_NAME, "r");
    char line[256];
    char buffer[100];
    time_t startDate;
    time_t endDate;
    time_t logTime;

    if (!fp)
    {
        printf("Failed to open log file!\n");

        LOG_ERROR("Failed to open log file!");

        return;
    }

    do
    {
        printf("Log from date (dd/mm/yyyy, x show log from start): ");
        scanf("%s", buffer);
        cleanBuffer();
        if (!strcmp(buffer, "x"))
        {
            startDate = (time_t)LLONG_MIN;
            break;
        }
        startDate = strToDate(buffer);
        if (startDate == -1)
            printf("The date you entered is invalid, enter again!\n");
    } while (startDate == -1);

    do
    {
        printf("Log until date (dd/mm/yyyy, x show log until end): ");
        scanf("%s", buffer);
        cleanBuffer();
        if (!strcmp(buffer, "x"))
        {
            endDate = (time_t)LLONG_MAX;
            break;
        }
        endDate = strToDate(buffer);
        if (endDate == -1)
            printf("The date you entered is invalid, enter again!\n");
    } while (endDate == -1);

    endDate += 24 * 3600 - 1;
    printf("\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // skip empty lines
        if (line[0] == '\n' || line[0] == '\r') continue;

        // remove trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        sscanf(line, "%19[^,]", buffer);
        logTime = strToDateTime(buffer);

        if (difftime(logTime, startDate) >= 0 && difftime(logTime, endDate) <= 0)
            printf("%s\n", line);
    }

    fclose(fp);
}