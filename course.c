// course.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course.h"
#include "utility.h"
#include "common.h"

void addCourse() {
    char course_code[MAX_COURSE_CODE_LENGTH], course_name[MAX_COURSE_NAME_LENGTH];
    char dept[MAX_DEPT_LENGTH], sem[MAX_SEM_LENGTH], prerequisite[MAX_FIELD_LENGTH];
    char faculty[MAX_FACULTY_NAME_LENGTH];
    int seats;
    int faculty_count = 0;

    printf("Enter course code: ");
    fgets(course_code, MAX_COURSE_CODE_LENGTH, stdin);
    course_code[strcspn(course_code, "\n")] = '\0';

    printf("Enter course name: ");
    fgets(course_name, MAX_COURSE_NAME_LENGTH, stdin);
    course_name[strcspn(course_name, "\n")] = '\0';

    printf("Enter number of seats: ");
    scanf("%d", &seats);
    getchar(); // To consume newline character

    printf("Enter department: ");
    fgets(dept, MAX_DEPT_LENGTH, stdin);
    dept[strcspn(dept, "\n")] = '\0';

    printf("Enter semester: ");
    fgets(sem, MAX_SEM_LENGTH, stdin);
    sem[strcspn(sem, "\n")] = '\0';

    printf("Enter prerequisite: ");
    fgets(prerequisite, MAX_FIELD_LENGTH, stdin);
    prerequisite[strcspn(prerequisite, "\n")] = '\0';

    FILE *file = fopen(COURSE_FILE, "a");
    if (!file) {
        printf("Error opening file %s\n", COURSE_FILE);
        return;
    }

    while (faculty_count < 3) {
        printf("Enter faculty (or 'done' to finish): ");
        fgets(faculty, MAX_FACULTY_NAME_LENGTH, stdin);
        faculty[strcspn(faculty, "\n")] = '\0';

        if (strcasecmp(faculty, "done") == 0) {
            if (faculty_count == 0) {
                printf("At least one faculty member must be added.\n");
                continue;
            }
            break;
        }

        fprintf(file, "%s,%s,%s,%s,%d,%s,%s\n", course_code, course_name, dept, sem, seats, faculty, prerequisite);
        faculty_count++;
    }

    fclose(file);
    printf("Course %s added successfully with %d faculty member(s).\n", course_name, faculty_count);
    
}

void removeCourse() {
    char course_code[MAX_COURSE_CODE_LENGTH];
    printf("Enter course code to remove: ");
    fgets(course_code, MAX_COURSE_CODE_LENGTH, stdin);
    trim_newline(course_code);

    FILE *file = fopen(COURSE_FILE, "r");
    if (!file) {
        printf("Error opening file %s\n", COURSE_FILE);
        return;
    }
    FILE *temp_file = fopen("temp.csv", "w");
    if (!temp_file) {
        printf("Error opening temporary file\n");
        fclose(file);
        return;
    }
    char line[MAX_LINE_LENGTH];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, course_code, strlen(course_code)) != 0) {
            fputs(line, temp_file);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp_file);
    remove(COURSE_FILE);
    rename("temp.csv", COURSE_FILE);
    writeSortedCSV(COURSE_FILE);
    if (found) {
        printf("Course %s removed successfully.\n", course_code);
    } else {
        printf("Course %s not found.\n", course_code);
    }
    
}

