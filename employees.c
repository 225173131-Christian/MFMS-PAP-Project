#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "employees.h"

/* Parallel arrays: index i in every array belongs to the same employee */
static char   empId[MAX_EMPLOYEES][MAX_ID];
static char   empName[MAX_EMPLOYEES][MAX_NAME];
static char   empDept[MAX_EMPLOYEES][MAX_DEPT];
static double empBasic[MAX_EMPLOYEES];
static double empHousing[MAX_EMPLOYEES];
static double empTransport[MAX_EMPLOYEES];
static double empOther[MAX_EMPLOYEES];
static int    empCount = 0;

/* ---------- input helpers (validation) ---------- */

static int readLine(const char *prompt, char *buf, int size)
{
    size_t length;
    int ch;

    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return 0;
    }
    length = strlen(buf);
    if (length > 0 && buf[length - 1] == '\n') {
        buf[length - 1] = '\0';
        return 1;
    }
    if (!feof(stdin)) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard the rest of the overlong line */
        }
        printf("  Error: input is too long; please try again.\n");
        return -1;
    }
    return 1;
}

static int isBlank(const char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (!isspace((unsigned char)s[i])) return 0;
    }
    return 1;
}

static int readNonEmpty(const char *prompt, char *buf, int size)
{
    int status;
    char *start;
    size_t length;

    for (;;) {
        status = readLine(prompt, buf, size);
        if (status == 0) {
            return 0;
        }
        if (status < 0) {
            continue;
        }
        if (isBlank(buf)) {
            printf("  Error: this field cannot be empty.\n");
            continue;
        }

        start = buf;
        while (isspace((unsigned char)*start)) {
            start++;
        }
        length = strlen(start);
        while (length > 0 && isspace((unsigned char)start[length - 1])) {
            start[--length] = '\0';
        }
        if (start != buf) {
            memmove(buf, start, length + 1);
        }
        return 1;
    }
}

