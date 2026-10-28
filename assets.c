/*
 * assets.c - Asset Management module
 * Municipal Financial Management System (MFMS) - Project A
 *
 * Data is stored in parallel arrays: index i refers to the same asset
 * in every array.
 */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "assets.h"

#define TEXT_SIZE 50

static int assetIds[MAX_ASSETS];
static char assetNames[MAX_ASSETS][TEXT_SIZE];
static char assetTypes[MAX_ASSETS][TEXT_SIZE];
static double assetValues[MAX_ASSETS];
static char assetDepartments[MAX_ASSETS][TEXT_SIZE];
static char assetConditions[MAX_ASSETS][TEXT_SIZE];
static int assetCount = 0;

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

static int readInt(const char *prompt)
{
    char line[64];
    char extra;
    int number;
    int result;

    result = readLine(prompt, line, sizeof(line));
    if (result == 0) return -2;
    if (result < 0 || sscanf(line, "%d %c", &number, &extra) != 1) {
        printf("Enter a whole number.\n");
        return -1;
    }
    return number;
}

static int readValue(const char *prompt, double *value)
{
    char line[128];
    char extra;
    double number;
    int result;

    while (1) {
        result = readLine(prompt, line, sizeof(line));
        if (result == 0) return 0;
        if (result < 0) continue;
        if (sscanf(line, "%lf %c", &number, &extra) != 1 ||
            !isfinite(number) || number <= 0) {
            printf("Enter a number greater than zero.\n");
        } else {
            *value = number;
            return 1;
        }
    }
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

static int containsText(const char *text, const char *search)
{
    int i, j;

    for (i = 0; text[i] != '\0'; i++) {
        for (j = 0; search[j] != '\0'; j++) {
            if (text[i + j] == '\0' ||
                tolower((unsigned char)text[i + j]) !=
                tolower((unsigned char)search[j])) {
                break;
            }
        }
        if (search[j] == '\0') return 1;
    }
    return search[0] == '\0';
}

int findAssetById(int id)
{
    int i;
    for (i = 0; i < assetCount; i++) {
        if (assetIds[i] == id) return i;
    }
    return -1;
}

int getAssetCount(void)
{
    return assetCount;
}

double getTotalAssetValue(void)
{
    double total = 0;
    int i;

    for (i = 0; i < assetCount; i++) total += assetValues[i];
    return total;
}

static int chooseFromList(const char *title, const char *options[],
                          int optionCount, char *answer)
{
    int choice;
    int i;

    printf("%s\n", title);
    for (i = 0; i < optionCount; i++) {
        printf("%d. %s\n", i + 1, options[i]);
    }

    while (1) {
        choice = readInt("Choose: ");
        if (choice == -2) return 0;
        if (choice >= 1 && choice <= optionCount) {
            strcpy(answer, options[choice - 1]);
            return 1;
        }
        printf("Choose a number from 1 to %d.\n", optionCount);
    }
}

void addAsset(void)
{
    static const char *types[] = {
        "Vehicle", "Computer", "Building", "Equipment", "Office Furniture", "Other"
    };
    static const char *conditions[] = {"Good", "Fair", "Poor"};
    int id;

    if (assetCount == MAX_ASSETS) {
        printf("The asset list is full.\n");
        return;
    }

    while (1) {
        id = readInt("Asset ID: ");
        if (id == -2) return;
        if (id <= 0) {
            printf("ID must be greater than zero.\n");
        } else if (findAssetById(id) >= 0) {
            printf("That ID is already in use.\n");
        } else {
            break;
        }
    }

    assetIds[assetCount] = id;
    if (!readText("Asset name: ", assetNames[assetCount], TEXT_SIZE)) return;
    if (!chooseFromList("Asset type:", types, 6, assetTypes[assetCount])) return;
    if (!readValue("Purchase value (N$): ", &assetValues[assetCount])) return;
    if (!readText("Department: ", assetDepartments[assetCount], TEXT_SIZE)) return;
    if (!chooseFromList("Condition:", conditions, 3,
                        assetConditions[assetCount])) return;

    assetCount++;
    printf("Asset added.\n");
}

static void printAssetHeading(void)
{
    printf("\n%-6s %-22s %-17s %-14s %-18s %-10s\n",
           "ID", "Name", "Type", "Value (N$)", "Department", "Condition");
}

static void printAsset(int i)
{
    printf("%-6d %-22.22s %-17.17s %-14.2f %-18.18s %-10s\n",
           assetIds[i], assetNames[i], assetTypes[i], assetValues[i],
           assetDepartments[i], assetConditions[i]);
}

void displayAssets(void)
{
    int i;

    if (assetCount == 0) {
        printf("No assets have been added.\n");
        return;
    }

    printAssetHeading();
    for (i = 0; i < assetCount; i++) printAsset(i);
}

void searchAsset(void)
{
    char search[128];
    int choice, id, i, found = 0;

    if (assetCount == 0) {
        printf("No assets have been added.\n");
        return;
    }

    printf("\n1. Search by ID\n2. Search by name\n");
    printf("3. Search by type\n4. Search by department\n");
    choice = readInt("Choose: ");
    if (choice == -2) return;

    if (choice == 1) {
        id = readInt("Asset ID: ");
        if (id == -2) return;
        i = findAssetById(id);
        if (i >= 0) {
            printAssetHeading();
            printAsset(i);
            return;
        }
    } else if (choice >= 2 && choice <= 4) {
        const char *prompt = choice == 2 ? "Name: " :
                             choice == 3 ? "Type: " : "Department: ";
        if (!readText(prompt, search, sizeof(search))) return;

        for (i = 0; i < assetCount; i++) {
            const char *field = choice == 2 ? assetNames[i] :
                                choice == 3 ? assetTypes[i] : assetDepartments[i];
            int matches = choice == 2 ? containsText(field, search) :
                                        sameText(field, search);
            if (matches) {
                if (found == 0) printAssetHeading();
                printAsset(i);
                found++;
            }
        }
        if (found > 0) return;
    } else {
        printf("Invalid choice.\n");
        return;
    }

    printf("No matching asset found.\n");
}

void displayAssetReport(void)
{
    int i, good = 0, fair = 0, poor = 0;

    printf("\n--- Asset Report ---\n");
    if (assetCount == 0) {
        printf("No assets have been added.\n");
        return;
    }

    displayAssets();
    for (i = 0; i < assetCount; i++) {
        if (strcmp(assetConditions[i], "Good") == 0) good++;
        else if (strcmp(assetConditions[i], "Fair") == 0) fair++;
        else poor++;
    }

    printf("\nNumber of assets: %d\n", assetCount);
    printf("Total value: N$%.2f\n", getTotalAssetValue());
    printf("Good: %d, Fair: %d, Poor: %d\n", good, fair, poor);
}

void assetMenu(void)
{
    int choice;

    do {
        printf("\n--- Asset Management ---\n");
        printf("1. Add asset\n2. Display assets\n3. Search assets\n4. Back\n");
        choice = readInt("Choose: ");
        if (choice == -2) return;

        switch (choice) {
            case 1: addAsset(); break;
            case 2: displayAssets(); break;
            case 3: searchAsset(); break;
            case 4: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}
