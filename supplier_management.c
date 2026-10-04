
#include <stdio.h>
#include <string.h>
#include "supplier.h"

#define MAX_SUPPLIERS 5

char supEmail[MAX_SUPPLIERS][100];   
char supName[MAX_SUPPLIERS][100];
char supPhone[MAX_SUPPLIERS][30];
char supTown[MAX_SUPPLIERS][50];     
int  supCount = 0;                   /* suppliers stored so far */

/* Reads a line of text and removes the Enter key */
void readSupplierText(char text[], int size)
{
    fgets(text, size, stdin);
    text[strcspn(text, "\n")] = '\0';
}

/* Adds one supplier */
void addSupplier()
{
    if (supCount >= MAX_SUPPLIERS) {
        printf("Supplier list is full.\n");
        return;
    }
    printf("Enter supplier name: ");  readSupplierText(supName[supCount], 100);
    printf("Enter email: ");          readSupplierText(supEmail[supCount], 100);
    printf("Enter phone: ");          readSupplierText(supPhone[supCount], 30);
    printf("Enter town: ");           readSupplierText(supTown[supCount], 50);
    supCount++;
    printf("Supplier added.\n");
}

/* Shows all suppliers */
void displaySupplier()
{
    int i;
    if (supCount == 0) {
        printf("No suppliers yet.\n");
        return;
    }
    for (i = 0; i < supCount; i++) {
        printf("\n--- SUPPLIER %d ---\n", i + 1);
        printf("Name : %s\nEmail: %s\nPhone: %s\nTown : %s\n",
               supName[i], supEmail[i], supPhone[i], supTown[i]);
    }
}

/* Finds a supplier by exact name */
void searchSupplier()
{
    char search[100];
    int i;
    printf("Enter supplier name to search: ");
    readSupplierText(search, 100);
    for (i = 0; i < supCount; i++) {
        if (strcmp(supName[i], search) == 0) {
            printf("Supplier found.\nEmail: %s\nPhone: %s\nTown : %s\n",
                   supEmail[i], supPhone[i], supTown[i]);
            return;
        }
    }
    printf("Supplier not found.\n");
}

/* Supplier sub-menu: call this from the main menu option "Supplier Management" */
void supplierMenu()
{
    char line[10];
    int choice;
    do {
        printf("\n1. Add Supplier\n2. Display Supplier\n");
        printf("3. Search Supplier\n4. Back\nEnter choice: ");
        readSupplierText(line, 10);
        choice = (strlen(line) == 1) ? line[0] - '0' : 0;
        if (choice == 1) addSupplier();
        else if (choice == 2) displaySupplier();
        else if (choice == 3) searchSupplier();
        else if (choice != 4) printf("Invalid choice.\n");
    } while (choice != 4);
}
