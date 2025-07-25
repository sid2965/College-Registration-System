// utility.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include <ctype.h>
#include "common.h"



void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int is_selection_enabled() {
    FILE *file = fopen(SELECTION_STATUS_FILE, "r");
    if (!file) {
        perror("Unable to open selection status file");
        return 0;
    }

    char status[20];
    fgets(status, sizeof(status), file);
    fclose(file);

    if (strcasecmp(status, "enabled") == 0) {
        return 1;
    }

    return 0;
}

void encryptPassword(char password[]) {
    int key = 3; // Caesar cipher key

    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'a' && password[i] <= 'z') {
            password[i] = 'a' + (password[i] - 'a' + key) % 26;
        } else if (password[i] >= 'A' && password[i] <= 'Z') {
            password[i] = 'A' + (password[i] - 'A' + key) % 26;
        }
    }
}

// Function to validate password complexity
int validatePassword(char password[]) {
    int length = strlen(password);
    int hasLower = 0, hasUpper = 0, hasDigit = 0, hasSpecial = 0;

    for (int i = 0; i < length; i++) {
        if (islower(password[i])) hasLower = 1;
        else if (isupper(password[i])) hasUpper = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else hasSpecial = 1;
    }

    return (length >= 8 && hasLower && hasUpper && hasDigit && hasSpecial);
}

// Function to check if a username already exists in a CSV file
int usernameExists(const char *filename, char username[]) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_USERNAME_LENGTH];
        sscanf(line, "%[^,]", storedUsername);
        if (strcmp(storedUsername, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Function to authenticate a user from a CSV file
int authenticateUser(const char *filename, char username[], char password[]) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_USERNAME_LENGTH], storedPassword[MAX_PASSWORD_LENGTH];
        sscanf(line, "%[^,],%[^,]", storedUsername, storedPassword);
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
void view_student_details(const char *filename, const char *reg_number) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

    printf("\nDetails from %s:\n", filename);

    while (fgets(line, sizeof(line), file)) {
        char temp_reg[MAX_REG_LENGTH];
        sscanf(line, "%[^,],", temp_reg);

        if (strcmp(temp_reg, reg_number) == 0) {
            printf("%s", line);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with Registration Number %s not found in %s.\n", reg_number, filename);
    }

    fclose(file);
}

void read_courses(const char *filename, Course courses[], int *course_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d",
               courses[*course_count].course_code,
               courses[*course_count].course_name,
               courses[*course_count].faculty,
               &courses[*course_count].seats_available);
        (*course_count)++;
    }
    fclose(file);
}

void read_students(const char *filename, Student students[], int *student_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    char line[300];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
               students[*student_count].name,
               students[*student_count].reg,
               &students[*student_count].meritscore,
               students[*student_count].dept,
               students[*student_count].sem,
               students[*student_count].choices[0],
               students[*student_count].choices[1],
               students[*student_count].choices[2],
               students[*student_count].choices[3],
               students[*student_count].choices[4],
               students[*student_count].choices[5]);
        (*student_count)++;
    }
    fclose(file);
}

void write_allotment_status(const char *filename, Allotment allotments[], int allotment_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    fprintf(file, "name,reg,meritscore,dept,sem,course_allocation,waiting_list_number\n");
    for (int i = 0; i < allotment_count; i++) {
        fprintf(file, "%s,%s,%d,%s,%s,%s,%d\n",
                allotments[i].name,
                allotments[i].reg,
                allotments[i].meritscore,
                allotments[i].dept,
                allotments[i].sem,
                allotments[i].course_allocation,
                allotments[i].waiting_list_number);
    }
    fclose(file);
}

void write_waiting_list(const char *filename, WaitingListEntry waiting_list[], int waiting_list_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    fprintf(file, "course_code,student_name,reg,meritscore\n");
    for (int i = 0; i < waiting_list_count; i++) {
        fprintf(file, "%s,%s,%s,%d\n",
                waiting_list[i].course_code,
                waiting_list[i].student_name,
                waiting_list[i].reg,
                waiting_list[i].meritscore);
    }
    fclose(file);
}

