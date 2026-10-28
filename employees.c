#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "employees.h"

static char employeeIds[MAX_EMPLOYEES][MAX_ID];
static char employeeNames[MAX_EMPLOYEES][MAX_NAME];
static char departments[MAX_EMPLOYEES][MAX_DEPT];
static double basicPay[MAX_EMPLOYEES];
static double housingPay[MAX_EMPLOYEES];
static double transportPay[MAX_EMPLOYEES];
static double otherPay[MAX_EMPLOYEES];
static int employeeCount = 0;

static int readLine(const char *prompt, char *text, int size)
{
    size_t length;
    int ch;

    printf("%s", prompt);
    if (fgets(text, size, stdin) == NULL) {
        return 0;
    }
    length = strlen(text);
    if (length > 0 && text[length - 1] == '\n') {
        text[length - 1] = '\0';
    } else if (!feof(stdin)) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Clear the rest of the line. */
        }
        printf("Input is too long. Try again.\n");
        return -1;
    }
    return 1;
}

static int readText(const char *prompt, char *answer, int size)
{
    char line[128];
    char *start;
    size_t length;
    int result;

    while (1) {
        result = readLine(prompt, line, sizeof(line));
        if (result == 0) {
            return 0;
        }
        if (result < 0) {
            continue;
        }

        start = line;
        while (isspace((unsigned char)*start)) {
            start++;
        }
        length = strlen(start);
        while (length > 0 && isspace((unsigned char)start[length - 1])) {
            start[--length] = '\0';
        }

        if (length == 0) {
            printf("This cannot be left empty.\n");
        } else if (length >= (size_t)size) {
            printf("Please use fewer characters.\n");
        } else {
            strcpy(answer, start);
            return 1;
        }
    }
}

static int readNumber(const char *prompt, double *number)
{
    char line[128];
    char extra;
    double value;
    int result;

    while (1) {
        result = readLine(prompt, line, sizeof(line));
        if (result == 0) {
            return 0;
        }
        if (result < 0) {
            continue;
        }
        if (sscanf(line, "%lf %c", &value, &extra) != 1 ||
            !isfinite(value) || value < 0) {
            printf("Enter a valid amount that is not negative.\n");
        } else {
            *number = value;
            return 1;
        }
    }
}

static int readChoice(const char *prompt)
{
    char line[64];
    char extra;
    int choice;
    int result;

    result = readLine(prompt, line, sizeof(line));
    if (result == 0) {
        return -2;
    }
    if (result < 0 || sscanf(line, "%d %c", &choice, &extra) != 1) {
        return -1;
    }
    return choice;
}

static int findEmployee(const char *id)
{
    int i;

    for (i = 0; i < employeeCount; i++) {
        if (strcmp(employeeIds[i], id) == 0) {
            return i;
        }
    }
    return -1;
}

static int containsIgnoreCase(const char *text, const char *search)
{
    int i, j;

    for (i = 0; text[i] != '\0'; i++) {
        for (j = 0; search[j] != '\0'; j++) {
            if (text[i + j] == '\0' ||
                tolower((unsigned char)text[i + j]) !=
                tolower((unsigned char)search[j])) {
                break;
            }
        }
        if (search[j] == '\0') {
            return 1;
        }
    }
    return search[0] == '\0';
}

double calculateSalary(double basic, double housing, double transport, double other)
{
    return basic + housing + transport + other;
}

double calculateTax(double gross)
{
    if (gross <= 5000) return 0;
    if (gross <= 15000) return gross * 0.18;
    if (gross <= 30000) return gross * 0.25;
    return gross * 0.32;
}

static void showSalary(int i)
{
    double gross = calculateSalary(basicPay[i], housingPay[i],
                                   transportPay[i], otherPay[i]);
    double tax = calculateTax(gross);

    printf("\nEmployee: %s (%s)\n", employeeNames[i], employeeIds[i]);
    printf("Department: %s\n", departments[i]);
    printf("Basic pay: N$%.2f\n", basicPay[i]);
    printf("Housing allowance: N$%.2f\n", housingPay[i]);
    printf("Transport allowance: N$%.2f\n", transportPay[i]);
    printf("Other allowances: N$%.2f\n", otherPay[i]);
    printf("Gross pay: N$%.2f\n", gross);
    printf("Tax: N$%.2f\n", tax);
    printf("Net pay: N$%.2f\n", gross - tax);
}

