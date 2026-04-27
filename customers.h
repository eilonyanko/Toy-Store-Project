//Eilon Yanko 318879277

#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#define _CRT_SECURE_NO_WARNINGS

#include "utilities.h"

#define CUSTOMER_NAME_SIZE 25

typedef struct customer {
	char name[CUSTOMER_NAME_SIZE];
	time_t joinDate;
	float totalPurchased;
	bool isActive;
}Customer;

typedef struct listNode
{
	Customer customer;
	struct listNode* next;
}ListNode;

typedef struct list
{
	ListNode* head;
	ListNode* tail;
	int listCnt;
}List;

void printCustomersList(List* list);
void loadCustomersFromFile(List** customers, const char* filename);
void addCustomer(List* customers);
void saveCusromersToFile(List* list, char* fileName);
void clearCustomersList(List* list);
void deleteCustomer(List* customers);
bool isCustomersExists(List* customers, char* name);
void newCustomerJoin(List* customers, char* name);
void updateCustomerTotalPurchased(List* customers, char* name, float deltaPurcahsed);

#endif
