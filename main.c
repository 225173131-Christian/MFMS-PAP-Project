/*
 * PAP521S - Project A: Municipal Financial Management System
 * main.c - Module 1: Main Menu + integration of every other module.
 *
 * Contains NO business logic of its own (per Section 9 of the brief) -
 * it only shows menus and calls into each module's functions.
 *
 * Note on design: Employee and Supplier management each expose their own
 * self-contained menu function (employeeMenu(), supplierMenu()), so those
 * are a single call each. Budget and Asset management do not expose a
 * menu function, and Asset management additionally needs its data array
 * owned by the caller - so those two submenus, and the asset array itself,
 * live here in main.c instead.
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "budget.h"
#include "employees.h"
#include "reports.h"
#include "supplier_management.h"

/*
 * Read a menu number as a complete line. Returns 1 on success, 0 for invalid
 * input, and -1 on end-of-file. This avoids scanf leaving bad input behind.
 */
static int readMenuChoice(const char *prompt, int *choice)
{
    char line[128];
    char *end;
    long value;
    int ch;

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }

    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard the rest of the overlong line */
        }
        printf("Invalid input. Please enter a whole number.\n");
        return 0;
    }

    errno = 0;
    value = strtol(line, &end, 10);
    if (end == line || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        printf("Invalid input. Please enter a whole number.\n");
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        end++;
    }
    if (*end != '\0') {
        printf("Invalid input. Please enter a whole number.\n");
        return 0;
    }

    *choice = (int)value;
    return 1;
}

static void budgetManagementMenu(void)
{
    int choice;
    int status;

    for (;;) {
        printf("\n--- BUDGET MANAGEMENT ---\n");
        printf("1. Enter Departmental Budget\n");
        printf("2. Enter Expenditure\n");
        printf("3. Display Budgets\n");
        printf("4. Generate Budget Report\n");
        printf("5. Back to Main Menu\n");

        status = readMenuChoice("Enter your choice: ", &choice);
        if (status < 0) {
            return;
        }
        if (status == 0) {
            continue;
        }

        switch (choice) {
            case 1: addBudget(); break;
            case 2: enterExpenditure(); break;
            case 3: displayBudgets(); break;
            case 4: generateReport(); break;
            case 5: return;
            default: printf("Invalid choice. Please select 1-5.\n");
        }
    }
}

static void reportsMenu(void)
{
    int choice;
    int status;

    for (;;) {
        printf("\n--- REPORTS ---\n");
        printf("1. Employee Report\n");
        printf("2. Budget Report\n");
        printf("3. Supplier Report\n");
        printf("4. Asset Report\n");
        printf("5. Back to Main Menu\n");

        status = readMenuChoice("Enter your choice: ", &choice);
        if (status < 0) {
            return;
        }
        if (status == 0) {
            continue;
        }

        switch (choice) {
            case 1: employeeReport(); break;
            case 2: budgetReport(); break;
            case 3: supplierReport(); break;
            case 4: assetReport(); break;
            case 5: return;
            default: printf("Invalid choice. Please select 1-5.\n");
        }
    }
}

int main(void)
{
    int choice;
    int status;

    for (;;) {
        printf("\n========================================\n");
        printf("MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("1. Employee Management\n");
        printf("2. Budget Management\n");
        printf("3. Supplier Management\n");
        printf("4. Asset Management\n");
        printf("5. Reports\n");
        printf("6. Exit\n");

        status = readMenuChoice("Enter your choice: ", &choice);
        if (status < 0) {
            printf("\nInput ended. Exiting system.\n");
            break;
        }
        if (status == 0) {
            continue;
        }

        switch (choice) {
            case 1: employeeMenu(); break;
            case 2: budgetManagementMenu(); break;
            case 3: supplierMenu(); break;
            case 4: assetMenu(); break;
            case 5: reportsMenu(); break;
            case 6:
                printf("Exiting system. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please select 1-6.\n");
        }
    }

    return 0;
}
