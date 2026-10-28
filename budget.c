#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "budget.h"

static char departments[MAX_DEPARTMENTS][50];
static double budgets[MAX_DEPARTMENTS];
static double spent[MAX_DEPARTMENTS];
static int departmentCount = 0;

static int readLine(const char *prompt, char *text, int size)
{
    int ch;
    size_t length;

    printf("%s", prompt);
    if (fgets(text, size, stdin) == NULL) {
        return 0;
    }

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
        if (result == 0) {
            return 0;
        }
        if (result < 0) {
            continue;
        }

        start = line;
        while (isspace((unsigned char)*start)) {
            start++;
        }
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

static int readAmount(const char *prompt, double *amount)
{
    char line[128];
    char extra;
    double value;
    int result;

    while (1) {
        result = readLine(prompt, line, sizeof(line));
        if (result == 0) {
            return 0;
        }
        if (result < 0) {
            continue;
        }
        if (sscanf(line, "%lf %c", &value, &extra) != 1 ||
            !isfinite(value) || value < 0) {
            printf("Enter a valid amount that is not negative.\n");
        } else {
            *amount = value;
            return 1;
        }
    }
}

static int sameName(const char *first, const char *second)
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

static int findDepartment(const char *name)
{
    int i;

    for (i = 0; i < departmentCount; i++) {
        if (sameName(departments[i], name)) {
            return i;
        }
    }
    return -1;
}

void addBudget(void)
{
    char name[50];
    double amount;

    if (departmentCount == MAX_DEPARTMENTS) {
        printf("The department list is full.\n");
        return;
    }
    if (!readText("Department name: ", name, sizeof(name))) {
        return;
    }
    if (findDepartment(name) >= 0) {
        printf("A budget already exists for that department.\n");
        return;
    }
    if (!readAmount("Allocated budget (N$): ", &amount)) {
        return;
    }

    strcpy(departments[departmentCount], name);
    budgets[departmentCount] = amount;
    spent[departmentCount] = 0;
    departmentCount++;
    printf("Budget added.\n");
}

void enterExpenditure(void)
{
    char name[50];
    double amount;
    int index;

    if (departmentCount == 0) {
        printf("Add a department budget first.\n");
        return;
    }
    if (!readText("Department name: ", name, sizeof(name))) {
        return;
    }

    index = findDepartment(name);
    if (index < 0) {
        printf("Department not found.\n");
        return;
    }
    if (!readAmount("Expenditure (N$): ", &amount)) {
        return;
    }

    spent[index] += amount;
    printf("Expenditure recorded.\n");
}

void displayBudgets(void)
{
    int i;
    double remaining;

    if (departmentCount == 0) {
        printf("No department budgets have been added.\n");
        return;
    }

    for (i = 0; i < departmentCount; i++) {
        remaining = budgets[i] - spent[i];
        printf("\nDepartment: %s\n", departments[i]);
        printf("Budget: N$%.2f\n", budgets[i]);
        printf("Spent: N$%.2f\n", spent[i]);
        printf("Remaining: N$%.2f\n", remaining);
        printf("Status: %s\n", remaining >= 0 ? "Within budget" : "Over budget");
    }
}

void generateReport(void)
{
    int i;
    double totalBudget = 0;
    double totalSpent = 0;

    if (departmentCount == 0) {
        printf("No department budgets have been added.\n");
        return;
    }

    for (i = 0; i < departmentCount; i++) {
        totalBudget += budgets[i];
        totalSpent += spent[i];
    }

    printf("\n--- Budget Report ---\n");
    printf("Total budget: N$%.2f\n", totalBudget);
    printf("Total spent: N$%.2f\n", totalSpent);
    printf("Remaining: N$%.2f\n", totalBudget - totalSpent);

    for (i = 0; i < departmentCount; i++) {
        if (spent[i] > budgets[i]) {
            printf("%s is over budget.\n", departments[i]);
        }
    }
}
