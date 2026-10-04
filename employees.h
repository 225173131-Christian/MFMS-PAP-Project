#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#define MAX_EMPLOYEES 100
#define MAX_ID        15
#define MAX_NAME      50
#define MAX_DEPT      30

/* Menu for the whole Employee Management module (call this from main.c) */
void employeeMenu(void);

/* Core operations */
void   addEmployee(void);
void   displayEmployees(void);
void   searchEmployee(void);
void   salaryInformation(void);

/* Calculations (pass-by-value, return a value) */
double calculateSalary(double basic, double housing, double transport, double other);
double calculateTax(double gross);

/* Helpers the Reports module can use */
int    getEmployeeCount(void);
double getAverageSalary(void);
double getHighestSalary(void);
double getLowestSalary(void);

#endif