//Eilon Yanko 318879277

#include "items.h"

void insert(node** tree, node* parent, Item* item)
{
	node* temp = NULL;
	//if tree node is empty, then create a new item and add it as head.
	if (!(*tree))
	{
		temp = (node*)malloc(sizeof(node));
		//initialize left and right pointers to NULL, this node is currently a leaf
		temp->left = temp->right = NULL;
		//initialize father to the one who called me.
		temp->parent = parent;

		temp->data.id = item->id;
		strncpy(temp->data.name, item->name, ITEM_NAME_SIZE);
		strncpy(temp->data.category, item->category, CATEGORY_SIZE);
		temp->data.price = item->price;
		temp->data.topSeller = item->topSeller;
		temp->data.isActive = item->isActive;
		temp->data.launchDate = item->launchDate;
		temp->data.inventory = item->inventory;
		*tree = temp;
		return;
	}

	if (item->id < (*tree)->data.id)
	{
		//insert into left pointer of tree, sending the pointer, father (himself) and value
		insert(&(*tree)->left, *tree, item);
	}
	else if (item->id > (*tree)->data.id)
	{
		//insert into right pointer of tree, sending the pointer, father (himself) and value
		insert(&(*tree)->right, *tree, item);
	}
}

void insertHead(node** tree, Item* item)
{
	insert(tree, NULL, item);
}

void printItem(node* tree)
{
	char dateBuffer[12];
	if (tree)
	{
		printf("%d\t", tree->data.id);
		printFixed(tree->data.name, 20);
		printFixed(tree->data.category, 20);
		printf("%.2f\t\t", tree->data.price);
		printf("%s\t\t", tree->data.topSeller ? "Yes" : "No");
		printf("%s\t", tree->data.isActive ? "Yes" : "No");
		printf("%s\t", dateToStr(tree->data.launchDate, dateBuffer, sizeof(dateBuffer)));
		printf("%d\n", tree->data.inventory);
	}
}

void printInorder(node* tree)
{
	if (tree)
	{
		printInorder(tree->left);

		printItem(tree);

		printInorder(tree->right);
	}
}

void printItemsList(node* tree)
{
	printf("ID\t");
	printFixed("Item Name", 20);
	printFixed("Category", 20);
	printf("Price\t\t");
	printf("Top Seller\t");
	printf("Active\t");
	printf("Launch Date\t");
	printf("Inventory\n");
	printInorder(tree);
}

int findPostorder(node* tree, Critrria* crit)
{
	int numOfFoundItems = 0;
	char lowerTreeName[ITEM_NAME_SIZE];
	char lowerCritName[ITEM_NAME_SIZE];
	char lowerTreeCat[CATEGORY_SIZE];
	char lowerCritCat[CATEGORY_SIZE];

	if (tree)
	{
		numOfFoundItems += findPostorder(tree->left, crit);

		numOfFoundItems += findPostorder(tree->right, crit);

		if (!tree->data.isActive)
			return numOfFoundItems;

		if (strcmp(crit->name, "x") != 0)
		{
			toLowerCase(lowerTreeName, tree->data.name);
			toLowerCase(lowerCritName, crit->name);
			if (strstr(lowerTreeName, lowerCritName) == NULL)
				return numOfFoundItems;
		}

		if (strcmp(crit->category, "x") != 0)
		{
			toLowerCase(lowerTreeCat, tree->data.category);
			toLowerCase(lowerCritCat, crit->category);
			if (strstr(lowerTreeCat, lowerCritCat) == NULL)
				return numOfFoundItems;
		}

		if (crit->launchDateFrom >= tree->data.launchDate || crit->launchDateTo <= tree->data.launchDate)
			return numOfFoundItems;

		if (crit->priceMin >= tree->data.price || crit->priceMax <= tree->data.price)
			return numOfFoundItems;

		if (crit->inventoryFrom >= tree->data.inventory || crit->inventoryTo <= tree->data.inventory)
			return numOfFoundItems;

		if ((crit->topSeller == 'y' && !tree->data.topSeller) || (crit->topSeller == 'n' && tree->data.topSeller))
			return numOfFoundItems;

		numOfFoundItems++;

		printItem(tree);
	}

	return numOfFoundItems;
}

