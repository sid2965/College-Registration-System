// common.h
#ifndef COMMON_H
#define COMMON_H
#define ALLOTMENT_FILE "allotment_status.csv"
#define STUDENTS_RANKED "students_ranked.csv"
#define COURSE_FILE "courses.csv"
#define STUDENT_FILE "students.csv"
#define ADMIN_FILE "admins.csv"
#define COMPLETED_COURSES "completedCourses.csv"
#define SELECTED_COURSES "selected_courses.csv"
#define SELECTION_STATUS_FILE "selection_status.txt"
#define MAX_COURSE_CODE_LENGTH 10
#define MAX_COURSE_NAME_LENGTH 100
#define MAX_FACULTY_NAME_LENGTH 100
#define MAX_NAME_LENGTH 100
#define MAX_REG_LENGTH 20
#define MAX_DEPT_LENGTH 5
#define MAX_SEM_LENGTH 2
#define MAX_CHOICES 6
#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_STUDENTS 150
#define MAX_WAITLIST 15
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_EMAIL_LENGTH 30
#define MAX_FIELD_LENGTH 256
#define MAX_SELECTED_COURSES 6
#define MIN_SELECTED_COURSES 4





struct student {
    int reg; // Registration number
    char name[50];
    char date[20];
    char address[100];
    char dept[5];
    float gpa;
    int workshops;
    int internships;
    int certificateCourses;
    int competitions;
    float meritScore;
    int sem;
    char completedCourses[8][10]; // Array to store up to 8 completed course IDs
    int completedCourseCount; // Count of completed courses
};


typedef struct {
    char name[MAX_NAME_LENGTH];
    char reg[MAX_REG_LENGTH];
    char course_allocation[MAX_CHOICES][10];  // Store up to 6 course codes
    char faculty[MAX_CHOICES][50];  // Store the faculty for each allotted course
} Allotment;


struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char email[MAX_EMAIL_LENGTH];
};

typedef struct {
    char course_code[MAX_COURSE_CODE_LENGTH];
    char course_name[MAX_COURSE_NAME_LENGTH];
    char faculty[MAX_FACULTY_NAME_LENGTH];
    int seats_available;
} Course;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char reg[MAX_REG_LENGTH];
    int meritscore;
    char dept[MAX_DEPT_LENGTH];
    char sem[MAX_SEM_LENGTH];
    char choices[MAX_CHOICES][MAX_COURSE_CODE_LENGTH];
} Student;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char reg[MAX_REG_LENGTH];
    int meritscore;
    char dept[MAX_DEPT_LENGTH];
    char sem[MAX_SEM_LENGTH];
    char course_allocation[MAX_COURSE_CODE_LENGTH];
    int waiting_list_number;
} Allotment;

typedef struct {
    char course_code[MAX_COURSE_CODE_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    char reg[MAX_REG_LENGTH];
    int meritscore;
} WaitingListEntry;

typedef struct {
    char courseID[MAX_FIELD_LENGTH];
    char courseName[MAX_FIELD_LENGTH];
    char dept[MAX_FIELD_LENGTH];
    int sem;
    int availableSeats;
    char faculty[MAX_FIELD_LENGTH];
    int selected; // New field to track course selection
    char prerequisite[MAX_FIELD_LENGTH]; // Field to store the prerequisite course
} Course;