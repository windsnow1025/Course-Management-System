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

void addCourseStudentNode(CourseNode* courseNode, Student* student) {
    CourseStudentNode* courseStudentNode = createCourseStudentNode(student);
    if (courseNode == NULL) {
        return;
    }
    if (courseNode->data->studentNode == NULL) {
        courseNode->data->studentNode = courseStudentNode;
        return;
    }
    CourseStudentNode* currStudent = courseNode->data->studentNode;
    while (currStudent != NULL) {
        if (strcmp(currStudent->data->id, student->id) == 0) {
            printf("Student already exists in this course.\n");
            free(courseStudentNode);
            return;
        }
        if (currStudent->next == NULL) {
            currStudent->next = courseStudentNode;
            return;
        }
        currStudent = currStudent->next;
    }
}

void delCourseStudentNode(CourseNode* courseNode, char* studentId) {
    if (courseNode == NULL) {
        printf("Course not found.\n");
        return;
    }
    CourseStudentNode* prevStudent = NULL;
    CourseStudentNode* currStudent = courseNode->data->studentNode;
    while (currStudent != NULL) {
        if (strcmp(currStudent->data->id, studentId) == 0) {
            if (prevStudent == NULL) {
                courseNode->data->studentNode = currStudent->next;
            }
            else {
                prevStudent->next = currStudent->next;
            }
            free(currStudent);
            printf("Course registration cancelled successfully.\n");
            return;
        }
        prevStudent = currStudent;
        currStudent = currStudent->next;
    }
    printf("Student not found in this course.\n");
}

void queryCourseScore(CourseNode* courseNode, char* studentId) {
    if (courseNode == NULL) {
        printf("Course not found.\n");
        return;
    }
    CourseStudentNode* student = courseNode->data->studentNode;
    while (student != NULL) {
        if (strcmp(student->data->id, studentId) == 0) {
            printf("Your score in course %s is %d.\n", courseNode->data->id, student->score);
            return;
        }
        student = student->next;
    }
    printf("Student not found in this course.\n");
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

Teacher* searchTeacherInCourse(CourseNode* root, char* id, char* password) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(root->data->teacher->id, id) == 0 && strcmp(root->data->teacher->password, password) == 0) {
        return root->data->teacher;
    }
    Teacher* teacher = searchTeacherInCourse(root->left, id, password);
    if (teacher != NULL) {
        return teacher;
    }
    return searchTeacherInCourse(root->right, id, password);
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
    printf("------------------------------------------\n");
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
    printf("------------------------------------------\n\n");
}

void printCourses(CourseNode* root) {
	if (root == NULL) {
		return;
	}
	printCourses(root->left);
	printCourse(root);
	printCourses(root->right);
}

Student* studentLogin(StudentNode* studentNode) {
    char id[20];
    char password[20];
    printf("Please enter your student ID: ");
    scanf("%s", id);
    printf("Please enter your password: ");
    scanf("%s", password);
    while (studentNode != NULL) {
        if (strcmp(studentNode->data->id, id) == 0 && strcmp(studentNode->data->password, password) == 0) {
            printf("Login successful.\n");
            return studentNode->data;
        }
        studentNode = studentNode->next;
    }
    printf("Login failed.\n");
    return NULL;
}

Teacher* teacherLogin(CourseNode* root) {
    char id[20];
    char password[20];
    printf("Please enter your teacher ID: ");
    scanf("%s", id);
    printf("Please enter your password: ");
    scanf("%s", password);
    Teacher* teacher = searchTeacherInCourse(root, id, password);
    if (teacher != NULL) {
        printf("Login successful.\n");
        return teacher;
    }
    printf("Login failed.\n");
    return NULL;
}

