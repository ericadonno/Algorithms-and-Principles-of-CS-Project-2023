#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 25000
#define MAX_LENGTH 256

typedef struct Ingredient
{
	char *name;
	int quantity;
	struct Ingredient *next;
} Ingredient;

typedef struct IngredientRif
{
	char *name;
	int quantity;
	int scadenza;
	struct IngredientRif *next;
} IngredientRif;

typedef struct Recipe
{
	char *name;
	Ingredient *ingredients;
	int peso;
	struct Recipe *next;
} Recipe;

typedef struct Ordine
{
	int arriv_time;
	char *name;
	int quantity;
	int attesa;
	int peso;
	struct Ordine *next;
} Ordine;

Recipe *hashTable[TABLE_SIZE];
IngredientRif *hashRif[TABLE_SIZE];

Ordine *ordinihead = NULL;
Ordine *ordinitail = NULL;
Ordine *ordiniprontihead = NULL;


int hash(char *str) {
    int hash = 5381;
    int c;

    while ((c = *str++)) {
        if (c == '_') {
            c = ' '; // Treat underscore as a space
        }
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return abs(hash % TABLE_SIZE); // Ensure positive index using abs()
}

Ingredient *createIngredient(char *name, int quantity) {
    Ingredient *newIngredient = malloc(sizeof(Ingredient));
    if (!newIngredient) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    newIngredient->name = malloc(strlen(name) + 1); // Allocate memory for the name
    if (!newIngredient->name) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    strcpy(newIngredient->name, name); // Copy the name
    newIngredient->quantity = quantity;
    newIngredient->next = NULL;
    return newIngredient;
}

Ordine *createOrdine(int arriv_time, char *name, int quantity, int peso) {
    Ordine *newOrdine = malloc(sizeof(Ordine));
    if (!newOrdine) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    newOrdine->name = malloc(strlen(name) + 1); // Allocate memory for the name
    if (!newOrdine->name) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    strcpy(newOrdine->name, name); // Copy the name
    newOrdine->arriv_time = arriv_time;
    newOrdine->quantity = quantity;
    newOrdine->attesa = 0;
    newOrdine->peso = peso;
    newOrdine->next = NULL;
    return newOrdine;
}

void inserisciOrdine(int arriv_time, char *name, int quantity, int peso){
	Ordine *newOrdine = createOrdine(arriv_time, name, quantity, peso);

		if (ordinihead == NULL) {
			ordinihead = newOrdine;
			ordinitail = newOrdine;
		} else {
			ordinitail->next = newOrdine;
			ordinitail = newOrdine;
		}

}

IngredientRif *createIngredientRif(char *name, int quantity, int scadenza) {
    IngredientRif *newIngredientRif = malloc(sizeof(IngredientRif));
    if (!newIngredientRif) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    newIngredientRif->name = malloc(strlen(name) + 1); // Allocate memory for the name
    if (!newIngredientRif->name) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    strcpy(newIngredientRif->name, name); // Copy the name
    newIngredientRif->quantity = quantity;
    newIngredientRif->scadenza = scadenza;
    newIngredientRif->next = NULL;
    return newIngredientRif;
}

Recipe *createRecipe(char *name) {
    Recipe *newRecipe = malloc(sizeof(Recipe));
    if (!newRecipe) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    newRecipe->name = malloc(strlen(name) + 1); // Allocate memory for the name
    if (!newRecipe->name) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    strcpy(newRecipe->name, name); // Copy the name
    newRecipe->ingredients = NULL;
    newRecipe->peso = 0;
    newRecipe->next = NULL;
    return newRecipe;
}

void insertIngredient(Recipe *recipe, char *ingredientName, int quantity)
{
	Ingredient *newIngredient = createIngredient(ingredientName, quantity);
	newIngredient->next = recipe->ingredients;
	recipe->ingredients = newIngredient;
}

void insertIngredientRif(char *name, int quantity, int scadenza)
{
    int index = hash(name);
    IngredientRif *newIngredientRif = createIngredientRif(name, quantity, scadenza);
    IngredientRif *current = hashRif[index];
    IngredientRif *prev = NULL;

    while (current && scadenza > current->scadenza)
    {
        prev = current;
        current = current->next;
    }

    if (prev)
    {
        prev->next = newIngredientRif;
        newIngredientRif->next = current;
    }
    else
    {
        newIngredientRif->next = hashRif[index];
        hashRif[index] = newIngredientRif;
    }
}

Recipe *insertRecipe(char *name)
{
	int index = hash(name);
	Recipe *newRecipe = createRecipe(name);
	newRecipe->next = hashTable[index];
	hashTable[index] = newRecipe;
	return newRecipe;
}

Recipe* findRecipe(char *name)
{
	int index = hash(name);
	Recipe *current = hashTable[index];
	while (current)
	{
		if (strcmp(current->name, name) == 0)
		{
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void deleteRecipe(char *name)
{
	Ordine *current_ordine = ordinihead;
    while (current_ordine)
    {
        if (strcmp(current_ordine->name, name) == 0)
        {
            printf("ordini in sospeso\n");
            return;
        }
        current_ordine = current_ordine->next;
    }
	
	Ordine *curOrd = ordiniprontihead;
    while (curOrd)
    {
        if (strcmp(curOrd->name, name) == 0)
        {
            printf("ordini in sospeso\n");
            return;
        }
        curOrd = curOrd->next;
    }

	int index = hash(name);
	Recipe *current = hashTable[index];
	Recipe *prev = NULL;

	while (current)
		{
		if (strcmp(current->name, name) == 0)
		{
			if (prev)
			{
				prev->next = current->next;
			}
			else
			{
				hashTable[index] = current->next;
			}
			// Free ingredients
			Ingredient *ingredient = current->ingredients;
			while (ingredient)
			{
				Ingredient *tmp = ingredient;
				ingredient = ingredient->next;
				free(tmp->name);
				free(tmp);
			}
			free(current->name);
			free(current);
			printf("rimossa\n");
			return;
		}
		else{
			prev=current;
			current=current->next;
		}
	}
	printf("non presente\n");

}

void displayRecipes()
{
	printf("Ricette:\n");
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		Recipe *current = hashTable[i];
		while (current)
		{
			printf("Recipe: %s, Peso: %d\n", current->name, current->peso);
			Ingredient *ingredient = current->ingredients;
			while (ingredient)
			{
				printf("  Ingredient: %s, Quantity: %d\n", ingredient->name,
					   ingredient->quantity);
				ingredient = ingredient->next;
			}
			current = current->next;
		}
	}
}

void displayRif()
{
	printf("Ingredienti Disponibili:\n");
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		IngredientRif *current = hashRif[i];
		while (current)
		{
			printf("Ingredient: %s, ", current->name);
			printf("Quantity: %d, ",current->quantity);
			printf("Scadenza: %d\n", current->scadenza);
			current = current->next;
		}
	}
}

void displayOrdine(){
	printf("Ordini:\n");
	if (ordinihead==NULL){
		printf("non ci sono ordini\n");
	}
	else{
		Ordine *current = ordinihead;
		while (current){
			printf("Ordine arrivato: %d, ", current->arriv_time);
			printf("Ricetta: %s, ",current->name);
			printf("Quantity %d, ",current->quantity);
			printf("Peso: %d, ", current->peso);
			printf("E' pronto? %d\n",current->attesa);
			current=current->next;
	}

	}


}

void aggiungi_ricetta(char *line)
{
    char *token;
    char *name = NULL, *ingredient = NULL;
    Recipe *currecipe;
    int quantity;
    int peso = 0;

    // Parse the recipe name
    token = strtok(line, " \n");
    if (token != NULL)
    {
        name = malloc(strlen(token) + 1); // Allocate memory for name
        strcpy(name, token);

        if (findRecipe(name) != NULL)
        {
            printf("ignorato\n");
            free(name); // Free memory if recipe is ignored
        }
        else
        {
            currecipe = insertRecipe(name); // insertRecipe will handle the name's memory

            // Parse the rest of the line
            while ((token = strtok(NULL, " \n")) != NULL)
            {
                ingredient = malloc(strlen(token) + 1); // Allocate memory for ingredient
                strcpy(ingredient, token);

                token = strtok(NULL, " \n");
                if (token != NULL)
                {
                    quantity = atoi(token);
                    peso = peso + quantity;
                    insertIngredient(currecipe, ingredient, quantity);
                }
                free(ingredient); // Free ingredient memory after it's used
            }
            currecipe->peso = peso;
            printf("aggiunta\n");
        }
    }
}

void rimuovi_ricetta(char *line)
{
    char *token;
    char *name = NULL;

    token = strtok(line, " \n");
    if (token != NULL)
    {
        name = malloc(strlen(token) + 1); // Allocate memory for name
        strcpy(name, token);

        deleteRecipe(name); // deleteRecipe will handle the name's memory
        free(name); // Free the memory after deleting the recipe
    }
}

void rifornimento(char *line)
{
    char *token;
    int quantity = 0, scadenza = 0;
    char *name = NULL;

    token = strtok(line, " \n");
    while (token != NULL)
    {
        name = malloc(strlen(token) + 1); // Allocate memory for name
        strcpy(name, token);

        token = strtok(NULL, " \n");
        if (token != NULL)
        {
            quantity = atoi(token);
        }
        token = strtok(NULL, " \n");
        if (token != NULL)
        {
            scadenza = atoi(token);
        }

        insertIngredientRif(name, quantity, scadenza);
        free(name); // Free name after it's used

        token = strtok(NULL, " \n");
    }
    printf("rifornito\n");
}

void ordine(char* line,int lcount){
	char *token, *name=NULL;
	int quantity;
	Recipe* rec;
	token=strtok(line," \n");
	if (token!=NULL){
		name=malloc(strlen(token)+1);
		strcpy(name, token);
		rec=findRecipe(name);
		if (rec!=NULL){
			token=strtok(NULL, " \n");
			if (token !=NULL){
				quantity=atoi(token);
				printf("accettato\n");
				inserisciOrdine(lcount,name,quantity,rec->peso*quantity);
				free(name);
				return;
			}
		}
		else{
			printf("rifiutato\n");
		}
		free(name);
	}
}

void elimIngredientRif(char *name, int quantity) {
    int index = hash(name);
    int count = quantity;
    IngredientRif *current = hashRif[index];
    IngredientRif *prev = NULL;
    
    while (current != NULL && count > 0) {
        if (strcmp(current->name, name) == 0) {
            if (current->quantity <= count) {
                // Reduce the count by the quantity of the current node
                count=count-current->quantity;
				IngredientRif*temp=current;

                // Remove the current node
                if (prev != NULL) {
                    prev->next = current->next;
                } else {
                    hashRif[index] = current->next;
                }
                current = current->next;
				free(temp->name);
                free(temp);
            } else {
                // Reduce the quantity of the current node
                current->quantity=current->quantity-count;
                count = 0;
            }
        } else {
            // Move to the next node
            prev = current;
            current = current->next;
        }
    }
    
    return;
}

void elimIngredientRifscad(int line) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        IngredientRif *current = hashRif[i];

        while (current != NULL) {
            if (current->scadenza <= line) {
                // Node to be deleted
                IngredientRif *temp = current;
				// Deleting the head of the list
				hashRif[i] = current->next;
                current = current->next; // Move current before freeing
				free(temp->name);
                free(temp); // Free the memory of the deleted node
            }
			else{
				break;
			}
        }
    }
}