void viewStudentDetails() {
    int sem;
    char dept[MAX_DEPT_LENGTH], reg_number[MAX_REG_LENGTH];

    printf("Enter Department: ");
    fgets(dept, sizeof(dept), stdin);
    trim_newline(dept);
    
    printf("Enter Semester: ");
    scanf("%d", &sem);
    getchr(); // To consume newline character

    printf("Enter Registration Number (leave blank to view all students): ");
    fgets(reg_number, sizeof(reg_number), stdin);
    trim_newline(reg_number);

    char filename[MAX_LINE_LENGTH];
    snprintf(filename, sizeof(filename), "details%d%s.csv", sem, dept);

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No students found for Department: %s, Semester: %d\n", dept, sem);
        ask_to_continue("viewStudentDetails");
        return;
    }

    if (strlen(reg_number) == 0) {
        char line[MAX_LINE_LENGTH];
        printf("Details from %s:\n", filename);
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
    } else {
        view_student_details(filename, reg_number);
    }

    fclose(file);

    // Display details from completedCourses.csv
    FILE *completed_courses_file = fopen("completedCourses.csv", "r");
    if (!completed_courses_file) {
        printf("No completed courses data found.\n");
        ask_to_continue("viewStudentDetails");
        return;
    }

    if (strlen(reg_number) == 0) {
        char line[MAX_LINE_LENGTH];
        printf("\nDetails from completedCourses.csv:\n");
        while (fgets(line, sizeof(line), completed_courses_file)) {
            printf("%s", line);
        }
    } else {
        view_student_details("completedCourses.csv", reg_number);
    }

    fclose(completed_courses_file);
    ask_to_continue("viewStudentDetails");
}

void startCourseSelection() {
   FILE *file = fopen(SELECTION_STATUS_FILE, "w");
    if (!file) {
        printf("Error opening file %s\n", SELECTION_STATUS_FILE);
        return;
    }

    fprintf(file, "started\n");
    fclose(file);

    printf("Course selection started successfully.\n");
    
}

void endCourseSelection() {
    FILE *file = fopen(SELECTION_STATUS_FILE, "w");
    if (!file) {
        printf("Error opening file %s\n", SELECTION_STATUS_FILE);
        return;
    }

    fprintf(file, "ended\n");
    fclose(file);

    printf("Course selection ended successfully.\n");
    
}

void runAllotter() {
    printf("Running allotter...\n");
    Course courses[MAX_STUDENTS];
    Student students[MAX_STUDENTS];
    Allotment allotments[MAX_STUDENTS];
    WaitingListEntry waiting_list[MAX_WAITLIST];

    int course_count = 0;
    int student_count = 0;
    int allotment_count = 0;
    int waiting_list_count = 0;

    read_courses(COURSE_FILE, courses, &course_count);
    read_students(STUDENTS_RANKED, students, &student_count);
    allot_courses(students, student_count, courses, course_count, allotments, &allotment_count, waiting_list, &waiting_list_count);
    write_allotment_status("allotment_status.csv", allotments, allotment_count);
    write_waiting_list("waiting_list.csv", waiting_list, waiting_list_count);

    printf("Allotment process completed. Check the 'allotment_status.csv' and 'waiting_list.csv' files.\n");
    printf("Allotter run successfully.\n");

}

void storeDetails() {
     struct student s;
    float gpa;

    printf("Enter Registration Number: ");
    scanf("%d", &s.reg);

    printf("Enter Name: ");
    getchar(); // Clearing the newline character left in the buffer
    fgets(s.name, sizeof(s.name), stdin);
    removeNewline(s.name);

    printf("Enter Date of Birth (YYYY-MM-DD): ");
    fgets(s.date, sizeof(s.date), stdin);
    removeNewline(s.date);

    printf("Enter Address: ");
    fgets(s.address, sizeof(s.address), stdin);
    removeNewline(s.address);

    printf("Enter Department: ");
    fgets(s.dept, sizeof(s.dept), stdin);
    removeNewline(s.dept);

    printf("Enter Semester: ");
    scanf("%d", &s.sem);

    if (s.sem > 1) {
        printf("Enter the number of completed courses: ");
        scanf("%d", &s.completedCourseCount);
        getchar(); // Clear the newline character left in the buffer

        for (int i = 0; i < s.completedCourseCount; i++) {
            printf("Enter completed course ID %d: ", i + 1);
            fgets(s.completedCourses[i], sizeof(s.completedCourses[i]), stdin);
            removeNewline(s.completedCourses[i]);
        }
    } else {
        s.completedCourseCount = 0; // No completed courses for semester 1 students
    }

    printf("Enter GPA: ");
    scanf("%f", &gpa);
    s.gpa = gpa;

    printf("Enter number of workshops: ");
    scanf("%d", &s.workshops);

    printf("Enter number of internships: ");
    scanf("%d", &s.internships);

    printf("Enter number of certificate courses: ");
    scanf("%d", &s.certificateCourses);

    printf("Enter number of competitions: ");
    scanf("%d", &s.competitions);

    s.meritScore = (s.gpa * 0.5) + (s.workshops * 0.1) + (s.internships * 0.2) + (s.certificateCourses * 0.1) + (s.competitions * 0.1);

    store(s);

    printf("Student details have been stored successfully.\n");
    
}

