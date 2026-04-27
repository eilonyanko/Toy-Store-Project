//Eilon Yanko 318879277

#ifndef ITEMS_H
#define ITEMS_H

#define _CRT_SECURE_NO_WARNINGS

#include "utilities.h"

#define ITEM_NAME_SIZE 50
#define CATEGORY_SIZE 20

typedef struct item {
	unsigned int id;
	char name[ITEM_NAME_SIZE];
	char category[CATEGORY_SIZE];
	float price;
	bool topSeller;
	bool isActive;
	time_t launchDate;
	unsigned int inventory;
}Item;

typedef struct binTree {
	Item data;
	struct binTree *right, *left, *parent;
}node;

typedef struct criteria {
	char name[ITEM_NAME_SIZE];
	char category[CATEGORY_SIZE];
	float priceMin;
	float priceMax;
	char topSeller;
	time_t launchDateFrom;
	time_t launchDateTo;
	unsigned int inventoryFrom;
	unsigned int inventoryTo;
}Critrria;

void insertHead(node** tree, Item* item);
void printItemsList(node* tree);
void deltree(node* tree);
bool delItem(node* tree);
node* search(node* tree, unsigned int id);
void cleanBuffer();
bool getNewItem(node** tree);
bool saveItemsToFile(node* tree, const char* fileName);
void loadItemsFromFile(node** tree, const char* filename);
bool editItem(node* tree);
void findItems(node* tree);
bool itemInfo(node* tree, unsigned int id, char* itemName, float* price);
void itemUpdateInventory(node* tree, unsigned int id, int deltaInventory);

#endif