/* Keeps asking until the user enters a number that is >= 0 */
static int readMoney(const char *prompt, double *amount)
{
    char tmp[64];
    char *end;
    double value;
    int status;

    while (1) {
        status = readLine(prompt, tmp, sizeof(tmp));
        if (status == 0) {
            return 0;
        }
        if (status < 0) {
            continue;
        }
        errno = 0;
        value = strtod(tmp, &end);
        if (end == tmp || errno == ERANGE || !isfinite(value)) {
            printf("  Error: please enter a number.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) end++;
        if (*end != '\0') {
            printf("  Error: invalid number.\n");
        } else if (value < 0) {
            printf("  Error: amount cannot be negative.\n");
        } else {
            *amount = value;
            return 1;
        }
    }
}

static int readChoice(const char *prompt)
{
    char tmp[32];
    char *end;
    long v;
    int status;

    status = readLine(prompt, tmp, sizeof(tmp));
    if (status == 0) return -2;
    if (status < 0) return -1;
    errno = 0;
    v = strtol(tmp, &end, 10);
    if (end == tmp || errno == ERANGE || v < INT_MIN || v > INT_MAX) return -1;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return -1;
    return (int)v;
}

/* ---------- search helpers ---------- */

static int findById(const char *id)
{
    int i;
    for (i = 0; i < empCount; i++) {
        if (strcmp(empId[i], id) == 0) return i;
    }
    return -1;
}

static void toLowerCopy(const char *src, char *dest)
{
    int i;
    for (i = 0; src[i] != '\0'; i++) dest[i] = (char)tolower((unsigned char)src[i]);
    dest[i] = '\0';
}

/* ---------- calculations ---------- */

double calculateSalary(double basic, double housing, double transport, double other)
{
    return basic + housing + transport + other;   /* gross salary */
}

/* Simplified, illustrative tax bands - agree on these as a group */
double calculateTax(double gross)
{
    double rate;
    if (gross <= 5000)       rate = 0.00;
    else if (gross <= 15000) rate = 0.18;
    else if (gross <= 30000) rate = 0.25;
    else                     rate = 0.32;
    return gross * rate;
}

static void printSalaryDetails(int i)
{
    double gross = calculateSalary(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
    double tax   = calculateTax(gross);

    printf("\n--- Salary Information ---\n");
    printf("Employee ID      : %s\n", empId[i]);
    printf("Name             : %s\n", empName[i]);
    printf("Department       : %s\n", empDept[i]);
    printf("Basic Salary     : N$%.2f\n", empBasic[i]);
    printf("Housing Allowance: N$%.2f\n", empHousing[i]);
    printf("Transport Allow. : N$%.2f\n", empTransport[i]);
    printf("Other Allowances : N$%.2f\n", empOther[i]);
    printf("Gross Salary     : N$%.2f\n", gross);
    printf("Tax              : N$%.2f\n", tax);
    printf("Net Salary       : N$%.2f\n", gross - tax);
}

/* ---------- main operations ---------- */

void addEmployee(void)
{
    char id[MAX_ID], name[MAX_NAME], dept[MAX_DEPT];
    double basic, housing, transport, other;

    if (empCount >= MAX_EMPLOYEES) {
        printf("Employee list is full (%d).\n", MAX_EMPLOYEES);
        return;
    }

    printf("\n--- Add Employee ---\n");
    do {
        if (!readNonEmpty("Employee ID: ", id, sizeof(id))) return;
        if (findById(id) != -1) printf("  Error: that ID already exists.\n");
    } while (findById(id) != -1);

    if (!readNonEmpty("Full name: ", name, sizeof(name))) return;
    if (!readNonEmpty("Department: ", dept, sizeof(dept))) return;
    if (!readMoney("Basic salary (N$): ", &basic)) return;
    if (!readMoney("Housing allowance (N$): ", &housing)) return;
    if (!readMoney("Transport allowance (N$): ", &transport)) return;
    if (!readMoney("Other allowances (N$): ", &other)) return;

    strcpy(empId[empCount], id);
    strcpy(empName[empCount], name);
    strcpy(empDept[empCount], dept);
    empBasic[empCount]     = basic;
    empHousing[empCount]   = housing;
    empTransport[empCount] = transport;
    empOther[empCount]     = other;
    empCount++;

    printf("Employee added successfully.\n");
}

void displayEmployees(void)
{
    int i;
    if (empCount == 0) {
        printf("\nNo employees registered yet.\n");
        return;
    }
    printf("\n%-10s %-25s %-18s %12s\n", "ID", "Name", "Department", "Gross (N$)");
    printf("---------------------------------------------------------------------\n");
    for (i = 0; i < empCount; i++) {
        printf("%-10s %-25s %-18s %12.2f\n", empId[i], empName[i], empDept[i],
               calculateSalary(empBasic[i], empHousing[i], empTransport[i], empOther[i]));
    }
    printf("Total employees: %d\n", empCount);
}

void searchEmployee(void)
{
    int choice, i, found = 0;
    char input[MAX_NAME], lowerInput[MAX_NAME], lowerName[MAX_NAME];

    if (empCount == 0) {
        printf("\nNo employees to search.\n");
        return;
    }

    printf("\n--- Search Employee ---\n1. By ID\n2. By name (partial match)\n");
    choice = readChoice("Choice: ");
    if (choice == -2) return;

    if (choice == 1) {
        if (!readNonEmpty("Enter ID: ", input, sizeof(input))) return;
        i = findById(input);
        if (i == -1) printf("No employee with ID %s.\n", input);
        else printSalaryDetails(i);
    } else if (choice == 2) {
        if (!readNonEmpty("Enter name or part of name: ", input, sizeof(input))) return;
        toLowerCopy(input, lowerInput);
        for (i = 0; i < empCount; i++) {
            toLowerCopy(empName[i], lowerName);
            if (strstr(lowerName, lowerInput) != NULL) {
                printSalaryDetails(i);
                found++;
            }
        }
        if (!found) printf("No employee found matching \"%s\".\n", input);
    } else {
        printf("Invalid choice.\n");
    }
}

void salaryInformation(void)
{
    char id[MAX_ID];
    int i;

    if (empCount == 0) {
        printf("\nNo employees registered yet.\n");
        return;
    }
    if (!readNonEmpty("Enter employee ID: ", id, sizeof(id))) return;
    i = findById(id);
    if (i == -1) printf("No employee with ID %s.\n", id);
    else printSalaryDetails(i);
}

/* ---------- functions for the Reports module ---------- */

int getEmployeeCount(void) { return empCount; }

double getAverageSalary(void)
{
    int i;
    double total = 0;
    if (empCount == 0) return 0;
    for (i = 0; i < empCount; i++)
        total += calculateSalary(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
    return total / empCount;
}

double getHighestSalary(void)
{
    int i;
    double s, max = 0;
    for (i = 0; i < empCount; i++) {
        s = calculateSalary(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
        if (i == 0 || s > max) max = s;
    }
    return max;
}

double getLowestSalary(void)
{
    int i;
    double s, min = 0;
    for (i = 0; i < empCount; i++) {
        s = calculateSalary(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
        if (i == 0 || s < min) min = s;
    }
    return min;
}

/* ---------- module menu ---------- */

void employeeMenu(void)
{
    int choice;
    do {
        printf("\n========== EMPLOYEE MANAGEMENT ==========\n");
        printf("1. Add employee\n2. Display employees\n3. Search employee\n");
        printf("4. Calculate salary information\n5. Back to main menu\n");
        choice = readChoice("Enter your choice: ");
        if (choice == -2) return;

        switch (choice) {
            case 1: addEmployee();       break;
            case 2: displayEmployees();  break;
            case 3: searchEmployee();    break;
            case 4: salaryInformation(); break;
            case 5: break;
            default: printf("Invalid choice. Please enter 1-5.\n");
        }
    } while (choice != 5);
}