int checkIngredient(char *name,int quantity){
	int index = hash(name);
	int count=quantity;
	IngredientRif* ingredientrif;
	ingredientrif = hashRif[index];
	while (ingredientrif!=NULL&&count>0){
		if (strcmp(ingredientrif->name,name)==0){
			count=count-(ingredientrif->quantity);
			ingredientrif=ingredientrif->next;
		}
		else{
			ingredientrif=ingredientrif->next;
		}
	}
	if (count<=0){//c'è abbastanza ingrediente
		return 0;
	}
	else{
		return 1;
	}


}

int checkOrder(char *name, int quantity) {
    Ingredient *curIngredient;
    int index = hash(name);
    Recipe *curRecipe = hashTable[index];

    while (curRecipe) {
        if (strcmp(curRecipe->name, name) == 0) {
            curIngredient = curRecipe->ingredients;
            while (curIngredient != NULL) {
                if (checkIngredient(curIngredient->name, (curIngredient->quantity) * quantity) != 0) {
                    return 1;
                }
                curIngredient = curIngredient->next;
            }

            curIngredient = curRecipe->ingredients;
            while (curIngredient != NULL) {
                elimIngredientRif(curIngredient->name, (curIngredient->quantity) * quantity);
                curIngredient = curIngredient->next;
            }

            return 0;
        } else {
            curRecipe = curRecipe->next;
        }
    }

    return 1;
}

