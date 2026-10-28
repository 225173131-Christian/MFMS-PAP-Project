#ifndef SUPPLIER_MANAGEMENT_H
#define SUPPLIER_MANAGEMENT_H

#define MAX_SUPPLIERS 100
#define SUPPLIER_ID_LEN 20
#define SUPPLIER_NAME_LEN 100
#define SUPPLIER_EMAIL_LEN 100
#define SUPPLIER_PHONE_LEN 30
#define SUPPLIER_TOWN_LEN 50

void addSupplier(void);
void supplierMenu(void);
void displaySupplier(void);
void searchSupplier(void);
int getSupplierCount(void);

#endif
