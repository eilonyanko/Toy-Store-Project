//Eilon Yanko 318879277

#ifndef LOG_H
#define LOG_H

#define _CRT_SECURE_NO_WARNINGS

#include "utilities.h"
#include <stdarg.h>

#define LOG_FILE_NAME "log.txt"
#define SALE_ACTION "Sale"
#define RETURN_ACTION "Return"

#define MAX_SECONDS_TO_RETURN 60*60*24
//#define MAX_SECONDS_TO_RETURN (60*60*24*14)

#define LOG_LOGIN(userName) storeLog(2, "Login", userName)
#define LOG_ERROR(error) storeLog(2, "Error", error)
#define LOG_SALE(customerName, itemID, itemName, price) storeLog(5, SALE_ACTION ,customerName, itemID, itemName, price)
#define LOG_RETURN(customerName, itemID, itemName, price) storeLog(5, RETURN_ACTION ,customerName, itemID, itemName, price)
#define LOG_CLOSE storeLog(1, "Application closed, goodbye!")
#define LOG_ADD_CUSTOMER(customerName) storeLog(3, "Added", "Customer", customerName)
#define LOG_DELETE_CUSTOMER(customerName) storeLog(3, "Deleted", "Customer", customerName)
#define LOG_ADD_EMPLOYEE(employeeName) storeLog(3, "Added", "Employee", employeeName)
#define LOG_DELETE_EMPLOYEE(employeeName) storeLog(3, "Deleted", "Employee", employeeName)
#define LOG_EDIT_EMPLOYEE(employeeName) storeLog(3, "Edited", "Employee", employeeName)
#define LOG_ADD_ITEM(itemName) storeLog(3, "Added", "Item", itemName)
#define LOG_DELETE_ITEM(itemName) storeLog(3, "Deleted", "Item", itemName)
#define LOG_EDIT_ITEM(itemName) storeLog(3, "Edited", "Item", itemName)

void storeLog(int count, ...);
bool customerPurcahseHistory(char* customerName);
bool saleReturnCheck(char* customerName, unsigned int id, float* price);
void showLog();

#endif