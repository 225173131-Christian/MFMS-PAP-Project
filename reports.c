#include <stdio.h>

#include "reports.h"
#include "employee.h"
#include "budget.h"
#include "assets.h"
#include "supplier_management.h"

void displaySupplier(void);

void employeeReport(void)
{
    printf("\n=================================\n");
    printf("\nREPORT FOR EMPLOYEES\n");
    printf("\n=================================\n");

    printf("Total Employees : %d\n", getEmployeeCount());
    printf("Average Salary  : N$ %.2lf\n", getAverageSalary());
    printf("Highest Salary  : N$ %.2lf\n", getHighestSalary());
    printf("Lowest Salary   : N$ %.2lf\n", getLowestSalary());
}

void budgetReport(void)
{
    printf("\n=================================\n");
    printf("\n REPORT FOR BUDGET\n");
    printf("\n=================================\n");

    generateReport();
}

void supplierReport(void)
{
    printf("\n=================================\n");
    printf("\n REPORT FOR SUPPLIERS \n");
    printf("\n=================================\n");

    displaySupplier();
}

void assetReport(Asset assets[], int assetCount)
{
    printf("\n=================================\n");
    printf("\nREPORT FOR ASSETS \n");
    printf("\n=================================\n");

    displayAssets(assets, assetCount);
}

void displayReportsMenu(void)
{
    printf("\n========================\n");
    printf("       \nREPORTS MENU\n");
    printf("\n=======================\n");

    printf("1. Employee Report\n");
    printf("2. Budget Report\n");
    printf("3. Supplier Report\n");
    printf("4. Asset Report\n");
    printf("5. Return to Main Menu\n");
}