void courseRegistrationMenu() {
    if (!is_selection_enabled()) {
                    printf("Course selection is not enabled. Please start the course selection first.\n");
                    break;
                }
                
                char regNumber[MAX_FIELD_LENGTH];
                printf("Enter Registration Number: ");
                scanf("%s", regNumber);

                char dept[MAX_FIELD_LENGTH];
                int sem;
                printf("Enter Department: ");
                scanf("%s", dept);
                printf("Enter Semester: ");
                scanf("%d", &sem);

                FILE *file = fopen("courses.csv", "r");
                if (!file) {
                    perror("Unable to open file");
                    return EXIT_FAILURE;
                }

                char line[MAX_LINE_LENGTH];
                int count = 0;

                // Count the number of lines in the file
                while (fgets(line, sizeof(line), file)) {
                    count++;
                }

                // Allocate memory for the courses
                Course *courses = malloc((count - 1) * sizeof(Course)); // -1 for the header line
                if (!courses) {
                    perror("Unable to allocate memory");
                    fclose(file);
                    return EXIT_FAILURE;
                }

                // Reset the file pointer to the beginning of the file
                rewind(file);

                // Read the header line
                fgets(line, sizeof(line), file);

                // Read the rest of the lines
                int index = 0;
                while (fgets(line, sizeof(line), file)) {
                    sscanf(line, "%[^,],%[^,],%[^,],%d,%d,%[^,],%[^\n]",
                           courses[index].courseID,
                           courses[index].courseName,
                           courses[index].dept,
                           &courses[index].sem,
                           &courses[index].availableSeats,
                           courses[index].faculty,
                           courses[index].prerequisite);
                    courses[index].selected = 0; // Initialize selection status
                    index++;
                }

                fclose(file);

                displayCourses(courses, count - 1, dept, sem); // -1 to exclude the header line

                int numCoursesToSelect;
                printf("How many courses do you want to select? (Minimum 4, Maximum 6): ");
                scanf("%d", &numCoursesToSelect);

                if (numCoursesToSelect < MIN_SELECTED_COURSES || numCoursesToSelect > MAX_SELECTED_COURSES) {
                    printf("Invalid number of courses. Please select between %d and %d courses.\n",
                           MIN_SELECTED_COURSES, MAX_SELECTED_COURSES);
                    free(courses);
                    break;
                }

                int selected_count = selectCourses(courses, count - 1, numCoursesToSelect); // -1 to exclude the header line

                if (selected_count < MIN_SELECTED_COURSES) {
                    printf("Minimum number of courses not selected.\n");
                } else {
                    printf("Courses selected successfully.\n");

                    // Create and open a CSV file to store selected courses
                    FILE *selectedFile = fopen("selected_courses.csv", "w");
                    if (!selectedFile) {
                        perror("Unable to create file");
                        free(courses);
                        break;
                    }

                    // Write the registration number to the file
                    fprintf(selectedFile, "Registration Number: %s\n", regNumber);

                    // Write the selected courses to the file
                    fprintf(selectedFile, "Selected Courses:\n");
                    for (int i = 0; i < count - 1; ++i) { // -1 to exclude the header line
                        if (courses[i].selected) {
                            fprintf(selectedFile, "%s, %s, %s, %d, %s\n",
                                    courses[i].courseID,
                                    courses[i].courseName,
                                    courses[i].dept,
                                    courses[i].sem,
                                    courses[i].faculty);
                        }
                    }

                    fclose(selectedFile);
                }

                free(courses);
                break;

    
}

void rankDisplayMenu() {
    infoget();
    
}

void allotmentViewer() {
    char reg[MAX_REG_LENGTH];
    
    printf("Enter registration number: ");
    scanf("%s", reg);

    display_allotments("allotment_status.csv", reg)

    
}