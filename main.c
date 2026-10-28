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

#include <stdio.h>
#include <string.h>

#include "assets.h"
#include "budget.h"
#include "employees.h"
#include "reports.h"
#include "supplier_management.h"

static int readChoice(const char *prompt)
{
    char line[64];
    char extra;
    int choice;

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) return -2;
    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Clear the rest of the line. */
        }
        return -1;
    }
    if (sscanf(line, "%d %c", &choice, &extra) != 1) return -1;
    return choice;
}

static void budgetMenu(void)
{
    int choice;

    do {
        printf("\n--- Budget Management ---\n");
        printf("1. Add department budget\n2. Enter expenditure\n");
        printf("3. Display budgets\n4. Budget report\n5. Back\n");
        choice = readChoice("Choose: ");

        switch (choice) {
            case 1: addBudget(); break;
            case 2: enterExpenditure(); break;
            case 3: displayBudgets(); break;
            case 4: generateReport(); break;
            case 5: break;
            case -2: return;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

static void reportsMenu(void)
{
    int choice;

    do {
        printf("\n--- Reports ---\n");
        printf("1. Employees\n2. Budget\n3. Suppliers\n4. Assets\n5. Back\n");
        choice = readChoice("Choose: ");

        switch (choice) {
            case 1: employeeReport(); break;
            case 2: budgetReport(); break;
            case 3: supplierReport(); break;
            case 4: assetReport(); break;
            case 5: break;
            case -2: return;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

int main(void)
{
    int choice;

    do {
        printf("\n--- Municipal Financial Management System ---\n");
        printf("1. Employee management\n2. Budget management\n");
        printf("3. Supplier management\n4. Asset management\n");
        printf("5. Reports\n6. Exit\n");
        choice = readChoice("Choose: ");

        switch (choice) {
            case 1: employeeMenu(); break;
            case 2: budgetMenu(); break;
            case 3: supplierMenu(); break;
            case 4: assetMenu(); break;
            case 5: reportsMenu(); break;
            case 6: printf("Goodbye.\n"); break;
            case -2: return 0;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 6);

    return 0;
}

