#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "supplier_management.h"

static char supplierIds[MAX_SUPPLIERS][SUPPLIER_ID_LEN];
static char supplierNames[MAX_SUPPLIERS][SUPPLIER_NAME_LEN];
static char supplierEmails[MAX_SUPPLIERS][SUPPLIER_EMAIL_LEN];
static char supplierPhones[MAX_SUPPLIERS][SUPPLIER_PHONE_LEN];
static char supplierTowns[MAX_SUPPLIERS][SUPPLIER_TOWN_LEN];
static int supplierCount = 0;

static int readLine(const char *prompt, char *text, int size)
{
    size_t length;
    int ch;

    printf("%s", prompt);
    if (fgets(text, size, stdin) == NULL) return 0;
    length = strlen(text);

    if (length > 0 && text[length - 1] == '\n') {
        text[length - 1] = '\0';
    } else if (!feof(stdin)) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Clear the rest of the line. */
        }
        printf("Input is too long. Try again.\n");
        return -1;
    }
    return 1;
}

static int readText(const char *prompt, char *answer, int size)
{
    char line[128];
    char *start;
    size_t length;
    int result;

    while (1) {
        result = readLine(prompt, line, sizeof(line));
        if (result == 0) return 0;
        if (result < 0) continue;

        start = line;
        while (isspace((unsigned char)*start)) start++;
        length = strlen(start);
        while (length > 0 && isspace((unsigned char)start[length - 1])) {
            start[--length] = '\0';
        }

        if (length == 0) {
            printf("This cannot be left empty.\n");
        } else if (length >= (size_t)size) {
            printf("Please use fewer characters.\n");
        } else {
            strcpy(answer, start);
            return 1;
        }
    }
}

static int readChoice(const char *prompt)
{
    char line[64];
    char extra;
    int choice;
    int result;

    result = readLine(prompt, line, sizeof(line));
    if (result == 0) return -2;
    if (result < 0 || sscanf(line, "%d %c", &choice, &extra) != 1) return -1;
    return choice;
}

static int sameText(const char *first, const char *second)
{
    while (*first && *second) {
        if (tolower((unsigned char)*first) != tolower((unsigned char)*second)) {
            return 0;
        }
        first++;
        second++;
    }
    return *first == '\0' && *second == '\0';
}

static int containsName(const char *name, const char *search)
{
    int i, j;

    for (i = 0; name[i] != '\0'; i++) {
        for (j = 0; search[j] != '\0'; j++) {
            if (name[i + j] == '\0' ||
                tolower((unsigned char)name[i + j]) !=
                tolower((unsigned char)search[j])) {
                break;
            }
        }
        if (search[j] == '\0') return 1;
    }
    return search[0] == '\0';
}

static int findSupplier(const char *id)
{
    int i;
    for (i = 0; i < supplierCount; i++) {
        if (sameText(supplierIds[i], id)) return i;
    }
    return -1;
}

int getSupplierCount(void)
{
    return supplierCount;
}

void addSupplier(void)
{
    char id[SUPPLIER_ID_LEN];

    if (supplierCount == MAX_SUPPLIERS) {
        printf("The supplier list is full.\n");
        return;
    }
    if (!readText("Supplier ID: ", id, sizeof(id))) return;
    if (findSupplier(id) >= 0) {
        printf("That supplier ID is already in use.\n");
        return;
    }
    if (!readText("Supplier name: ", supplierNames[supplierCount],
                  SUPPLIER_NAME_LEN)) return;
    if (!readText("Email: ", supplierEmails[supplierCount],
                  SUPPLIER_EMAIL_LEN)) return;
    if (!readText("Telephone: ", supplierPhones[supplierCount],
                  SUPPLIER_PHONE_LEN)) return;
    if (!readText("Town or location: ", supplierTowns[supplierCount],
                  SUPPLIER_TOWN_LEN)) return;

    strcpy(supplierIds[supplierCount], id);
    supplierCount++;
    printf("Supplier added.\n");
}

void displaySupplier(void)
{
    int i;

    if (supplierCount == 0) {
        printf("No suppliers have been added.\n");
        return;
    }
    for (i = 0; i < supplierCount; i++) {
        printf("\nSupplier %d\n", i + 1);
        printf("ID: %s\nName: %s\nEmail: %s\n", supplierIds[i],
               supplierNames[i], supplierEmails[i]);
        printf("Telephone: %s\nTown: %s\n",
               supplierPhones[i], supplierTowns[i]);
    }
}

void searchSupplier(void)
{
    char search[SUPPLIER_NAME_LEN];
    int choice, i, found = 0;

    if (supplierCount == 0) {
        printf("No suppliers have been added.\n");
        return;
    }

    printf("\n1. Search by ID\n2. Search by name\n");
    choice = readChoice("Choose: ");
    if (choice == -2) return;
    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n");
        return;
    }

    if (!readText(choice == 1 ? "Supplier ID: " : "Name to search for: ",
                  search, sizeof(search))) return;

    if (choice == 1) {
        i = findSupplier(search);
        if (i >= 0) {
            printf("\n%s, %s, %s, %s\n", supplierNames[i],
                   supplierEmails[i], supplierPhones[i], supplierTowns[i]);
            return;
        }
    } else {
        for (i = 0; i < supplierCount; i++) {
            if (containsName(supplierNames[i], search)) {
                printf("\nID: %s\nName: %s\nEmail: %s\nTelephone: %s\nTown: %s\n",
                       supplierIds[i], supplierNames[i], supplierEmails[i],
                       supplierPhones[i], supplierTowns[i]);
                found++;
            }
        }
        if (found > 0) return;
    }
    printf("No matching supplier found.\n");
}

void supplierMenu(void)
{
    int choice;

    do {
        printf("\n--- Supplier Management ---\n");
        printf("1. Add supplier\n2. Display suppliers\n3. Search suppliers\n4. Back\n");
        choice = readChoice("Choose: ");
        if (choice == -2) return;

        switch (choice) {
            case 1: addSupplier(); break;
            case 2: displaySupplier(); break;
            case 3: searchSupplier(); break;
            case 4: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