void studentMenu(Student* student, CourseNode* root) {
    int choice;
    char courseId[20];
    CourseNode* courseNode;
    while (1) {
        system("cls");
        printf("1. Query Course Information\n");
        printf("2. Register for a Course\n");
        printf("3. Cancel Course Registration\n");
        printf("4. Query Course Score\n");
        printf("5. Browse All Course Information\n");
        printf("6. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                printCourse(courseNode);
            }
            else {
                printf("Course not found.\n");
            }
            break;
        case 2:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                addCourseStudentNode(courseNode, student);
                printf("Course registration successful.\n");
            }
            else {
                printf("Course not found.\n");
            }
            break;
        case 3:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                delCourseStudentNode(courseNode, student->id);
            }
            else {
                printf("Course not found.\n");
            }
            break;
        case 4:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                queryCourseScore(courseNode, student->id);
            }
            else {
                printf("Course not found.\n");
            }
            break;
        case 5:
            printCourses(root);
            break;
        case 6:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void teacherMenu(Teacher* teacher, CourseNode* root) {
    int choice;
    char courseId[20];
    char studentId[20];
    int newScore;
    CourseNode* courseNode;
    while (1) {
        system("cls");
        printf("1. Query My Course Information\n");
        printf("2. Query My Students Information\n");
        printf("3. Input Course Score\n");
        printf("4. Modify Course Score\n");
        printf("5. Statistics of Course Score\n");
        printf("6. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL && strcmp(courseNode->data->teacher->id, teacher->id) == 0) {
                printCourse(courseNode);
            }
            else {
                printf("Course not found or you are not the teacher of this course.\n");
            }
            break;
        case 2:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL && strcmp(courseNode->data->teacher->id, teacher->id) == 0) {
                printCourse(courseNode);
            }
            else {
                printf("Course not found or you are not the teacher of this course.\n");
            }
            break;
        case 3:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            printf("Please enter the student ID: ");
            scanf("%s", studentId);
            printf("Please enter the new score: ");
            scanf("%d", &newScore);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL && strcmp(courseNode->data->teacher->id, teacher->id) == 0) {
                modifyStudentScore(courseNode, studentId, newScore);
            }
            else {
                printf("Course not found or you are not the teacher of this course.\n");
            }
            break;
        case 4:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            printf("Please enter the student ID: ");
            scanf("%s", studentId);
            printf("Please enter the new score: ");
            scanf("%d", &newScore);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL && strcmp(courseNode->data->teacher->id, teacher->id) == 0) {
                modifyStudentScore(courseNode, studentId, newScore);
            }
            else {
                printf("Course not found or you are not the teacher of this course.\n");
            }
            break;
        case 5:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL && strcmp(courseNode->data->teacher->id, teacher->id) == 0) {
                // Enter the function of statistics of course score
            }
            else {
                printf("Course not found or you are not the teacher of this course.\n");
            }
            break;
        case 6:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void loginMenu(CourseNode* courseNode, StudentNode* studentNode) {
    int choice;
    Student* student;
    Teacher* teacher;
    while (1) {
        system("cls");
        printf("1. Student Login\n");
        printf("2. Teacher Login\n");
        printf("3. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            student = studentLogin(studentNode);
            if (student != NULL) {
                printf("User ID: %s\n", student->id);
                studentMenu(student, courseNode);
            }
            break;
        case 2:
            teacher = teacherLogin(courseNode);
            if (teacher != NULL) {
                printf("User ID: %s\n", teacher->id);
                teacherMenu(teacher, courseNode);
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
    // Create data
    Teacher* teacher1 = createTeacher("T001", "123456");
    Teacher* teacher2 = createTeacher("T002", "123456");
    Student* student1 = createStudent("S001", "Tom", "12345678901", "123456");
    Student* student2 = createStudent("S002", "Jerry", "12345678902", "123456");
    Student* student3 = createStudent("S003", "John", "12345678903", "123456");
    Course* course1 = createCourse("C001", "Computer Science", 4, teacher1, "Monday 10:00", "Room 101");
    Course* course2 = createCourse("C002", "Data Structures", 3, teacher2, "Tuesday 14:00", "Room 102");

    // Create nodes
    StudentNode* studentNode1 = createStudentNode(student1);
    StudentNode* studentNode2 = createStudentNode(student2);
    StudentNode* studentNode3 = createStudentNode(student3);
    CourseNode* courseNode1 = createCourseNode(course1);
    CourseNode* courseNode2 = createCourseNode(course2);

    // Build the course tree
    CourseNode* root = NULL;
    root = addCourseNode(root, courseNode1);
    root = addCourseNode(root, courseNode2);

    // Build the student linked list
    StudentNode* studentNode = NULL;
    studentNode = addStudentNode(studentNode, studentNode1);
	studentNode = addStudentNode(studentNode, studentNode2);
    studentNode = addStudentNode(studentNode, studentNode3);

    // Add students to courses
    addCourseStudentNode(courseNode1, student1);
	addCourseStudentNode(courseNode1, student2);
    addCourseStudentNode(courseNode2, student1);
    addCourseStudentNode(courseNode2, student3);

    printCourses(root);

    loginMenu(root, studentNode);
    return 0;
}