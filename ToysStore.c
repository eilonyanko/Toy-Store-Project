//Eilon Yanko 318879277

#include "items.h"
#include "emploees.h"
#include "customers.h"
#include "log.h"
#include "utilities.h"

#define ITEMS_FILE_NAME "items.bin"
#define EMPLOEES_FILE_NAME "emploees.txt"
#define CUSTOMERS_FILE_NAME "customers.txt"

typedef struct menu {
	int num;
	char action[100];
	int permissionLvl;
}Menu;

typedef struct screen {
	char title[50];
	int numOfMenus;
	Menu command[6];
}Screen;

int menuScreen(Screen* screen, int permissionLvl)
{
	int i;
	char* underlines = "------------------------------------------------------";
	int choice;

	while (1)
	{
		clearScreen();
		printf("%s\n", screen->title);
		printf("%.*s\n\n", (int)strlen(screen->title), underlines);

		for (i = 0; i < screen->numOfMenus; i++)
			if (permissionLvl <= screen->command[i].permissionLvl)
				printf("%d) %s\n\n", screen->command[i].num, screen->command[i].action);

		printf("Choose from the menu: ");
		scanf("%d", &choice);
		cleanBuffer();
		printf("\n");

		for (i = 0; i < screen->numOfMenus; i++)
			if (permissionLvl <= screen->command[i].permissionLvl)
				if (choice == screen->command[i].num)
					return choice;

		printf("\nYou entered a wrong choice, try again!\n\n");
		Sleep(3000);
	}
}

void sell(List* customers, node* itemsTree)
{
	char customerName[CUSTOMER_NAME_SIZE];
	float totalSell = 0;
	int itemsPurchasedCnt = 0;
	int id;
	char itemName[ITEM_NAME_SIZE];
	float price;
	char priceStr[100];
	char entryBuffer[10];
	char idStr[10];

	printf("Welcome to the store! What's your name? ");
	scanf("%[^\n]", customerName);
	cleanBuffer();

	if (!isCustomersExists(customers, customerName))
	{
		printf("I couldn't find you, so I'll add you now to our customers list\n\n");
		newCustomerJoin(customers, customerName);
	}

	printf("What would you like to buy?\n\n");
	while(1)
	{
		printf("Enter item number (0 to finish purchase): ");
		scanf("%d", &id);
		cleanBuffer();

		if (id == 0)
			break;

		if (itemInfo(itemsTree, id, itemName, &price))
		{
			printf("Would you like to buy? (y/n) ");
			scanf("%s", entryBuffer);
			cleanBuffer();

			if (!strcmp(entryBuffer, "y"))
			{
				printf("\n");
				sprintf(priceStr, "%.2f", price);
				sprintf(idStr, "%d", id);
				LOG_SALE(customerName, idStr, itemName, priceStr);
				totalSell += price;
				itemsPurchasedCnt++;
				itemUpdateInventory(itemsTree, id, -1);
			}
		}

		if (itemsPurchasedCnt == 3)
		{
			printf("You reached the maximum number of items for one purchase!\n");
			break;
		}

	}

	if (totalSell > 0)
	{
		updateCustomerTotalPurchased(customers, customerName, totalSell);
		printf("Total purchased: %.2f", totalSell);
	}
}

void returnPurcahsedItem(List* customers, node* itemsTree)
{
	char customerName[CUSTOMER_NAME_SIZE];
	int id;
	float price;

	printf("Welcome to the store! What's your name? ");
	scanf("%[^\n]", customerName);
	cleanBuffer();

	if (!isCustomersExists(customers, customerName))
	{
		printf("I couldn't find you, so you don't have any items to return!\n\n");
		return;
	}

	if (!customerPurcahseHistory(customerName))
	{
		printf("\nYou didn't purchase any item!\n\n");
		return;
	}

	printf("\n");
	printf("What would you like to return? (can't return an item that was purchased more that 14 days ago)\n\n");
	printf("Enter item number: ");
	scanf("%d", &id);
	cleanBuffer();

	if (saleReturnCheck(customerName, id, &price))
	{
		updateCustomerTotalPurchased(customers, customerName, -price);
		itemUpdateInventory(itemsTree, id, 1);
	}
}

void itemsScreen(int permissionLvl, node** itemsTree, Screen* itemsMenu)
{
	while (1)
	{
		int choice = menuScreen(itemsMenu, permissionLvl);

		switch (choice) {

		case 1:
			printItemsList(*itemsTree);
			break;

		case 2:
			findItems(*itemsTree);
			break;

		case 3:
			if (getNewItem(itemsTree))
				printf("Item was added succesfully!\n\n");
			break;
			
		case 4:
			if (delItem(*itemsTree))
				printf("Item was deleted succesfully!\n\n");
			break;

		case 5:
			if (editItem(*itemsTree))
				printf("Item was edited succesfully!\n\n");
			break;

		case 6:
			return;
		}

		pressKeyToContinue();
	}
}

void customersScreen(int permissionLvl, List* customers, Screen* customersMenu)
{
	while (1)
	{
		int choice = menuScreen(customersMenu, permissionLvl);

		switch (choice) {

		case 1:
			printCustomersList(customers);
			break;

		case 2:
			addCustomer(customers);
			printf("Customer was added succesfully!\n\n");
			break;

		case 3:
			deleteCustomer(customers);
			printf("Customer was deleted succesfully!\n\n");
			break;

		case 4:
			return;
		}

		pressKeyToContinue();
	}
}

void sellReturnScreen(int permissionLvl, List* customers, node* itemsTree, Screen* sellReturnMenu)
{
	while (1)
	{
		int choice = menuScreen(sellReturnMenu, permissionLvl);

		switch (choice) {

		case 1:
			printItemsList(itemsTree);
			break;

		case 2:
			findItems(itemsTree);
			break;

		case 3:
			sell(customers, itemsTree);
			break;

		case 4:
			returnPurcahsedItem(customers, itemsTree);
			break;

		case 5:
			return;
		}

		pressKeyToContinue();
	}
}

