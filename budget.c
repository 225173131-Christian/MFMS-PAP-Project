#include <stdio.h>
#include <string.h>
#include "budget.h"

char departments[MAX_DEPARTMENTS][50];
double allocatedBudgets[MAX_DEPARTMENTS];
double expenditures[MAX_DEPARTMENTS];
int departmentCount = 0;


void addBudget(void)
{
    if (departmentCount >= MAX_DEPARTMENTS)
    {
        printf("Maximum number of departments reached.\n");
        return;
    }
    printf("Enter Department Name: ");
    scanf(" %49[^\n]", departments[departmentCount]);
    printf("Enter allocated budget: N$");
    scanf("%lf", &allocatedBudgets[departmentCount]);
    while (allocatedBudgets[departmentCount] < 0)
    {
        printf("Budget cannot be negatice. Enter allocated budget: N$");
        scanf("%lf", &allocatedBudgets[departmentCount]);
    }
    expenditures[departmentCount] = 0.0;
    departmentCount++;
    printf("Department budget added successfully: %s\n",
            departments[departmentCount -1]);
}

void enterExpenditure(void)
{
    if(departmentCount == 0)
    {
        printf("No departments have been added.\n");
        return;
    }
    char searchDepartment[50];
    printf("Enter Department Name: ");
    scanf(" %49[^\n]", searchDepartment);
    int foundIndex = -1;
    for (int i = 0; i< departmentCount; i++)
    {
        if(strcmp(departments[i], searchDepartment) == 0)
        {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == -1)
    {
        printf("Department not found.\n");
        return;
    }


    double amount;
    printf("Enter expenditure amount: N$");
    scanf("%lf", &amount);

    while (amount < 0)
    {
        printf("Expenditure cannot be negative. Enter expenditure amount: N$");
        scanf("%lf", &amount);
    }
    expenditures[foundIndex] += amount;
    printf("Expenditure recorded successfully.\n");
}

void displayBudgets(void)
{
    if (departmentCount == 0)
    {
        printf("No departments have been add.\n");
        return;
    }

    for (int i = 0; i < departmentCount; i++)
    {
        double remaining = allocatedBudgets[i] - expenditures[i];

        printf("\nDepartment: %s\n", departments[i]);
        printf("Allocated Budget: N$%.2f\n", allocatedBudgets[i]);
        printf("Expenditure: N$%.2f\n", expenditures[i]);
        printf("Remaining Budget: N$%.2f\n", remaining);
        if (remaining >= 0){
            printf("Status: WITHIN BUDGET\n");
        }
        else {
            printf("Status: OVER BUDGET\n");
        }
    }
}

void generateReport(void)
{
    if (departmentCount == 0)
    {
        printf("No departments have been added.\n");
        return;
    }
    double totalAllocated = 0.0;
    double totalExpenditure = 0.0;

    for (int i =0; i < departmentCount; i++)
    {
        totalAllocated += allocatedBudgets[i];
        totalExpenditure += expenditures[i];
    }

    double totalRemaining = totalAllocated -totalExpenditure;

    printf("\n==== BUDGET REPORT ====\n");
    printf("Total Allocated Budget: N$%.2f\n", totalAllocated);
    printf("Total Expenditure: N$%.2f\n", totalExpenditure);
    printf("Total Remaining Budget: N$%.2f\n", totalRemaining);

    for (int i = 0; i < departmentCount; i++){
    if (expenditures[i] > allocatedBudgets[i]){
        printf("Department over Budget: %s\n", departments[i]);
    }
}
}