void findItems(node* tree)
{
	Critrria crit;
	char buffer[100];
	int numOfFoundItems;

	printf("Enter search criteria\n\n");

	printf("Item name (x to ignore): ");
	scanf("%[^\n]", crit.name);
	cleanBuffer();

	printf("Category (x to ignore): ");
	scanf("%[^\n]", crit.category);
	cleanBuffer();

	printf("Minimum price (x to ignore): ");
	scanf("%s", &buffer);
	cleanBuffer();
	if (strcmp(buffer, "x"))
		sscanf(buffer, "%f", &crit.priceMin);
	else
		crit.priceMin = 0;

	printf("Maximum price (x to ignore): ");
	scanf("%s", &buffer);
	cleanBuffer();
	if (strcmp(buffer, "x"))
		sscanf(buffer, "%f", &crit.priceMax);
	else
		crit.priceMax = FLT_MAX;

	printf("Is top seller (y/n/x to ignore): ");
	crit.topSeller = getchar();
	cleanBuffer();

	do
	{
		printf("Launch from date (dd/mm/yyyy, x to ignore): ");
		scanf("%s", buffer);
		cleanBuffer();
		if (!strcmp(buffer, "x"))
		{
			crit.launchDateFrom = (time_t)LLONG_MIN;
			break;
		}
		crit.launchDateFrom = strToDate(buffer);
		if (crit.launchDateFrom == -1)
			printf("The date you entered is invalid, enter again!\n");
	} while (crit.launchDateFrom == -1);

	do
	{
		printf("Launch to date (dd/mm/yyyy, x to ignore): ");
		scanf("%s", buffer);
		cleanBuffer();
		if (!strcmp(buffer, "x"))
		{
			crit.launchDateTo = (time_t)LLONG_MAX;
			break;
		}
		crit.launchDateTo = strToDate(buffer);
		if (crit.launchDateTo == -1)
			printf("The date you entered is invalid, enter again!\n");
	} while (crit.launchDateTo == -1);

	printf("Minimum number of items (x to ignore): ");
	scanf("%s", &buffer);
	cleanBuffer();
	if (strcmp(buffer, "x"))
		sscanf(buffer, "%u", &crit.inventoryFrom);
	else
		crit.inventoryFrom = 0;

	printf("Maximum number of items (x to ignore): ");
	scanf("%s", &buffer);
	cleanBuffer();
	if (strcmp(buffer, "x"))
		sscanf(buffer, "%u", &crit.inventoryTo);
	else
		crit.inventoryTo = UINT_MAX;

	printf("\nID\t");
	printFixed("Item Name", 20);
	printFixed("Category", 20);
	printf("Price\t\t");
	printf("Top Seller\t");
	printf("Active\t");
	printf("Launch Date\t");
	printf("Inventory\n");
	numOfFoundItems = findPostorder(tree, &crit);
	printf("\nNumber of items found: %d\n", numOfFoundItems);
}

void deltree(node* tree)
{
	if (tree)
	{
		deltree(tree->left);
		deltree(tree->right);
		free(tree);
	}
}

bool delItem(node* tree)
{
	unsigned int id;
	node* foundItem;

	printf("Delete item\n");

	printf("Catalog number: ");
	scanf("%u", &id);
	cleanBuffer();

	foundItem = search(tree, id);
	if (!foundItem)
	{
		printf("Catalog number wasn't found\n");

		return false;
	}

	foundItem->data.isActive = false;

	LOG_DELETE_ITEM(foundItem->data.name);

	return true;
}

node* search(node* tree, unsigned int id)
{
	if (!tree)
		return NULL;

	if (id < tree->data.id)
		return search(tree->left, id);

	else if (id > tree->data.id)
		return search(tree->right, id);

	else if (id == tree->data.id)
		return tree;
}

bool getNewItem(node** tree)
{
	time_t t;
	char date[50];
	char isTopSeller[50];
	Item temp;
	node* foundItem;
	printf("Insert new item \n");

	printf("Catalog number: ");
	scanf("%u", &(temp.id));
	cleanBuffer();

	foundItem = search(*tree, temp.id);
	if (foundItem)
	{
		printf("Item %s is already exists!\n\n", foundItem->data.name);
		if (foundItem->data.isActive)
			return false;
		else
		{
			printf("Item was deleted, and now restored \n\n");
			foundItem->data.isActive = true;
			return false;
		}
	}

	printf("Item name: ");
	scanf("%[^\n]", temp.name);
	cleanBuffer();

	printf("Category: ");
	scanf("%[^\n]", temp.category);
	cleanBuffer();

	printf("Price: ");
	scanf("%f", &(temp.price));
	cleanBuffer();

	printf("Is top seller: ");
	scanf("%s", isTopSeller);
	cleanBuffer();

	if (strcmp(isTopSeller, "YES") == 0 || strcmp(isTopSeller, "yes") == 0 || strcmp(isTopSeller, "Yes") == 0)
		temp.topSeller = true;
	else
		temp.topSeller = false;

	temp.isActive = true;

	do
	{
		printf("Launch date (dd/mm/yyyy): ");
		scanf("%s", date);
		cleanBuffer();
		t = strToDate(date);
		if (t == -1)
			printf("The date you entered is invalid, enter again!\n");
		else
			temp.launchDate = t;
	} while (t == -1);
	
	printf("Number of items: ");
	scanf("%u", &temp.inventory);
	cleanBuffer();
	printf("\n");

	insertHead(tree, &temp);

	LOG_ADD_ITEM(temp.name);

	return true;
}

