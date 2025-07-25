#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "admin.h"
#include "student.h"
#include "utility.h"
#include "common.h"
void mainMenu();
void signupMenu();
void loginMenu();
void resetPassword();

int main() {
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    while (1) {
        printf("\nMain Menu\n");
        printf("1. Signup\n");
        printf("2. Login\n");
        printf("3. Reset Password\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume newline character

        switch (choice) {
            case 1:
                signupMenu();
                break;
            case 2:
                loginMenu();
                break;
            case 3:
                resetPassword();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void signupMenu() {
    int choice;
    while (1) {
        printf("\nSignup Menu\n");
        printf("1. Admin Signup\n");
        printf("2. Student Signup\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume newline character

        switch (choice) {
            case 1:
                adminSignup(ADMIN_FILE);
                break;
            case 2:
                studentSignup(STUDENT_FILE);
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void loginMenu() {
    int choice;
    while (1) {
        printf("\nLogin Menu\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume newline character

        switch (choice) {
            case 1:
                adminLogin();
                adminMenu();
                break;
            case 2:
                studentLogin();
                studentMenu();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void resetPassword() {
    int choice;
    while (1) {
        printf("\nReset Password\n");
        printf("1. Admin Reset Password\n");
        printf("2. Student Reset Password\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume newline character

        switch (choice) {
            case 1:
                adminResetPassword(ADMIN_FILE);
                break;
            case 2:
                studentResetPassword(STUDENT_FILE);
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