void insert_ordinipronti(Ordine* node){
	Ordine* current=ordiniprontihead;
	Ordine *prev=NULL;
	while (current && node->arriv_time>current->arriv_time){
		prev=current;
		current=current->next;
	}
	if (prev){
		prev->next=node;
		node->next=current;
	}
	else{
		node->next=ordiniprontihead;
		ordiniprontihead=node;
	}
}

void ordinipronti(){
	Ordine *prev= NULL;
	Ordine* curOrdine=ordinihead;
	while (curOrdine!=NULL){
		if(checkOrder(curOrdine->name,curOrdine->quantity)==0){
			curOrdine->attesa=1;
			if (prev == NULL ){
				ordinihead = curOrdine->next;
			}
			else {
				prev->next = curOrdine->next;
			}

			if (curOrdine == ordinitail) {
                ordinitail = prev; // Update the tail to the previous node
            }

            if (ordinihead == NULL) {
                ordinitail = NULL; // If list is empty, tail should be NULL too
            }

			Ordine* next = curOrdine->next;
			insert_ordinipronti(curOrdine);
			curOrdine=next;

		}
		else{
			prev=curOrdine;
			curOrdine=curOrdine->next;
		}
		
	}
}

Ordine* insert_da_mandare(Ordine* damandarehead, Ordine* node) {
	Ordine* current=damandarehead;
	Ordine* prev=NULL;
	while (current && (node->peso<current->peso||(node->peso==current->peso&&node->arriv_time>current->arriv_time))){
		prev=current;
		current=current->next;
	}
	if (prev){
		prev->next=node;
		node->next=current;
	}
	else{
		node->next=damandarehead;
		damandarehead=node;
	}
	return damandarehead;
}