void allot_courses(Student students[], int student_count, Course courses[], int course_count, Allotment allotments[], int *allotment_count, WaitingListEntry waiting_list[], int *waiting_list_count) {
    for (int i = 0; i < student_count; i++) {
        int allocated = 0;
        for (int j = 0; j < MAX_CHOICES && strlen(students[i].choices[j]) > 0; j++) {
            for (int k = 0; k < course_count; k++) {
                if (strcmp(students[i].choices[j], courses[k].course_code) == 0 && courses[k].seats_available > 0) {
                    courses[k].seats_available--;
                    strcpy(allotments[*allotment_count].course_allocation, courses[k].course_code);
                    allocated = 1;
                    break;
                }
            }
            if (allocated) break;
        }

        if (!allocated) {
            for (int j = 0; j < MAX_CHOICES && strlen(students[i].choices[j]) > 0; j++) {
                int waitlist_count = 0;
                for (int k = 0; k < *waiting_list_count; k++) {
                    if (strcmp(waiting_list[k].course_code, students[i].choices[j]) == 0) {
                        waitlist_count++;
                    }
                }
                if (waitlist_count < MAX_WAITLIST) {
                    strcpy(waiting_list[*waiting_list_count].course_code, students[i].choices[j]);
                    strcpy(waiting_list[*waiting_list_count].student_name, students[i].name);
                    strcpy(waiting_list[*waiting_list_count].reg, students[i].reg);
                    waiting_list[*waiting_list_count].meritscore = students[i].meritscore;
                    (*waiting_list_count)++;
                    strcpy(allotments[*allotment_count].course_allocation, "No");
                    allotments[*allotment_count].waiting_list_number = waitlist_count + 1;
                    break;
                }
            }
        }

        strcpy(allotments[*allotment_count].name, students[i].name);
        strcpy(allotments[*allotment_count].reg, students[i].reg);
        allotments[*allotment_count].meritscore = students[i].meritscore;
        strcpy(allotments[*allotment_count].dept, students[i].dept);
        strcpy(allotments[*allotment_count].sem, students[i].sem);
        if (allocated) {
            allotments[*allotment_count].waiting_list_number = 0;
        }
        (*allotment_count)++;
    }
}

void store(struct student s) {
    char filename[50];

    sprintf(filename, "details%d%s.csv", s.sem,
                strcasecmp(s.dept, "cse") == 0 ? "cse" :
                strcasecmp(s.dept, "it") == 0 ? "it" :
                strcasecmp(s.dept, "ece") == 0 ? "ece" :
                strcasecmp(s.dept, "eee") == 0 ? "eee" :
                strcasecmp(s.dept, "bme") == 0 ? "bme" :
                strcasecmp(s.dept, "chem") == 0 ? "chem" :
                strcasecmp(s.dept, "civil") == 0 ? "civil" :
                strcasecmp(s.dept, "mech") == 0 ? "mech" : "unknown");

    char newLine[MAX_LINE_LENGTH];
    snprintf(newLine, sizeof(newLine), "%d, %s, %s, %s, %s, %.2f, %d, %d, %d, %d, %.2f, %d\n",
             s.reg, s.name, s.date, s.address, s.dept, s.gpa,
             s.workshops, s.internships, s.certificateCourses,
             s.competitions, s.meritScore, s.sem);

    writeSortedCSV(filename, newLine);

    if (s.sem > 1) {
        char completedFilename[50];
        sprintf(completedFilename, "completedCourses.csv");
        char completedLine[MAX_LINE_LENGTH];
        snprintf(completedLine, sizeof(completedLine), "%d", s.reg);
        for (int i = 0; i < s.completedCourseCount; i++) {
            snprintf(completedLine + strlen(completedLine), sizeof(completedLine) - strlen(completedLine), ",%s", s.completedCourses[i]);
        }
        snprintf(completedLine + strlen(completedLine), sizeof(completedLine) - strlen(completedLine), "\n");

        writeSortedCSV(completedFilename, completedLine);
    }
}

void displayCourses(Course *courses, int count, const char *dept, int sem) {
    int found = 0;
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(courses[i].dept, dept) == 0 && courses[i].sem == sem) {
            if (i == 0 || strcasecmp(courses[i].courseID, courses[i - 1].courseID) != 0) {
                printf("CourseID: %s\n", courses[i].courseID);
                printf("CourseName: %s\n", courses[i].courseName);
                printf("Department: %s\n", courses[i].dept);
                printf("Semester: %d\n", courses[i].sem);
            }
            printf("Faculty: %s\n", courses[i].faculty);
            printf("Available Seats: %d\n", courses[i].availableSeats);
            printf("Prerequisite: %s\n", courses[i].prerequisite);
            printf("\n");
            found = 1;
        }
    }
    if (!found) {
        printf("No courses found for Department: %s, Semester: %d\n", dept, sem);
    }
}

int hasPrerequisitesCompleted(Course *courses, int count, char *courseID) {
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(courses[i].courseID, courseID) == 0 && strlen(courses[i].prerequisite) > 0) {
            for (int j = 0; j < count; ++j) {
                if (strcasecmp(courses[j].courseID, courses[i].prerequisite) == 0 && courses[j].selected == 1) {
                    return 1;
                }
            }
            return 0;
        }
    }
    return 1;
}

