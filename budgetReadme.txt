Budget Module -UH Kafidi

Budget model has 3 pieces of information that belong together:
departments[i]
allocatedBudget[i]
expenditures[i]

When entering information, the same index represents the same department across all fields.

departmentCount traks how many departments have been entered. 

Budget module consists of 4 functions:

addBudget - For to add and store the Budget
enterExpenditure - For adding and storing Expenditure
displayBudgets - To display what the balance is and calculate whether department is over or within Budget
generateReport - Displays the full report

budget.h is the header file and should be used to anchor the module to the main function