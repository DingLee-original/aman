#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store employee information
struct Employee {
    int id;
    char name[50];
    int age;
    float salary;
};

// Function prototypes
void addEmployee();
void displayEmployees();
void searchEmployeeById();
void updateEmployee();
void deleteEmployee();

// File pointer for employee data storage
FILE *employeeFile;

int main() {
    int choice;

    employeeFile = fopen("employees.dat", "ab+"); // Open file in append binary mode
    if (employeeFile == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (1) {
        printf("\n===== Employee Management System =====\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee by ID\n");
        printf("4. Update Employee Information\n");
        printf("5. Delete Employee\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                displayEmployees();
                break;
            case 3:
                searchEmployeeById();
                break;
            case 4:
                updateEmployee();
                break;
            case 5:
                deleteEmployee();
                break;
            case 6:
                fclose(employeeFile);
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    }

    return 0;
}

void addEmployee() {
    struct Employee newEmployee;
    printf("Enter employee ID: ");
    scanf("%d", &newEmployee.id);
    printf("Enter employee name: ");
    scanf(" %[^\n]s", newEmployee.name);
    printf("Enter employee age: ");
    scanf("%d", &newEmployee.age);
    printf("Enter employee salary: ");
    scanf("%f", &newEmployee.salary);

    // Write new employee data to file
    fwrite(&newEmployee, sizeof(struct Employee), 1, employeeFile);
    printf("Employee added successfully.\n");
}

void displayEmployees() {
    struct Employee emp;
    rewind(employeeFile); // Move file pointer to the beginning
    printf("\n===== Employee List =====\n");
    printf("ID\tName\t\tAge\tSalary\n");
    while (fread(&emp, sizeof(struct Employee), 1, employeeFile)) {
        printf("%d\t%s\t\t%d\t%.2f\n", emp.id, emp.name, emp.age, emp.salary);
    }
}

void searchEmployeeById() {
    int searchId;
    printf("Enter employee ID to search: ");
    scanf("%d", &searchId);

    struct Employee emp;
    rewind(employeeFile); // Move file pointer to the beginning
    while (fread(&emp, sizeof(struct Employee), 1, employeeFile)) {
        if (emp.id == searchId) {
            printf("Employee found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nSalary: %.2f\n", emp.id, emp.name, emp.age, emp.salary);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", searchId);
}

void updateEmployee() {
    int updateId;
    printf("Enter employee ID to update: ");
    scanf("%d", &updateId);

    struct Employee emp;
    long int recsize = sizeof(struct Employee);
    rewind(employeeFile); // Move file pointer to the beginning
    while (fread(&emp, sizeof(struct Employee), 1, employeeFile)) {
        if (emp.id == updateId) {
            printf("Enter new name: ");
            scanf(" %[^\n]s", emp.name);
            printf("Enter new age: ");
            scanf("%d", &emp.age);
            printf("Enter new salary: ");
            scanf("%f", &emp.salary);

            fseek(employeeFile, -recsize, SEEK_CUR); // Move file pointer back one record
            fwrite(&emp, sizeof(struct Employee), 1, employeeFile);
            printf("Employee information updated successfully.\n");
            return;
        }
    }
    printf("Employee with ID %d not found.\n", updateId);
}

void deleteEmployee() {
    int deleteId;
    printf("Enter employee ID to delete: ");
    scanf("%d", &deleteId);

    struct Employee emp, temp;
    long int recsize = sizeof(struct Employee);
    FILE *tempFile = fopen("temp.dat", "wb"); // Temporary file to store non-deleted records
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        return;
    }

    rewind(employeeFile); // Move file pointer to the beginning
    while (fread(&emp, sizeof(struct Employee), 1, employeeFile)) {
        if (emp.id == deleteId) {
            printf("Employee deleted:\n");
            printf("ID: %d\nName: %s\nAge: %d\nSalary: %.2f\n", emp.id, emp.name, emp.age, emp.salary);
        } else {
            fwrite(&emp, sizeof(struct Employee), 1, tempFile); // Write non-deleted records to temporary file
        }
    }

    fclose(employeeFile);
    fclose(tempFile);
    remove("employees.dat"); // Remove original file
    rename("temp.dat", "em