int selectCourses(Course *courses, int count, int numCoursesToSelect) {
    int selected_count = 0;
    char courseID[MAX_FIELD_LENGTH];
    char faculty[MAX_FIELD_LENGTH];

    while (selected_count < numCoursesToSelect) {
        printf("Enter course ID to select (or 'done' to finish): ");
        scanf("%s", courseID);

        if (strcasecmp(courseID, "done") == 0) {
            break;
        }

        printf("Enter faculty name for the selected course: ");
        scanf(" %[^\n]%*c", faculty);

        int found = 0;
        for (int i = 0; i < count; ++i) {
            if (strcasecmp(courses[i].courseID, courseID) == 0 && strcasecmp(courses[i].faculty, faculty) == 0) {
                if (courses[i].selected) {
                    printf("Course already selected. Please choose another.\n");
                    found = 1;
                } else if (!hasPrerequisitesCompleted(courses, count, courseID)) {
                    printf("Prerequisite course not completed.\n");
                    found = 1;
                } else {
                    courses[i].selected = 1;
                    selected_count++;
                    printf("Course %s with faculty %s registered.\n", courses[i].courseID, courses[i].faculty);
                    found = 1;
                }
                break;
            }
        }

        if (found == 0) {
            printf("Course or faculty not found. Please enter a valid course ID and faculty name.\n");
        }
    }

    return selected_count;
}

void rankdis(char Name[50], char course[5], int sem, int reg) {
    int rank = 1, r = 0;
    char filename[50];
    char first[] = "ranklist";
    char sema[] = "sem";
    char semadd[2];
    sprintf(semadd, "%d", sem);

    if (sem == 1) {
        sprintf(filename, "%s.csv", first);
    } else {
        sprintf(filename, "%s%s%s%s.csv", first, sema, semadd, course);
    }

    FILE *fr = fopen(filename, "r");
    if (fr == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    struct Student s[100];
    char head[500]; // Ensure this is large enough to hold the entire line
    fgets(head, sizeof(head), fr); // Read and discard header line

    while (fgets(head, sizeof(head), fr)) {
        sscanf(head, "%[^,],%[^,],%[^,],%[^,],%f,%d,%d,%d,%d,%d,%f,%d",
               s[r].name, s[r].date, s[r].address, s[r].course,
               &s[r].gpa, &s[r].sem, &s[r].workshops, &s[r].internships,
               &s[r].certificateCourses, &s[r].competitions,
               &s[r].meritScore, &s[r].reg);

        if (strcasecmp(s[r].name, Name) == 0 && strcasecmp(s[r].course, course) == 0 && s[r].sem == sem && s[r].reg == reg) {
            printf("RANK : %d\n", rank);
            break;
        }

        rank++;
        r++;
    }

    fclose(fr);

    if (rank != 0) {
        if (rank >= 1 && rank <= 40) {
            printf("Batch: 1\n");
        } else if (rank >= 41 && rank <= 80) {
            printf("Batch: 2\n");
        } else if (rank >= 81 && rank <= 120) {
            printf("Batch: 3\n");
        } else {
            printf("Batch: 4\n");
        }
    } else {
        printf("Data Not Found\n");
    }
}

void infoget() { 
    char Name[50];
    char dept[5];
    int sem;
    int reg;

    printf("Enter Name: ");
    fgets(Name, sizeof(Name), stdin);
    Name[strcspn(Name, "\n")] = '\0'; // Remove newline if present

    printf("Enter Dept: ");
    fgets(course, sizeof(course), stdin);
    course[strcspn(course, "\n")] = '\0'; // Remove newline if present

    printf("Enter Semester: ");
    scanf("%d", &sem);

    printf("Enter Registration Number: ");
    scanf("%d", &reg);

    rankdis(Name, dept, sem, reg);
}


void display_allotments(const char *filename, const char *reg) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char line[200];
    fgets(line, sizeof(line), file);  // Skip header line

    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH];
        char reg_from_file[MAX_REG_LENGTH];
        char course_allocation[MAX_CHOICES][10];  // Store up to 6 course codes
        char faculty[MAX_CHOICES][50];  // Store the faculty for each allotted course
        int waiting_list_number;

        // Parse the line to extract data
        sscanf(line, "%[^,],%[^,],%d,%[^,],%*[^,],%[^,],%[^,],%d",
               name, reg_from_file, course_allocation[0], faculty[0], &waiting_list_number);

        // Check if the registration number matches and the student is not on the waiting list
        if (strcmp(reg_from_file, reg) == 0 && strcmp(course_allocation[0], "No") != 0) {
            printf("Student Name: %s\n", name);
            printf("Registration Number: %s\n", reg_from_file);

            // Print each allotted course and its faculty
            for (int i = 0; i < MAX_CHOICES; ++i) {
                if (strlen(course_allocation[i]) > 0) {
                    printf("Allotted Course %d: %s\n", i + 1, course_allocation[i]);
                    printf("Faculty: %s\n", faculty[i]);
                }
            }
            printf("\n");

            // Since we found the student, we can break out of the loop
            break;
        }
    }

    fclose(file);
}