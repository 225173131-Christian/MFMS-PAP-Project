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
#include "employees.h"
#include "budget.h"
#include "supplier_management.h"
#include "assets.h"
#include "reports.h"

/* ---------- Helper: discard the rest of the current input line ---------- */
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

/* ---------- Asset Management submenu (owns the asset array/count) ---------- */
static void assetManagementMenu(Asset assets[], int *assetCount) {
    int choice;

    do {
        printf("\n--- ASSET MANAGEMENT ---\n");
        printf("1. Add Asset\n");
        printf("2. Display Assets\n");
        printf("3. Search Asset\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addAsset(assets, assetCount); break;
            case 2: displayAssets(assets, *assetCount); break;
            case 3: searchAsset(assets, *assetCount); break;
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please select 1-4.\n");
        }
    } while (choice != 4);
}

/* ---------- Budget Management submenu (budget.h has no menu function) ---------- */
static void budgetManagementMenu(void) {
    int choice;

    do {
        printf("\n--- BUDGET MANAGEMENT ---\n");
        printf("1. Enter Departmental Budget\n");
        printf("2. Enter Expenditure\n");
        printf("3. Display Budgets\n");
        printf("4. Generate Budget Report\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addBudget(); break;
            case 2: enterExpenditure(); break;
            case 3: displayBudgets(); break;
            case 4: generateReport(); break;
            case 5: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please select 1-5.\n");
        }
    } while (choice != 5);
}

/*
 * Reports submenu. Written here (not via displayReportsMenu()) because
 * assetReport() needs the asset array, and displayReportsMenu() takes no
 * parameters - it has no way to reach that data. Calling the four report
 * functions directly here sidesteps that gap.
 */
static void reportsMenu(Asset assets[], int assetCount) {
    int choice;

    do {
        printf("\n--- REPORTS ---\n");
        printf("1. Employee Report\n");
        printf("2. Budget Report\n");
        printf("3. Supplier Report\n");
        printf("4. Asset Report\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: employeeReport(); break;
            case 2: budgetReport(); break;
            case 3: supplierReport(); break;
            case 4: assetReport(assets, assetCount); break;
            case 5: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please select 1-5.\n");
        }
    } while (choice != 5);
}

int main(void) {
    Asset assets[MAX_ASSETS];
    int assetCount = 0;
    int choice;

    do {
        printf("\n========================================\n");
        printf("MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("1. Employee Management\n");
        printf("2. Budget Management\n");
        printf("3. Supplier Management\n");
        printf("4. Asset Management\n");
        printf("5. Reports\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: employeeMenu(); break;
            case 2: budgetManagementMenu(); break;
            case 3: supplierMenu(); break;
            case 4: assetManagementMenu(assets, &assetCount); break;
            case 5: reportsMenu(assets, assetCount); break;
            case 6: printf("Exiting system. Goodbye!\n"); break;
            default: printf("Invalid choice. Please select 1-6.\n");
        }
    } while (choice != 6);

    return 0;
}
