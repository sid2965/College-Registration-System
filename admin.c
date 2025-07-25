// admin.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "admin.h"
#include "utility.h"
#include "course.h"
#include "common.h"



void adminSignup(const char *filename) {

char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH], email[MAX_EMAIL_LENGTH];

    printf("+-----------------------------------------+\n");
    printf("|               Sign Up                    |\n");
    printf("+-----------------------------------------+\n");

    printf("Enter username: ");
    scanf("%s", username);

    // Check if username already exists
    if (usernameExists(ADMIN_FILE, username)) {
        printf("Username already exists. Please choose a different username.\n");
        return;
    }

    while (1) {
        printf("Enter password: ");
        scanf("%s", password);

        // Validate password complexity
        if (!validatePassword(password)) {
            printf("Password must be at least 8 characters long and contain at least one uppercase letter, one lowercase letter, one digit, and one special symbol.\n");
        } else {
            break; // Break the loop if password is valid
        }

   
}
}

void adminLogin() {
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH];
                printf("Enter admin username: ");
                scanf("%s", username);

                if (!usernameExists(ADMIN_FILE, username)) {
                    printf("Username does not exist. Please sign up or enter a valid username.\n");
                    return;
                }

                printf("Enter password: ");
                scanf("%s", password);
                encryptPassword(password); // Encrypt the password for comparison

                if (authenticateUser(ADMIN_FILE, username, password)) {
                    printf("Login successful!\n");
                } else {
                    printf("Login failed. Invalid password.\n");
                }
    
}

void adminMenu() {
    int choice;
    while (1) {
        printf("\nAdmin Menu\n");
        printf("1. Add Course\n");
        printf("2. Remove Course\n");
        printf("3. View Student Details\n");
        printf("4. Start Course Selection\n");
        printf("5. End Course Selection\n");
        printf("6. Run Allotter\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume newline character

        switch (choice) {
            case 1:
                addCourse();
                break;
            case 2:
                removeCourse();
                break;
            case 3:
                viewStudentDetails();
                break;
            case 4:
                //ranker();
                printf("implement not yet");
                break;
            case 5:
                startCourseSelection();
                break;
            case 6:
                endCourseSelection();
                break;
            case 7:
                runAllotter();
                break;
            case 8:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void adminResetPassword(const char *filename) {
    char username[MAX_USERNAME_LENGTH], email[MAX_EMAIL_LENGTH], newPassword[MAX_PASSWORD_LENGTH];
    int userFound = 0;

    printf("+-----------------------------------------+\n");
    printf("|              Reset Password              |\n");
    printf("+-----------------------------------------+\n");

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter registered email: ");
    scanf("%s", email);

    FILE *file = fopen(ADMIN_FILE, "r");
    FILE *tempFile = fopen("temp.csv", "w");

    if (!file || !tempFile) {
        printf("Error opening file!\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_USERNAME_LENGTH], storedPassword[MAX_PASSWORD_LENGTH], storedMobile[MAX_EMAIL_LENGTH];
        sscanf(line, "%[^,],%[^,],%s", storedUsername, storedPassword, storedMobile);
        if (strcmp(storedUsername, username) == 0 && strcmp(storedMobile, email) == 0) {
            userFound = 1;
            while (1) {
                printf("Enter new password: ");
                scanf("%s", newPassword);

                // Validate password complexity
                if (!validatePassword(newPassword)) {
                    printf("Password must be at least 8 characters long and contain at least one uppercase letter, one lowercase letter, one digit, and one special symbol.\n");
                } else {
                    break; // Break the loop if password is valid
                }
            }
            encryptPassword(newPassword); // Encrypt the new password
            fprintf(tempFile, "%s,%s,%s\n", storedUsername, newPassword, storedMobile);
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(ADMIN_FILE);
    rename("temp.csv", ADMIN_FILE);

    if (userFound) {
        printf("Password reset successful!\n");
    } else {
        printf("User not found or email does not match.\n");
    }

    
}