#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "budget.h"

/* Each index contains one department's name, allocation, and expenditure. */
static char departments[MAX_DEPARTMENTS][50];
static double allocatedBudgets[MAX_DEPARTMENTS];
static double expenditures[MAX_DEPARTMENTS];
static int departmentCount;

/* Returns 1 for a line, 0 for EOF, and -1 when the line is too long. */
static int readLine(const char *prompt, char *buffer, size_t size)
{
    size_t length;
    int ch;

    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
        return 1;
    }
    if (!feof(stdin)) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard the rest of the overlong line */
        }
        printf("Input is too long. Please try again.\n");
        return -1;
    }
    return 1;
}

static int readRequiredText(const char *prompt, char *dest, size_t size)
{
    char buffer[128];
    char *start;
    size_t length;
    int status;

    for (;;) {
        status = readLine(prompt, buffer, sizeof(buffer));
        if (status == 0) {
            return 0;
        }
        if (status < 0) {
            continue;
        }

        start = buffer;
        while (isspace((unsigned char)*start)) {
            start++;
        }
        length = strlen(start);
        while (length > 0 && isspace((unsigned char)start[length - 1])) {
            start[--length] = '\0';
        }
        if (length == 0) {
            printf("This field cannot be empty.\n");
        } else if (length >= size) {
            printf("Please use no more than %lu characters.\n",
                   (unsigned long)(size - 1));
        } else {
            strcpy(dest, start);
            return 1;
        }
    }
}

static int readNonNegativeAmount(const char *prompt, double *amount)
{
    char buffer[128];
    char *end;
    double value;
    int status;

    for (;;) {
        status = readLine(prompt, buffer, sizeof(buffer));
        if (status == 0) {
            return 0;
        }
        if (status < 0) {
            continue;
        }

        errno = 0;
        value = strtod(buffer, &end);
        if (end == buffer || errno == ERANGE || !isfinite(value)) {
            printf("Please enter a valid finite number.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) {
            end++;
        }
        if (*end != '\0') {
            printf("Please enter a number without extra characters.\n");
        } else if (value < 0.0) {
            printf("Amount cannot be negative.\n");
        } else {
            *amount = value;
            return 1;
        }
    }
}

static int equalsIgnoreCase(const char *left, const char *right)
{
    while (*left != '\0' && *right != '\0') {
        if (tolower((unsigned char)*left) != tolower((unsigned char)*right)) {
            return 0;
        }
        left++;
        right++;
    }
    return *left == '\0' && *right == '\0';
}

static int findDepartment(const char *name)
{
    int i;

    for (i = 0; i < departmentCount; i++) {
        if (equalsIgnoreCase(departments[i], name)) {
            return i;
        }
    }
    return -1;
}

void addBudget(void)
{
    char department[50];
    double allocation;

    if (departmentCount >= MAX_DEPARTMENTS) {
        printf("Maximum number of departments reached.\n");
        return;
    }
    if (!readRequiredText("Enter department name: ", department,
                          sizeof(department))) {
        return;
    }
    if (findDepartment(department) >= 0) {
        printf("That department already has a budget.\n");
        return;
    }
    if (!readNonNegativeAmount("Enter allocated budget (N$): ", &allocation)) {
        return;
    }

    strcpy(departments[departmentCount], department);
    allocatedBudgets[departmentCount] = allocation;
    expenditures[departmentCount] = 0.0;
    departmentCount++;
    printf("Budget added for %s.\n", department);
}

void enterExpenditure(void)
{
    char department[50];
    double amount;
    int index;

    if (departmentCount == 0) {
        printf("No departments have been added.\n");
        return;
    }
    if (!readRequiredText("Enter department name: ", department,
                          sizeof(department))) {
        return;
    }
    index = findDepartment(department);
    if (index < 0) {
        printf("Department not found.\n");
        return;
    }
    if (!readNonNegativeAmount("Enter expenditure amount (N$): ", &amount)) {
        return;
    }

    expenditures[index] += amount;
    printf("Expenditure recorded for %s.\n", departments[index]);
}

void displayBudgets(void)
{
    int i;

    if (departmentCount == 0) {
        printf("No departments have been added.\n");
        return;
    }

    for (i = 0; i < departmentCount; i++) {
        double remaining = allocatedBudgets[i] - expenditures[i];

        printf("\nDepartment: %s\n", departments[i]);
        printf("Allocated Budget: N$%.2f\n", allocatedBudgets[i]);
        printf("Expenditure: N$%.2f\n", expenditures[i]);
        printf("Remaining Budget: N$%.2f\n", remaining);
        printf("Status: %s\n",
               remaining >= 0.0 ? "WITHIN BUDGET" : "OVER BUDGET");
    }
}

void generateReport(void)
{
    double totalAllocated = 0.0;
    double totalExpenditure = 0.0;
    int i;

    if (departmentCount == 0) {
        printf("No departments have been added.\n");
        return;
    }

    for (i = 0; i < departmentCount; i++) {
        totalAllocated += allocatedBudgets[i];
        totalExpenditure += expenditures[i];
    }

    printf("\n==== BUDGET REPORT ====\n");
    printf("Total Allocated Budget: N$%.2f\n", totalAllocated);
    printf("Total Expenditure: N$%.2f\n", totalExpenditure);
    printf("Total Remaining Budget: N$%.2f\n",
           totalAllocated - totalExpenditure);

    for (i = 0; i < departmentCount; i++) {
        if (expenditures[i] > allocatedBudgets[i]) {
            printf("Department over budget: %s\n", departments[i]);
        }
    }
}
    for (int i = 0; i < departmentCount; i++){
    if (expenditures[i] > allocatedBudgets[i]){
        printf("Department over Budget: %s\n", departments[i]);
    }
}
}