void saveInorder(node* tree, FILE* fp)
{
	if (tree)
	{
		
		saveInorder(tree->left, fp);

		fwrite(&tree->data, sizeof(Item), 1, fp);

		saveInorder(tree->right, fp);
	}
}

bool saveItemsToFile(node* tree, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");

	assert(fp != NULL);

	saveInorder(tree, fp);

	fclose(fp);

	return true;
}

long countItems(FILE* fp)
{
	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	return file_size / sizeof(Item);
}

int readItemsAt(FILE* fp, long index, Item* out)
{
	fseek(fp, index * sizeof(Item), SEEK_SET);
	return fread(out, sizeof(Item), 1, fp) == 1;
}

void loadBalanced(FILE* fp, node** tree, long low, long high)
{
	if (low > high)
		return;

	long mid = low + (high - low) / 2;

	Item temp;
	if (!readItemsAt(fp, mid, &temp))
	{
		printf("Error reading record at index %ld\n", mid);
		return;
	}

	insertHead(tree, &temp);

	// Recurse on left half first, then right half
	loadBalanced(fp, tree, low, mid - 1);
	loadBalanced(fp, tree, mid + 1, high);
}

void loadItemsFromFile(node** tree, const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Failed to open items file! Creating a new file\n");
		LOG_ERROR("Failed to open items file! Creating a new file");
		return;
	}

	long n = countItems(fp);
	if (n <= 0) {
		fclose(fp);
		return;
	}

	loadBalanced(fp, tree, 0, n - 1);

	fclose(fp);
}

bool editItem(node* tree)
{
	unsigned int id;
	char entryBuffer[100];
	time_t tempDate;
	node* foundItem;
	char dateBuffer[12];

	printf("Edit item (enter 'x' in each field you don't want to change) \n");

	printf("Catalog number: ");
	scanf("%u", &id);
	cleanBuffer();

	foundItem = search(tree, id);
	if (!foundItem)
	{
		printf("Catalog number wasn't found\n");

		return false;
	}

	printf("Item name [%s]: ", foundItem->data.name);
	scanf("%[^\n]", &entryBuffer);
	cleanBuffer();
	if (strcmp(entryBuffer, "x"))
		strcpy(foundItem->data.name, entryBuffer);

	printf("Category [%s]: ", foundItem->data.category);
	scanf("%[^\n]", &entryBuffer);
	cleanBuffer();
	if (strcmp(entryBuffer, "x"))
		strcpy(foundItem->data.category, entryBuffer);

	printf("Price [%.2f]: ", foundItem->data.price);
	scanf("%s", &entryBuffer);
	cleanBuffer();
	if (strcmp(entryBuffer, "x"))
		sscanf(entryBuffer, "%f", &foundItem->data.price);

	printf("Is top seller [%s]: ", foundItem->data.topSeller ? "yes" : "no");
	scanf("%s", &entryBuffer);
	cleanBuffer();
	if (strcmp(entryBuffer, "x"))
		if (strcmp(entryBuffer, "YES") == 0 || strcmp(entryBuffer, "yes") == 0 || strcmp(entryBuffer, "Yes") == 0)
			foundItem->data.topSeller = true;
		else
			foundItem->data.topSeller = false;

	do
	{
		printf("Launch date (dd/mm/yyyy) [%s]: ", dateToStr(foundItem->data.launchDate, dateBuffer, sizeof(dateBuffer)));
		scanf("%s", entryBuffer);
		cleanBuffer();
		if (!strcmp(entryBuffer, "x"))
			break;
		tempDate = strToDate(entryBuffer);
		if (tempDate == -1)
			printf("The date you entered is invalid, enter again!\n");
		else
			foundItem->data.launchDate = tempDate;
	} while (tempDate == -1);

	printf("Number of items in inventory[%d]: ", foundItem->data.inventory);
	scanf("%s", &entryBuffer);
	cleanBuffer();
	if (strcmp(entryBuffer, "x"))
		sscanf(entryBuffer, "%u", &foundItem->data.inventory);
	printf("\n");

	LOG_EDIT_ITEM(foundItem->data.name);

	return true;
}

bool itemInfo(node* tree, unsigned int id, char* itemName, float* price)
{
	node* foundItem = search(tree, id);

	if (foundItem == NULL)
	{
		printf("Item wasn't found!\n");
		return false;
	}

	if (foundItem->data.inventory == 0)
	{
		printf("Item is out of stock!\n");
		return false;
	}

	printf("Price of the item %s is: %.2f\n\n", foundItem->data.name, foundItem->data.price);
	strcpy(itemName, foundItem->data.name);
	*price = foundItem->data.price;
	return true;
}

void itemUpdateInventory(node* tree, unsigned int id, int deltaInventory)
{
	node* foundItem = search(tree, id);

	if (foundItem == NULL)
	{
		printf("Item wasn't found!");
		return;
	}

	foundItem->data.inventory += deltaInventory;
}