void addEmployee(void)
{
    char id[MAX_ID];
    char name[MAX_NAME];
    char department[MAX_DEPT];
    int i;

    if (employeeCount == MAX_EMPLOYEES) {
        printf("The employee list is full.\n");
        return;
    }
    if (!readText("Employee ID: ", id, sizeof(id))) return;

    if (findEmployee(id) >= 0) {
        printf("That ID is already in use.\n");
        return;
    }
    if (!readText("Full name: ", name, sizeof(name))) return;
    if (!readText("Department: ", department, sizeof(department))) return;
    if (!readNumber("Basic salary (N$): ", &basicPay[employeeCount])) return;
    if (!readNumber("Housing allowance (N$): ", &housingPay[employeeCount])) return;
    if (!readNumber("Transport allowance (N$): ", &transportPay[employeeCount])) return;
    if (!readNumber("Other allowances (N$): ", &otherPay[employeeCount])) return;

    i = employeeCount;
    strcpy(employeeIds[i], id);
    strcpy(employeeNames[i], name);
    strcpy(departments[i], department);
    employeeCount++;
    printf("Employee added.\n");
}

void displayEmployees(void)
{
    int i;

    if (employeeCount == 0) {
        printf("No employees have been added.\n");
        return;
    }

    printf("\n%-10s %-25s %-18s %12s\n", "ID", "Name", "Department", "Gross pay");
    for (i = 0; i < employeeCount; i++) {
        printf("%-10s %-25s %-18s %12.2f\n",
               employeeIds[i], employeeNames[i], departments[i],
               calculateSalary(basicPay[i], housingPay[i],
                               transportPay[i], otherPay[i]));
    }
}

void searchEmployee(void)
{
    char search[MAX_NAME];
    int choice, i, found = 0;

    if (employeeCount == 0) {
        printf("No employees have been added.\n");
        return;
    }

    printf("\n1. Search by ID\n2. Search by name\n");
    choice = readChoice("Choose: ");
    if (choice == -2) return;

    if (choice == 1) {
        if (!readText("Employee ID: ", search, sizeof(search))) return;
        i = findEmployee(search);
        if (i >= 0) {
            showSalary(i);
        } else {
            printf("Employee not found.\n");
        }
    } else if (choice == 2) {
        if (!readText("Name to search for: ", search, sizeof(search))) return;
        for (i = 0; i < employeeCount; i++) {
            if (containsIgnoreCase(employeeNames[i], search)) {
                showSalary(i);
                found++;
            }
        }
        if (found == 0) printf("No matching employee found.\n");
    } else {
        printf("Invalid choice.\n");
    }
}

void salaryInformation(void)
{
    char id[MAX_ID];
    int i;

    if (!readText("Employee ID: ", id, sizeof(id))) return;
    i = findEmployee(id);
    if (i < 0) {
        printf("Employee not found.\n");
    } else {
        showSalary(i);
    }
}

int getEmployeeCount(void)
{
    return employeeCount;
}

double getAverageSalary(void)
{
    double total = 0;
    int i;

    if (employeeCount == 0) return 0;
    for (i = 0; i < employeeCount; i++) {
        total += calculateSalary(basicPay[i], housingPay[i],
                                 transportPay[i], otherPay[i]);
    }
    return total / employeeCount;
}

double getHighestSalary(void)
{
    double highest = 0;
    double salary;
    int i;

    for (i = 0; i < employeeCount; i++) {
        salary = calculateSalary(basicPay[i], housingPay[i],
                                 transportPay[i], otherPay[i]);
        if (i == 0 || salary > highest) highest = salary;
    }
    return highest;
}

double getLowestSalary(void)
{
    double lowest = 0;
    double salary;
    int i;

    for (i = 0; i < employeeCount; i++) {
        salary = calculateSalary(basicPay[i], housingPay[i],
                                 transportPay[i], otherPay[i]);
        if (i == 0 || salary < lowest) lowest = salary;
    }
    return lowest;
}

void employeeMenu(void)
{
    int choice;

    do {
        printf("\n--- Employee Management ---\n");
        printf("1. Add employee\n2. Display employees\n");
        printf("3. Search employees\n4. Salary information\n5. Back\n");
        choice = readChoice("Choose: ");
        if (choice == -2) return;

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: salaryInformation(); break;
            case 5: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

