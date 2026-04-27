//Eilon Yanko 318879277

#include "customers.h"

List* createList()
{
	List* list = (List*)malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->listCnt = 0;

	return list;
}

void insertFirst(ListNode* x, List* list)
{
	x->next = list->head;
	list->head = x;

	list->listCnt++;

	if (x->next == NULL)
		list->tail = x;
}

bool isEmpty(List* list)
{
	return list->head == NULL;
}

bool deleteFirst(List* list)
{
	if (isEmpty(list))
		return false;

	ListNode* temp = list->head;
	list->head = list->head->next;
	free(temp);
	list->listCnt--;

	if (list->head == NULL)
		list->tail = NULL;

	return true;
}

void printCustomersList(List* list)
{
	ListNode* printPtr = list->head;
	char dateBuffer[12];

	printf("\n");
	printFixed("Customer Name", CUSTOMER_NAME_SIZE);
	printf("Join Date\t");
	printf("Total Purcahsed\n");

	while (printPtr != NULL)
	{
		if (printPtr->customer.isActive)
		{
			printFixed(printPtr->customer.name, CUSTOMER_NAME_SIZE);
			printf("%s\t", dateToStr(printPtr->customer.joinDate, dateBuffer, sizeof(dateBuffer)));
			printf("%.2f\n", printPtr->customer.totalPurchased);
		}
		
		printPtr = printPtr->next;
	}

	printf("\n");
}

ListNode* findCustomer(char* name, List* list)
{
	ListNode* scanPtr = list->head;

	while (scanPtr != NULL && strcmp(scanPtr->customer.name, name))
		scanPtr = scanPtr->next;

	return scanPtr;
}

void insertAfter(ListNode* x, ListNode* y, List* list)
{
	if (y == NULL)
	{
		insertFirst(x, list);
		return;
	}

	x->next = y->next;
	y->next = x;

	list->listCnt++;

	if (list->tail == y)
		list->tail = x;
}

int listSize(List* list)
{
	return list->listCnt;
}

bool deleteX(ListNode* x, List* list)
{
	if (x == list->head)
		return deleteFirst(list);

	ListNode* scanPtr = list->head;

	while (scanPtr != NULL && scanPtr->next != x)
		scanPtr = scanPtr->next;

	if (scanPtr == NULL)
		return false;

	else
	{
		scanPtr->next = x->next;
		if (scanPtr->next == NULL)
			list->tail = scanPtr;
		free(x);
		list->listCnt--;
		return true;
	}
}

ListNode* createNode(Customer* customer)
{
	ListNode* itemPtr = (ListNode*)malloc(sizeof(ListNode));
	itemPtr->customer = *customer;
	return itemPtr;
}

void clearCustomersList(List* list)
{
	while (!isEmpty(list))
		deleteFirst(list);

	free(list);
}

void insertLast(ListNode* x, List* list)
{
	insertAfter(x, list->tail, list);
}

void loadCustomersFromFile(List** customers, const char* filename)
{
	FILE* fp = fopen(filename, "r");
	*customers = createList();
	Customer customer;
	ListNode* customerNode;

	if (!fp)
	{
		printf("Failed to open customers file! creating a new file\n\n");

		LOG_ERROR("Failed to open customers file! Creating a new file");

		Sleep(2000);

		return;
	}

	else
	{
		char line[256];

		rewind(fp);

		while (fgets(line, sizeof(line), fp) != NULL)
		{
			// skip empty lines
			if (line[0] == '\n' || line[0] == '\r') continue;

			// remove trailing newline
			line[strcspn(line, "\r\n")] = '\0';

			char dateBuffer[12];

			if (sscanf(line, "%39[^,],%11[^,],%f[^,]", customer.name, dateBuffer, &customer.totalPurchased) == 3)
			{
				customer.joinDate = strToDate(dateBuffer);
				customer.isActive = true;
				customerNode = createNode(&customer);
				insertLast(customerNode, *customers);
			}
			else
				printf("Skipping invalid line: %s\n", line);
		}

		fclose(fp);
	}
}

void insertSortedByJoinDate(ListNode* x, List* list)
{
	ListNode* scanPtr = list->head;
	ListNode* prevScanPtr = NULL;

	if (isEmpty(list))
		insertLast(x, list);

	else
	{
		while (scanPtr != NULL)
		{
			if (difftime(x->customer.joinDate, scanPtr->customer.joinDate) < 0)
			{
				insertAfter(x, prevScanPtr, list);
				return;
			}
			prevScanPtr = scanPtr;
			scanPtr = scanPtr->next;
		}

		insertLast(x, list);
	}
}

void addCustomer(List* customers)
{
	Customer new;
	char dateBuffer[12];
	time_t t;
	ListNode* customerNode;

	printf("Enter a new name: ");
	scanf("%[^\n]", new.name);
	cleanBuffer();
	if (findCustomer(new.name, customers) != NULL)
	{
		printf("Customer already exists! Unable to add\n");
		printf("\n");
		return;
	}

	do
	{
		printf("Join date (dd/mm/yyyy): ");
		scanf("%s", dateBuffer);
		cleanBuffer();
		t = strToDate(dateBuffer);
		if (t == -1)
			printf("The date you entered is invalid, enter again!\n");
		else
			new.joinDate = t;
	} while (t == -1);

	new.totalPurchased = 0;

	new.isActive = true;

	customerNode = createNode(&new);
	insertSortedByJoinDate(customerNode, customers);
	LOG_ADD_CUSTOMER(new.name);
}

void saveCusromersToFile(List* list, char* fileName)
{
	FILE* fp = fopen(fileName, "w");
	char dateBuffer[12];

	assert(fp != NULL);

	ListNode* savePtr = list->head;

	while (savePtr != NULL)
	{
		if (savePtr->customer.isActive)
			fprintf(fp, "%s,%s,%.02f\n", savePtr->customer.name,
				dateToStr(savePtr->customer.joinDate, dateBuffer, sizeof(dateBuffer)), savePtr->customer.totalPurchased);
		savePtr = savePtr->next;
	}
	fclose(fp);
}

void deleteCustomer(List* customers)
{
	char nameBuffer[40];
	ListNode* temp;

	printf("Enter customer name to delete: ");
	scanf("%[^\n]", nameBuffer);
	cleanBuffer();
	printf("\n");
	temp = findCustomer(nameBuffer, customers);
	if (temp == NULL)
	{
		printf("Customer doesn't exist! Unable to delete\n");
		printf("\n");
	}
	else
	{
		printf("User is now inactive and will be deleted on close\n");
		LOG_DELETE_CUSTOMER(temp->customer.name);
		temp->customer.isActive = false;
	}
}

bool isCustomersExists(List* customers, char* name)
{
	return findCustomer(name, customers) != NULL;
}

void newCustomerJoin(List* customers, char* name)
{
	Customer new;
	ListNode* customerNode;

	strcpy(new.name, name);
	time(&new.joinDate);
	new.totalPurchased = 0;
	new.isActive = true;

	customerNode = createNode(&new);
	insertLast(customerNode, customers);
	LOG_ADD_CUSTOMER(new.name);
}

void updateCustomerTotalPurchased(List* customers, char* name, float deltaPurcahsed)
{
	ListNode* node = findCustomer(name, customers);

	if (node == NULL)
	{
		printf("Unable to find customer!");
		return;
	}
	else
		node->customer.totalPurchased += deltaPurcahsed;
}