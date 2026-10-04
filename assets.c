/*
 * assets.c - Asset Management module
 * Municipal Financial Management System (MFMS) - Project A
 *
 * Data is stored in parallel arrays: index i refers to the same asset
 * in every array.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "assets.h"

#define MAX_ASSETS 100
#define NAME_LEN   50
#define INPUT_LEN  100

/* ---------- Storage (parallel arrays) ---------- */
static int    assetId[MAX_ASSETS];
static char   assetName[MAX_ASSETS][NAME_LEN];
static char   assetType[MAX_ASSETS][NAME_LEN];
static double assetValue[MAX_ASSETS];
static char   assetDept[MAX_ASSETS][NAME_LEN];
static char   assetCondition[MAX_ASSETS][NAME_LEN];
static int    assetCount = 0;

/* ---------- Private helper prototypes ---------- */
static void   readLine(const char *prompt, char *buffer, int size);
static void   readText(const char *prompt, char *dest, int size);
static int    readInt(const char *prompt);
static double readPositiveDouble(const char *prompt);
static void   toLowerCase(const char *src, char *dest, int size);
static int    containsIgnoreCase(const char *text, const char *term);
static int    equalsIgnoreCase(const char *a, const char *b);
static void   chooseType(char *dest);
static void   chooseCondition(char *dest);
static void   printAssetHeader(void);
static void   printAssetRow(int i);

/* =====================================================
 *  INPUT HELPERS
 * ===================================================== */

/* Reads one line safely with fgets and removes the newline. */
static void readLine(const char *prompt, char *buffer, int size)
{
    int len;
    int ch;

    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    len = (int)strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        /* line was longer than the buffer: discard the leftovers */
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard */
        }
    }
}

/* Keeps asking until the user types a non-empty text (no empty names). */
static void readText(const char *prompt, char *dest, int size)
{
    char buffer[INPUT_LEN];
    int  start;

    while (1) {
        readLine(prompt, buffer, INPUT_LEN);

        /* skip leading spaces */
        start = 0;
        while (buffer[start] == ' ' || buffer[start] == '\t') {
            start++;
        }

        if (strlen(buffer + start) == 0) {
            printf("  Error: this field cannot be empty.\n");
        } else if ((int)strlen(buffer + start) >= size) {
            printf("  Error: maximum %d characters allowed.\n", size - 1);
        } else {
            strcpy(dest, buffer + start);
            return;
        }
    }
}

/* Reads a whole number; repeats until the input is a valid integer. */
static int readInt(const char *prompt)
{
    char  buffer[INPUT_LEN];
    char *end;
    long  value;

    while (1) {
        readLine(prompt, buffer, INPUT_LEN);
        value = strtol(buffer, &end, 10);

        if (end == buffer) {
            printf("  Error: please enter a valid whole number.\n");
            continue;
        }
        while (*end == ' ' || *end == '\t') {
            end++;
        }
        if (*end != '\0') {
            printf("  Error: please enter a valid whole number.\n");
            continue;
        }
        return (int)value;
    }
}

/* Reads a decimal number greater than 0. */
static double readPositiveDouble(const char *prompt)
{
    char   buffer[INPUT_LEN];
    char  *end;
    double value;

    while (1) {
        readLine(prompt, buffer, INPUT_LEN);
        value = strtod(buffer, &end);

        if (end == buffer) {
            printf("  Error: please enter a valid number.\n");
            continue;
        }
        while (*end == ' ' || *end == '\t') {
            end++;
        }
        if (*end != '\0') {
            printf("  Error: please enter a valid number.\n");
            continue;
        }
        if (value <= 0) {
            printf("  Error: value must be greater than 0.\n");
            continue;
        }
        return value;
    }
}

/* =====================================================
 *  STRING HELPERS
 * ===================================================== */

