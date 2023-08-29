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
    Student* data;
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
    Teacher* teacher;
    char time[20];
    char location[20];
    StudentNode* students;
} Course;

typedef struct CourseNode {
    Course* data;
    struct CourseNode* left;
    struct CourseNode* right;
} CourseNode;

Student* createStudent(char* id, char* name, char* phone, char* password) {
    Student* student = (Student*)malloc(sizeof(Student));
    strcpy(student->id, id);
    strcpy(student->name, name);
    strcpy(student->phone, phone);
    strcpy(student->password, password);
    return student;
}

Teacher* createTeacher(char* id, char* password) {
    Teacher* teacher = (Teacher*)malloc(sizeof(Teacher));
    strcpy(teacher->id, id);
    strcpy(teacher->password, password);
    return teacher;
}

Course* createCourse(char* id, char* name, int credit, Teacher* teacher, char* time, char* location) {
    Course* course = (Course*)malloc(sizeof(Course));
    strcpy(course->id, id);
    strcpy(course->name, name);
    course->credit = credit;
    course->teacher = teacher;
    strcpy(course->time, time);
    strcpy(course->location, location);
    course->students = NULL;
    return course;
}

CourseNode* createCourseNode(Course* course) {
    CourseNode* node = (CourseNode*)malloc(sizeof(CourseNode));
    node->data = course;
    node->left = NULL;
    node->right = NULL;
    return node;
}

StudentNode* createStudentNode(Student* student) {
    StudentNode* node = (StudentNode*)malloc(sizeof(StudentNode));
    node->data = student;
    node->score = -1;
    node->next = NULL;
    return node;
}

CourseNode* insertCourseNode(CourseNode* root, CourseNode* node) {
    if (root == NULL) {
        return node;
    }
    int cmp = strcmp(node->data->id, root->data->id);
    if (cmp < 0) {
        root->left = insertCourseNode(root->left, node);
    }
    else if (cmp > 0) {
        root->right = insertCourseNode(root->right, node);
    }
    return root;
}

void addStudentToCourseNode(CourseNode* courseNode, StudentNode* studentNode) {
    if (courseNode == NULL) {
        return;
    }
    if (courseNode->data->students == NULL) {
        courseNode->data->students = studentNode;
        return;
    }
    StudentNode* currStudent = courseNode->data->students;
    while (currStudent->next != NULL) {
        currStudent = currStudent->next;
    }
    currStudent->next = studentNode;
}

CourseNode* searchCourse(CourseNode* root, char* id) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = strcmp(id, root->data->id);
    if (cmp < 0) {
        return searchCourse(root->left, id);
    }
    else if (cmp > 0) {
        return searchCourse(root->right, id);
    }
    else {
        return root;
    }
}

void modifyStudentScore(CourseNode* courseNode, char* studentId, int newScore) {
    if (courseNode == NULL) {
        printf("Course not found.\n");
        return;
    }
    StudentNode* student = courseNode->data->students;
    while (student != NULL) {
        if (strcmp(student->data->id, studentId) == 0) {
            student->score = newScore;
            printf("Score of student %s in course %s has been modified to %d.\n", studentId, courseNode->data->id, newScore);
            return;
        }
        student = student->next;
    }
    printf("Student not found in this course.\n");
}

void printCourse(CourseNode* courseNode) {
    if (courseNode == NULL) {
        printf("Course not found.\n");
        return;
    }
    printf("Course ID: %s\n", courseNode->data->id);
    printf("Course Name: %s\n", courseNode->data->name);
    printf("Course Credit: %d\n", courseNode->data->credit);
    printf("Course Teacher: %s\n", courseNode->data->teacher->id);
    printf("Course Time: %s\n", courseNode->data->time);
    printf("Course Location: %s\n", courseNode->data->location);
    printf("Students:\n");
    StudentNode* student = courseNode->data->students;
    while (student != NULL) {
        printf("Student ID: %s, Score: %d\n", student->data->id, student->score);
        student = student->next;
    }
}

void loopMenu(CourseNode* course) {
}

int main() {
    Teacher* teacher1 = createTeacher("T001", "123456");
    Teacher* teacher2 = createTeacher("T002", "123456");

    Student* student1 = createStudent("S001", "Tom", "12345678901", "123456");
    Student* student2 = createStudent("S002", "Jerry", "12345678902", "123456");

    Course* course1 = createCourse("C001", "Computer Science", 4, teacher1, "Monday 10:00", "Room 101");
    Course* course2 = createCourse("C002", "Data Structures", 3, teacher2, "Tuesday 14:00", "Room 102");

    StudentNode* studentNode1 = createStudentNode(student1);
    StudentNode* studentNode2 = createStudentNode(student2);
    CourseNode* courseNode1 = createCourseNode(course1);
    CourseNode* courseNode2 = createCourseNode(course2);

    CourseNode* root = NULL;
    root = insertCourseNode(root, courseNode1);
    root = insertCourseNode(root, courseNode2);

    addStudentToCourseNode(courseNode1, studentNode1);
    addStudentToCourseNode(courseNode1, studentNode2);

    modifyStudentScore(courseNode1, "S001", 90);
    modifyStudentScore(courseNode1, "S002", 80);

    printf("Search and print course C001:\n");
    printCourse(searchCourse(root, "C001"));

    strcpy(student1->id, "S003");

    printf("\nSearch and print course C001 again:\n");
	printCourse(searchCourse(root, "C001"));

    loopMenu(root);
    return 0;
}