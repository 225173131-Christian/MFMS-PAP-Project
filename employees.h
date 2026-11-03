#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#define MAX_EMPLOYEES 100
#define MAX_ID 15
#define MAX_NAME 50
#define MAX_DEPT 30

void employeeMenu(void);
void addEmployee(void);
void displayEmployees(void);
void searchEmployee(void);
void salaryInformation(void);

double calculateSalary(double basic, double housing, double transport, double other);
double calculateTax(double gross);
int getEmployeeCount(void);
double getAverageSalary(void);
double getHighestSalary(void);
double getLowestSalary(void);

#endif