/* Copies src into dest in lowercase. */
static void toLowerCase(const char *src, char *dest, int size)
{
    int i;
    for (i = 0; src[i] != '\0' && i < size - 1; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

/* Case-insensitive "text contains term" (partial match). */
static int containsIgnoreCase(const char *text, const char *term)
{
    char lowText[NAME_LEN];
    char lowTerm[INPUT_LEN];

    toLowerCase(text, lowText, NAME_LEN);
    toLowerCase(term, lowTerm, INPUT_LEN);
    return strstr(lowText, lowTerm) != NULL;
}

/* Case-insensitive exact match using strcmp. */
static int equalsIgnoreCase(const char *a, const char *b)
{
    char lowA[INPUT_LEN];
    char lowB[INPUT_LEN];

    toLowerCase(a, lowA, INPUT_LEN);
    toLowerCase(b, lowB, INPUT_LEN);
    return strcmp(lowA, lowB) == 0;
}

/* =====================================================
 *  MENUS FOR TYPE AND CONDITION
 * ===================================================== */

static void chooseType(char *dest)
{
    int choice;

    printf("  Asset type:\n");
    printf("    1. Vehicle\n");
    printf("    2. Computer\n");
    printf("    3. Building\n");
    printf("    4. Equipment\n");
    printf("    5. Office Furniture\n");
    printf("    6. Other\n");

    while (1) {
        choice = readInt("  Select type (1-6): ");
        switch (choice) {
            case 1: strcpy(dest, "Vehicle");          return;
            case 2: strcpy(dest, "Computer");         return;
            case 3: strcpy(dest, "Building");         return;
            case 4: strcpy(dest, "Equipment");        return;
            case 5: strcpy(dest, "Office Furniture"); return;
            case 6: strcpy(dest, "Other");            return;
            default:
                printf("  Error: choose a number from 1 to 6.\n");
        }
    }
}

static void chooseCondition(char *dest)
{
    int choice;

    printf("  Condition:\n");
    printf("    1. Good\n");
    printf("    2. Fair\n");
    printf("    3. Poor\n");

    while (1) {
        choice = readInt("  Select condition (1-3): ");
        switch (choice) {
            case 1: strcpy(dest, "Good"); return;
            case 2: strcpy(dest, "Fair"); return;
            case 3: strcpy(dest, "Poor"); return;
            default:
                printf("  Error: choose a number from 1 to 3.\n");
        }
    }
}

/* =====================================================
 *  DISPLAY HELPERS
 * ===================================================== */

static void printAssetHeader(void)
{
    printf("\n%-6s %-22s %-17s %-14s %-18s %-10s\n",
           "ID", "Name", "Type", "Value (N$)", "Department", "Condition");
    printf("--------------------------------------------------------------------------------------\n");
}

static void printAssetRow(int i)
{
    printf("%-6d %-22.22s %-17.17s %-14.2f %-18.18s %-10s\n",
           assetId[i], assetName[i], assetType[i],
           assetValue[i], assetDept[i], assetCondition[i]);
}

/* =====================================================
 *  CORE FUNCTIONS
 * ===================================================== */

/* Returns the index of the asset with this ID, or -1 if not found. */
int findAssetById(int id)
{
    int i;
    for (i = 0; i < assetCount; i++) {
        if (assetId[i] == id) {
            return i;
        }
    }
    return -1;
}

int getAssetCount(void)
{
    return assetCount;
}

double getTotalAssetValue(void)
{
    double total = 0.0;
    int i;
    for (i = 0; i < assetCount; i++) {
        total += assetValue[i];
    }
    return total;
}

void addAsset(void)
{
    int id;

    printf("\n--- ADD ASSET ---\n");

    if (assetCount >= MAX_ASSETS) {
        printf("Error: the asset register is full (%d assets).\n", MAX_ASSETS);
        return;
    }

    /* Asset ID: positive and unique */
    while (1) {
        id = readInt("Asset ID: ");
        if (id <= 0) {
            printf("  Error: ID must be a positive number.\n");
        } else if (findAssetById(id) != -1) {
            printf("  Error: an asset with ID %d already exists.\n", id);
        } else {
            break;
        }
    }

    assetId[assetCount] = id;
    readText("Asset name: ", assetName[assetCount], NAME_LEN);
    chooseType(assetType[assetCount]);
    assetValue[assetCount] = readPositiveDouble("Purchase value (N$): ");
    readText("Department: ", assetDept[assetCount], NAME_LEN);
    chooseCondition(assetCondition[assetCount]);

    assetCount++;
    printf("\nAsset added successfully.\n");
}

void displayAssets(void)
{
    int i;

    printf("\n--- ASSET REGISTER ---\n");

    if (assetCount == 0) {
        printf("No assets have been registered yet.\n");
        return;
    }

    printAssetHeader();
    for (i = 0; i < assetCount; i++) {
        printAssetRow(i);
    }
    printf("\nTotal assets: %d\n", assetCount);
}

void searchAsset(void)
{
    int  choice;
    int  i;
    int  id;
    int  found = 0;
    char term[INPUT_LEN];

    printf("\n--- SEARCH ASSET ---\n");
    printf("1. Search by Asset ID\n");
    printf("2. Search by Name (partial match allowed)\n");
    printf("3. Search by Type\n");
    printf("4. Search by Department\n");

    choice = readInt("Enter choice: ");

    switch (choice) {
        case 1:
            id = readInt("Enter Asset ID: ");
            i = findAssetById(id);
            if (i != -1) {
                printAssetHeader();
                printAssetRow(i);
                found = 1;
            }
            break;

        case 2:
            readText("Enter name to search: ", term, INPUT_LEN);
            for (i = 0; i < assetCount; i++) {
                if (containsIgnoreCase(assetName[i], term)) {
                    if (!found) {
                        printAssetHeader();
                    }
                    printAssetRow(i);
                    found++;
                }
            }
            break;

        case 3:
            readText("Enter type to search: ", term, INPUT_LEN);
            for (i = 0; i < assetCount; i++) {
                if (equalsIgnoreCase(assetType[i], term)) {
                    if (!found) {
                        printAssetHeader();
                    }
                    printAssetRow(i);
                    found++;
                }
            }
            break;

        case 4:
            readText("Enter department to search: ", term, INPUT_LEN);
            for (i = 0; i < assetCount; i++) {
                if (equalsIgnoreCase(assetDept[i], term)) {
                    if (!found) {
                        printAssetHeader();
                    }
                    printAssetRow(i);
                    found++;
                }
            }
            break;

        default:
            printf("Invalid search option.\n");
            return;
    }

    if (!found) {
        printf("No matching assets found.\n");
    } else {
        printf("\nMatches found: %d\n", found);
    }
}

/* Used by the Reports module (Student 5). */
void displayAssetReport(void)
{
    int i;
    int good = 0, fair = 0, poor = 0;

    printf("\n========================================\n");
    printf("              ASSET REPORT\n");
    printf("========================================\n");

    if (assetCount == 0) {
        printf("No assets have been registered yet.\n");
        return;
    }

    printAssetHeader();
    for (i = 0; i < assetCount; i++) {
        printAssetRow(i);

        if (strcmp(assetCondition[i], "Good") == 0) {
            good++;
        } else if (strcmp(assetCondition[i], "Fair") == 0) {
            fair++;
        } else {
            poor++;
        }
    }

    printf("\nTotal Assets      : %d\n", assetCount);
    printf("Total Asset Value : N$%.2f\n", getTotalAssetValue());
    printf("Good: %d | Fair: %d | Poor: %d\n", good, fair, poor);
    if (poor > 0) {
        printf("Note: %d asset(s) in poor condition may need attention.\n", poor);
    }
}

/* Sub-menu for asset management. Called from the main menu. */
void assetMenu(void)
{
    int choice;

    do {
        printf("\n========================================\n");
        printf("            ASSET MANAGEMENT\n");
        printf("========================================\n");
        printf("1. Add Asset\n");
        printf("2. Display All Assets\n");
        printf("3. Search Asset\n");
        printf("4. Back to Main Menu\n");

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: addAsset();      break;
            case 2: displayAssets(); break;
            case 3: searchAsset();   break;
            case 4: printf("Returning to main menu...\n"); break;
            default:
                printf("Invalid choice. Please enter 1-4.\n");
        }
    } while (choice != 4);
}
