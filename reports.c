#include <stdio.h>

#include "assets.h"
#include "budget.h"
#include "employees.h"
#include "reports.h"
#include "supplier_management.h"

void employeeReport(void)
{
    printf("\n--- Employee Report ---\n");
    printf("Employees: %d\n", getEmployeeCount());
    printf("Average salary: N$%.2f\n", getAverageSalary());
    printf("Highest salary: N$%.2f\n", getHighestSalary());
    printf("Lowest salary: N$%.2f\n", getLowestSalary());
}

void budgetReport(void)
{
    generateReport();
}

void supplierReport(void)
{
    displaySupplier();
}

void assetReport(void)
{
    displayAssetReport();
}