void systemScreen(int permissionLvl, List* customers, Employee** emploees, Screen* systemMenu, int* numOfEmploees)
{
	while (1)
	{
		int choice = menuScreen(systemMenu, permissionLvl);

		switch (choice) {

		case 1:
			printEmploeesList(*emploees, *numOfEmploees);
			break;

		case 2:
			*numOfEmploees = addUser(emploees, *numOfEmploees);
			break;

		case 3:
			deleteUser(*emploees, *numOfEmploees);
			break;

		case 4:
			editUser(*emploees, *numOfEmploees);
			break;

		case 5:
			showLog();
			break;

		case 6:
			return;
		}

		pressKeyToContinue();
	}
}

void mainScreen(int permissionLvl, List* customers, Employee** emploees, node** itemsTree,
		Screen* systemMenu, Screen* customersMenu, Screen* itemsMenu, Screen* sellReturnMenu, Screen* mainMenu, int* numOfEmploees)
{
	while (1)
	{
		int choice = menuScreen(mainMenu, permissionLvl);

		switch (choice) {

		case 1:
			customersScreen(permissionLvl, customers, customersMenu);
			break;

		case 2:
			itemsScreen(permissionLvl, itemsTree, itemsMenu);
			break;

		case 3:
			sellReturnScreen(permissionLvl, customers, *itemsTree, sellReturnMenu);
			break;

		case 4:
			systemScreen(permissionLvl, customers, emploees, systemMenu, numOfEmploees);
			break;

		case 5:
			printf("Goodbye!\n");
			Sleep(1500);
			return;
		}
	}
}

void main()
{
	//intialization

	clearScreen();

	Employee* emploees = NULL;
	int numOfEmploees = loadEmploeesFromFile(&emploees, EMPLOEES_FILE_NAME);

	int permissionLvl = login(emploees, numOfEmploees);
	if (!permissionLvl)
	{
		LOG_CLOSE;
		return;
	}

	node* itemsTree = NULL;
	loadItemsFromFile(&itemsTree, ITEMS_FILE_NAME);

	List* customers = NULL;
	loadCustomersFromFile(&customers, CUSTOMERS_FILE_NAME);

	Screen mainMenu = {
	.title = "Main",
	.numOfMenus = 5,
	.command = {
		[0] = {.num = 1, .action = "Customers menu", .permissionLvl = 3},
		[1] = {.num = 2, .action = "Items menu", .permissionLvl = 3},
		[2] = {.num = 3, .action = "Sell and return menu", .permissionLvl = 2},
		[3] = {.num = 4, .action = "System menu", .permissionLvl = 1},
		[4] = {.num = 5, .action = "Close the store", .permissionLvl = 3}
	}
	};

	Screen itemsMenu = {
	.title = "Items",
	.numOfMenus = 6,
	.command = {
		[0] = {.num = 1, .action = "Show all items", .permissionLvl = 3},
		[1] = {.num = 2, .action = "Find items", .permissionLvl = 3},
		[2] = {.num = 3, .action = "Add item", .permissionLvl = 3},
		[3] = {.num = 4, .action = "Delete item", .permissionLvl = 2},
		[4] = {.num = 5, .action = "Edit item", .permissionLvl = 2},
		[5] = {.num = 6, .action = "Return to main menu", .permissionLvl = 3}
	}
	};

	Screen customersMenu = {
	.title = "Customers",
	.numOfMenus = 4,
	.command = {
		[0] = {.num = 1, .action = "Show all customers", .permissionLvl = 3},
		[1] = {.num = 2, .action = "Add customer", .permissionLvl = 3},
		[2] = {.num = 3, .action = "Delete customer", .permissionLvl = 2},
		[3] = {.num = 4, .action = "Return to main menu", .permissionLvl = 3}
	}
	};

	Screen sellReturnMenu = {
	.title = "Sell and Return",
	.numOfMenus = 5,
	.command = {
		[0] = {.num = 1, .action = "Show all items", .permissionLvl = 3},
		[1] = {.num = 2, .action = "Find items", .permissionLvl = 3},
		[2] = {.num = 3, .action = "Sell item", .permissionLvl = 2},
		[3] = {.num = 4, .action = "Return item", .permissionLvl = 2},
		[4] = {.num = 5, .action = "Return to main menu", .permissionLvl = 3}
	}
	};

	Screen systemMenu = {
	.title = "System",
	.numOfMenus = 6,
	.command = {
		[0] = {.num = 1, .action = "Show all users", .permissionLvl = 1},
		[1] = {.num = 2, .action = "Add user", .permissionLvl = 1},
		[2] = {.num = 3, .action = "Delete user", .permissionLvl = 1},
		[3] = {.num = 4, .action = "Edit user", .permissionLvl = 1},
		[4] = {.num = 5, .action = "Show log", .permissionLvl = 1},
		[5] = {.num = 6, .action = "Return to main menu", .permissionLvl = 3}
	}
	};

	//body
	
	mainScreen(permissionLvl, customers, &emploees, &itemsTree, &systemMenu, &customersMenu, &itemsMenu, &sellReturnMenu,&mainMenu, &numOfEmploees);
	
	//finish
	saveItemsToFile(itemsTree, ITEMS_FILE_NAME);
	deltree(itemsTree);

	saveEmploeesToFile(emploees, EMPLOEES_FILE_NAME, numOfEmploees);
	free(emploees);

	saveCusromersToFile(customers, CUSTOMERS_FILE_NAME);
	clearCustomersList(customers);

	LOG_CLOSE;
}