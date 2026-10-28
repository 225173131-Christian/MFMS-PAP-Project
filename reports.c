#include <stdio.h>

#include "reports.h"
#include "employees.h"
#include "budget.h"
#include "assets.h"
#include "supplier_management.h"

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

void assetReport(void)
{
    printf("\n=================================\n");
    printf("\nREPORT FOR ASSETS \n");
    printf("\n=================================\n");

    displayAssetReport();
}
