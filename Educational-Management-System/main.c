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
    struct StudentNode* next;
} StudentNode;

typedef struct CourseStudentNode {
    Student* data;
    int score;
    struct CourseStudentNode* next;
} CourseStudentNode;

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
    CourseStudentNode* studentNode;
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
    course->studentNode = NULL;
    return course;
}

StudentNode* createStudentNode(Student* student) {
    StudentNode* node = (StudentNode*)malloc(sizeof(StudentNode));
    node->data = student;
    node->next = NULL;
    return node;
}

CourseNode* createCourseNode(Course* course) {
    CourseNode* node = (CourseNode*)malloc(sizeof(CourseNode));
    node->data = course;
    node->left = NULL;
    node->right = NULL;
    return node;
}

CourseStudentNode* createCourseStudentNode(Student* student) {
    CourseStudentNode* node = (CourseStudentNode*)malloc(sizeof(CourseStudentNode));
    node->data = student;
    node->score = -1;
    node->next = NULL;
    return node;
}

StudentNode* addStudentNode(StudentNode* head, StudentNode* node) {
	if (head == NULL) {
		return node;
	}
	StudentNode* curr = head;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = node;
	return head;
}

CourseNode* addCourseNode(CourseNode* root, CourseNode* node) {
    if (root == NULL) {
        return node;
    }
    int cmp = strcmp(node->data->id, root->data->id);
    if (cmp < 0) {
        root->left = addCourseNode(root->left, node);
    }
    else if (cmp > 0) {
        root->right = addCourseNode(root->right, node);
    }
    return root;
}

void addCourseStudentNode(CourseNode* courseNode, CourseStudentNode* courseStudentNode) {
    if (courseNode == NULL) {
        return;
    }
    if (courseNode->data->studentNode == NULL) {
        courseNode->data->studentNode = courseStudentNode;
        return;
    }
    CourseStudentNode* currStudent = courseNode->data->studentNode;
    while (currStudent->next != NULL) {
        currStudent = currStudent->next;
    }
    currStudent->next = courseStudentNode;
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
    CourseStudentNode* student = courseNode->data->studentNode;
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
    CourseStudentNode* student = courseNode->data->studentNode;
    while (student != NULL) {
        printf("Student ID: %s, Score: %d\n", student->data->id, student->score);
        student = student->next;
    }
}

void iterateCourse(CourseNode* root) {
	if (root == NULL) {
		return;
	}
	iterateCourse(root->left);
	printCourse(root);
	iterateCourse(root->right);
}

char* studentLogin(StudentNode* node) {
    char id[20];
    char password[20];
    printf("Please enter your student ID: ");
    scanf("%s", id);
    printf("Please enter your password: ");
    scanf("%s", password);
    while (node != NULL) {
        if (strcmp(node->data->id, id) == 0 && strcmp(node->data->password, password) == 0) {
            printf("Login successful.\n");
            return node->data->id;
        }
        node = node->next;
    }
    printf("Login failed.\n");
    return NULL;
}

Teacher* searchTeacherInCourse(CourseNode* root, char* id, char* password) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(root->data->teacher->id, id) == 0 && strcmp(root->data->teacher->password, password) == 0) {
        return root->data->teacher;
    }
    Teacher* found = searchTeacherInCourse(root->left, id, password);
    if (found != NULL) {
        return found;
    }
    return searchTeacherInCourse(root->right, id, password);
}

char* teacherLogin(CourseNode* root) {
    char id[20];
    char password[20];
    printf("Please enter your teacher ID: ");
    scanf("%s", id);
    printf("Please enter your password: ");
    scanf("%s", password);
    Teacher* teacher = searchTeacherInCourse(root, id, password);
    if (teacher != NULL) {
        printf("Login successful.\n");
        return teacher->id;
    }
    printf("Login failed.\n");
    return NULL;
}

void loopMenu(CourseNode* courseNode, StudentNode* studentNode) {
    int choice;
    char* userId;
    while (1) {
        printf("1. Student Login\n");
        printf("2. Teacher Login\n");
        printf("3. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            userId = studentLogin(studentNode);
            if (userId != NULL) {
                // Enter student menu
            }
            break;
        case 2:
            userId = teacherLogin(courseNode);
            if (userId != NULL) {
                // Enter teacher menu
            }
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
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

    CourseStudentNode* courseStudentNode1 = createCourseStudentNode(student1);
    CourseStudentNode* courseStudentNode2 = createCourseStudentNode(student2);

    CourseNode* root = NULL;
    root = addCourseNode(root, courseNode1);
    root = addCourseNode(root, courseNode2);

    StudentNode* studentNode = NULL;
    studentNode = addStudentNode(studentNode, studentNode1);
	studentNode = addStudentNode(studentNode, studentNode2);

    addCourseStudentNode(courseNode1, courseStudentNode1);
    addCourseStudentNode(courseNode1, courseStudentNode2);

    modifyStudentScore(courseNode1, "S001", 90);
    modifyStudentScore(courseNode1, "S002", 80);

    printf("Search and print course C001:\n");
    iterateCourse(root);

    strcpy(student1->id, "S003");

    printf("\nSearch and print course C001 again:\n");
    iterateCourse(root);

    loopMenu(root, studentNode);
    return 0;
}