void displaydamandare(Ordine* damandarehead){
	Ordine* current = damandarehead;
	if (damandarehead==NULL){
		printf("camioncino vuoto\n");
		return;
	}

    while (current != NULL) {
        // Print details of the current node
        printf("%d %s %d\n",
               current->arriv_time, current->name, current->quantity);
        Ordine* next = current->next;
		free(current->name);
        free(current);
        current = next;
    }
	return;

}

void camioncino(int max) {
    int count = 0;
    Ordine* damandarehead = NULL;
    Ordine* current = ordiniprontihead;
    Ordine* prev = NULL;

    while (current != NULL) {
		if (count + current->peso <= max) { // Check if it fits in the truck
			count += current->peso; // Increase the truck's load with the order's weight

			// Handle removing the current node
			if (prev == NULL) {
				// We are removing the head
				ordiniprontihead = current->next;
			} else {
				// We are removing a middle or tail node
				prev->next = current->next;
			}

			Ordine* next = current->next;
			current->next = NULL;
			damandarehead = insert_da_mandare(damandarehead, current);
			current = next;
		} else {
			break; // Exit the loop if the order cannot fit in the truck
		}
    }
    displaydamandare(damandarehead);
}

void freeAllMemory() {
    // Free all recipes and their ingredients
    for (int i = 0; i < TABLE_SIZE; i++) {
        Recipe *currentRecipe = hashTable[i];
        while (currentRecipe) {
            Recipe *tempRecipe = currentRecipe;
            currentRecipe = currentRecipe->next;

            // Free the recipe name
            free(tempRecipe->name);

            // Free ingredients of the current recipe
            Ingredient *currentIngredient = tempRecipe->ingredients;
            while (currentIngredient) {
                Ingredient *tempIngredient = currentIngredient;
                currentIngredient = currentIngredient->next;

                // Free the ingredient name
                free(tempIngredient->name);

                free(tempIngredient); // Free ingredient node
            }

            free(tempRecipe); // Free recipe node
        }
    }


    for (int i = 0; i < TABLE_SIZE; i++) {
        IngredientRif *currentRif = hashRif[i];
        while (currentRif) {
            IngredientRif *tempRif = currentRif;
            currentRif = currentRif->next;

            // Free the ingredient name
            free(tempRif->name);

            free(tempRif); // Free ingredient reference node
        }
    }

    // Free all orders
    Ordine *currentOrdine = ordinihead;
    while (currentOrdine) {
        Ordine *tempOrdine = currentOrdine;
        currentOrdine = currentOrdine->next;

        // Free the order name
        free(tempOrdine->name);

        free(tempOrdine); // Free order node
    }
}

int main()
{
	int t, cap, lcount=-1;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int c=0;

	// Initialize hash table
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		hashTable[i] = NULL;
		hashRif[i]=NULL;
	}
	
	if (scanf("%d", &t) != 1) {
		fprintf(stderr, "Error reading input for 't'.\n");
		exit(EXIT_FAILURE); // Handle error appropriately
	}

	if (scanf("%d", &cap) != 1) {
		fprintf(stderr, "Error reading input for 'cap'.\n");
		exit(EXIT_FAILURE); // Handle error appropriately
	}
	getchar();
	
	while ((read = getline(&line, &len, stdin)) != -1 && c==0)
	{
		lcount++;
		elimIngredientRifscad(lcount);
		if (lcount%t==0&&lcount!=0){
			camioncino(cap);
		}

		char *firstWord = strtok(line, " \n");
		if (firstWord == NULL && c==1){
			break;
		}
		else if (strcmp(firstWord, "aggiungi_ricetta") == 0)
		{
			// Pass the remaining part of the line after the command
			aggiungi_ricetta(line + strlen(firstWord) + 1); // +1 to skip space after command
		}
		else if (strcmp(firstWord, "rimuovi_ricetta") == 0)
		{
			rimuovi_ricetta(line + strlen(firstWord) + 1);
		}
		else if (strcmp(firstWord, "rifornimento") == 0)
		{
			rifornimento(line + strlen(firstWord)+1);
			ordinipronti();
		}
		else if (strcmp(firstWord, "ordine")==0){
			ordine(line + strlen(firstWord)+1, lcount);
			ordinipronti();
		}
	//displayOrdine();
	//displayRecipes();
	//displayRif();
	}
	lcount++;
	elimIngredientRifscad(lcount);
	if (lcount%t==0&&lcount!=0){
		camioncino(cap);
	}

	freeAllMemory();
	return 0;
}
