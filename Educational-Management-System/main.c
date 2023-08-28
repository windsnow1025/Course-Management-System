#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

typedef struct Student {
    char id[20];
    char name[20];
    char phone[20];
    char password[20];
} Student;

typedef struct StudentNode {
    Student data;
    int score;
    struct StudentNode* next;
} StudentNode;

typedef struct Teacher {
    char id[20];
    char password[20];
} Teacher;

typedef struct Course {
    char id[20];
    char name[20];
    int credit;
    Teacher teacher;
    char time[20];
    char location[20];
    StudentNode* students;
} Course;

typedef struct CourseNode {
    Course data;
    struct CourseNode* left;
    struct CourseNode* right;
} CourseNode;

CourseNode* createCourse(char* id, char* name, int credit, Teacher teacher, char* time, char* location) {
    CourseNode* node = (CourseNode*)malloc(sizeof(CourseNode));
    strcpy(node->data.id, id);
    strcpy(node->data.name, name);
    node->data.credit = credit;
    node->data.teacher = teacher;
    strcpy(node->data.time, time);
    strcpy(node->data.location, location);
    node->data.students = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

CourseNode* insertCourse(CourseNode* root, CourseNode* node) {
    if (root == NULL) {
        return node;
    }
    int cmp = strcmp(node->data.id, root->data.id);
    if (cmp < 0) {
        root->left = insertCourse(root->left, node);
    }
    else if (cmp > 0) {
        root->right = insertCourse(root->right, node);
    }
    return root;
}

void addStudent(CourseNode* course, Student student, int score) {
    StudentNode* node = (StudentNode*)malloc(sizeof(StudentNode));
    node->data = student;
    node->score = score;
    node->next = course->data.students;
    course->data.students = node;
}

CourseNode* searchCourse(CourseNode* root, char* id) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = strcmp(id, root->data.id);
    if (cmp < 0) {
        return searchCourse(root->left, id);
    } else if (cmp > 0) {
        return searchCourse(root->right, id);
    } else {
        return root;
    }
}

void printCourse(CourseNode* course) {
    if (course == NULL) {
        printf("Course not found.\n");
        return;
    }
    printf("Course ID: %s\n", course->data.id);
    printf("Course Name: %s\n", course->data.name);
    printf("Course Credit: %d\n", course->data.credit);
    printf("Course Teacher: %s\n", course->data.teacher.id);
    printf("Course Time: %s\n", course->data.time);
    printf("Course Location: %s\n", course->data.location);
    printf("Students:\n");
    StudentNode* student = course->data.students;
    while (student != NULL) {
        printf("Student ID: %s, Score: %d\n", student->data.id, student->score);
        student = student->next;
    }
}

void loopMenu(CourseNode* course) {
}


int main() {
    Teacher teacher1 = { "T001", "123456" };
    Teacher teacher2 = { "T002", "123456" };
    Student student1 = { "S001", "Tom", "12345678901", "123456" };
    Student student2 = { "S002", "Jerry", "12345678902", "123456" };

    CourseNode* course1 = createCourse("C001", "Computer Science", 4, teacher1, "Monday 10:00", "Room 101");
    CourseNode* course2 = createCourse("C002", "Data Structures", 3, teacher2, "Tuesday 14:00", "Room 102");
    
    addStudent(course1, student1, 90);
    addStudent(course1, student2, 85);

    CourseNode* root = NULL;
    root = insertCourse(root, course1);
    root = insertCourse(root, course2);

    printf("Search and print course C001:\n");
    printCourse(searchCourse(root, "C001"));

    loopMenu(root);
    return 0;
}