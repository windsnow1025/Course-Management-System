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

/* Function to create basic data structures */

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

/* Function to create nodes */

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
    node->score = 0;
    node->next = NULL;
    return node;
}

/* Function to add nodes */

void addStudentNode(StudentNode** headPtr, StudentNode* node) {
    StudentNode* head = *headPtr;
    if (head == NULL) {
        *headPtr = node;
        return;
    }
    StudentNode* curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = node;
}

void addCourseNode(CourseNode** rootPtr, CourseNode* node) {
    CourseNode* root = *rootPtr;
    if (root == NULL) {
        *rootPtr = node;
        return;
    }
    int cmp = strcmp(node->data->id, root->data->id);
    if (cmp < 0) {
        addCourseNode(&(root->left), node);
    }
    else if (cmp > 0) {
        addCourseNode(&(root->right), node);
    }
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
            printf("Course registration successful.\n");
            return;
        }
        currStudent = currStudent->next;
    }
}

/* Function to delete nodes */

CourseNode* minValueNode(CourseNode* node) {
    CourseNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

void deleteCourseNode(CourseNode** rootPtr, char* id) {
    CourseNode* root = *rootPtr;
    if (root == NULL) {
        return;
    }
    int cmp = strcmp(id, root->data->id);
    if (cmp < 0) {
        deleteCourseNode(&(root->left), id);
    }
    else if (cmp > 0) {
        deleteCourseNode(&(root->right), id);
    }
    else {
        if (root->left == NULL) {
            *rootPtr = root->right;
            free(root);
        }
        else if (root->right == NULL) {
            *rootPtr = root->left;
            free(root);
        }
        else {
            CourseNode* temp = minValueNode(root->right);
            root->data = temp->data;
            deleteCourseNode(&(root->right), temp->data->id);
        }
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

/* Function to modify nodes */

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

/* Function to count nodes */

int countCourses(CourseNode* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countCourses(root->left) + countCourses(root->right);
}

int countCourseStudents(CourseNode* courseNode) {
    int count = 0;
    CourseStudentNode* student = courseNode->data->studentNode;
    while (student != NULL) {
        count++;
        student = student->next;
    }
    return count;
}

/* Function to query nodes */

int queryCourseScore(CourseNode* courseNode, char* studentId) {
    if (courseNode == NULL) {
        return -1;
    }
    CourseStudentNode* student = courseNode->data->studentNode;
    while (student != NULL) {
        if (strcmp(student->data->id, studentId) == 0) {
            return student->score;
        }
        student = student->next;
    }
    return -1;
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

CourseNode* searchCourseByTeacherId(CourseNode* root, char* teacherId) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(root->data->teacher->id, teacherId) == 0) {
        return root;
    }
    CourseNode* courseNode = searchCourseByTeacherId(root->left, teacherId);
    if (courseNode != NULL) {
        return courseNode;
    }
    return searchCourseByTeacherId(root->right, teacherId);
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

/* Function to print nodes */

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

void printCourseStudents(CourseNode* courseNode) {
    if (courseNode == NULL) {
        printf("Course not found.\n");
        return;
    }
    printf("Students in course %s:\n", courseNode->data->id);
    CourseStudentNode* student = courseNode->data->studentNode;
    while (student != NULL) {
        printf("Student ID: %s\n", student->data->id);
        student = student->next;
    }
}

void printCourseScoreStatistics(CourseNode* courseNode) {
    if (courseNode == NULL) {
        printf("Course not found.\n");
        return;
    }
    int count90 = 0, count80 = 0, count60 = 0, countBelow60 = 0;
    CourseStudentNode* student = courseNode->data->studentNode;
    while (student != NULL) {
        if (student->score >= 90) {
            count90++;
        }
        else if (student->score >= 80) {
            count80++;
        }
        else if (student->score >= 60) {
            count60++;
        }
        else {
            countBelow60++;
        }
        student = student->next;
    }
    printf("Statistics of course %s:\n", courseNode->data->id);
    printf("90 and above: %d\n", count90);
    printf("80 to 89: %d\n", count80);
    printf("60 to 79: %d\n", count60);
    printf("Below 60: %d\n", countBelow60);
}

/* Function to login */

Student* studentLogin(StudentNode* studentNode) {
    char id[20];
    char password[20];
    printf("Please enter your student ID: ");
    scanf("%s", id);
    while ((getchar()) != '\n');
    printf("Please enter your password: ");
    scanf("%s", password);
    while ((getchar()) != '\n');
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
    while ((getchar()) != '\n');
    printf("Please enter your password: ");
    scanf("%s", password);
    while ((getchar()) != '\n');
    Teacher* teacher = searchTeacherInCourse(root, id, password);
    if (teacher != NULL) {
        printf("Login successful.\n");
        return teacher;
    }
    printf("Login failed.\n");
    return NULL;
}

int adminLogin() {
    char username[20];
    char password[20];
    printf("Please enter your username: ");
    scanf("%s", username);
    while ((getchar()) != '\n');
    printf("Please enter your password: ");
    scanf("%s", password);
    while ((getchar()) != '\n');
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        printf("Login successful.\n");
        return 1;
    }
    printf("Login failed.\n");
    return 0;
}

/* Function to display menus */

void studentMenu(Student* student, CourseNode* root) {
    int choice;
    char courseId[20];
    CourseNode* courseNode;
    int score;
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
<<<<<<< Updated upstream
=======
        while ((getchar()) != '\n');
>>>>>>> Stashed changes
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
                score = queryCourseScore(courseNode, student->id);
                if (score == -1) {
					printf("You are not registered for this course.\n");
				}
				else {
					printf("Your score for this course is %d.\n", score);
				}
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
        system("pause");
    }
}

void teacherMenu(Teacher* teacher, CourseNode* root) {
    int choice;
    char courseId[20];
    char studentId[20];
    int currentScore;
    int newScore;
    CourseNode* courseNode = searchCourseByTeacherId(root, teacher->id);
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
<<<<<<< Updated upstream
=======
        while ((getchar()) != '\n');
>>>>>>> Stashed changes
        switch (choice) {
        case 1:
            printCourse(courseNode);
            break;
        case 2:
            printCourseStudents(courseNode);
            break;
        case 3:
            printf("Please enter the student ID: ");
            scanf("%s", studentId);
            currentScore = queryCourseScore(courseNode, studentId);
            if (currentScore == 0) {
                printf("Please enter the new score: ");
                scanf("%d", &newScore);
                modifyStudentScore(courseNode, studentId, newScore);
            }
            else if (currentScore == -1) {
				printf("Student not found.\n");
			}
            else {
                printf("Score already exists. Please use Modify Course Score.\n");
            }
            break;
        case 4:
            printf("Please enter the student ID: ");
            scanf("%s", studentId);
            currentScore = queryCourseScore(courseNode, studentId);
            if (currentScore != 0) {
                printf("Please enter the new score: ");
                scanf("%d", &newScore);
                modifyStudentScore(courseNode, studentId, newScore);
            }
            else if (currentScore == -1) {
                printf("Student not found.\n");
			}
            else {
                printf("No score found. Please use Input Course Score.\n");
            }
            break;
        case 5:
            printCourseScoreStatistics(courseNode);
            break;
        case 6:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        system("pause");
    }
}

void adminMenu(CourseNode** root) {
    int choice;
    char courseId[20];
    char courseName[20];
    int courseCredit;
    char teacherId[20];
    char teacherPassword[20];
    char courseTime[20];
    char courseLocation[20];
    CourseNode* courseNode;
    while (1) {
        system("cls");
        printf("1. Create Course Information\n");
        printf("2. Add Course Information\n");
        printf("3. Delete Course Information\n");
        printf("4. Count Total Courses\n");
        printf("5. Query Course Student Count\n");
        printf("6. Browse All Course Information\n");
        printf("7. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
<<<<<<< Updated upstream
=======
        while ((getchar()) != '\n');
>>>>>>> Stashed changes
        switch (choice) {
        case 1:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            printf("Please enter the course name: ");
            scanf("%s", courseName);
            printf("Please enter the course credit: ");
            scanf("%d", &courseCredit);
            printf("Please enter the teacher ID: ");
            scanf("%s", teacherId);
            printf("Please enter the teacher password: ");
            scanf("%s", teacherPassword);
            while ((getchar()) != '\n');
            printf("Please enter the course time: ");
            fgets(courseTime, sizeof(courseTime), stdin);
            courseTime[strcspn(courseTime, "\n")] = 0;
            printf("Please enter the course location: ");
            fgets(courseLocation, sizeof(courseLocation), stdin);
            courseLocation[strcspn(courseLocation, "\n")] = 0;
            Teacher* teacher = createTeacher(teacherId, teacherPassword);
            Course* course = createCourse(courseId, courseName, courseCredit, teacher, courseTime, courseLocation);
            CourseNode* courseNode = createCourseNode(course);
            addCourseNode(root, courseNode);
            printf("Course created successfully.\n");
            break;
        case 2:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(*root, courseId);
            if (courseNode != NULL) {
                printf("Please enter the new course name: ");
                scanf("%s", courseName);
                printf("Please enter the new course credit: ");
                scanf("%d", &courseCredit);
                printf("Please enter the new teacher ID: ");
                scanf("%s", teacherId);
                printf("Please enter the new teacher password: ");
                scanf("%s", teacherPassword);
                while ((getchar()) != '\n');
                printf("Please enter the new course time: ");
                fgets(courseTime, sizeof(courseTime), stdin);
                courseTime[strcspn(courseTime, "\n")] = 0;
                printf("Please enter the new course location: ");
                fgets(courseLocation, sizeof(courseLocation), stdin);
                courseLocation[strcspn(courseLocation, "\n")] = 0;
                strcpy(courseNode->data->name, courseName);
                courseNode->data->credit = courseCredit;
                strcpy(courseNode->data->teacher->id, teacherId);
                strcpy(courseNode->data->teacher->password, teacherPassword);
                strcpy(courseNode->data->time, courseTime);
                strcpy(courseNode->data->location, courseLocation);
                printf("Course information updated successfully.\n");
            }
            else {
                printf("Course not found.\n");
            }
            break;
        case 3:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            deleteCourseNode(root, courseId);
            printf("Course deleted successfully.\n");
            break;
        case 4:
            printf("Total number of courses: %d\n", countCourses(*root));
            break;
        case 5:
            printf("Please enter the course ID: ");
            scanf("%s", courseId);
            courseNode = searchCourse(*root, courseId);
            if (courseNode != NULL) {
                printf("Number of students in course %s: %d\n", courseId, countCourseStudents(courseNode));
            }
            else {
                printf("Course not found.\n");
            }
            break;
        case 6:
            printCourses(*root);
            break;
        case 7:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        system("pause");
    }
}

void loginMenu(CourseNode* courseNode, StudentNode* studentNode) {
    int choice;
    Student* student;
    Teacher* teacher;
    int admin;
    while (1) {
        system("cls");
        printf("1. Student Login\n");
        printf("2. Teacher Login\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
<<<<<<< Updated upstream
=======
        while ((getchar()) != '\n');
>>>>>>> Stashed changes
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
            admin = adminLogin();
            if (admin) {
                adminMenu(&courseNode);
            }
            break;
        case 4:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        system("pause");
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
    addCourseNode(&root, courseNode1);
    addCourseNode(&root, courseNode2);

    // Build the student linked list
    StudentNode* studentNode = NULL;
    addStudentNode(&studentNode, studentNode1);
    addStudentNode(&studentNode, studentNode2);
    addStudentNode(&studentNode, studentNode3);

    // Add students to courses
    addCourseStudentNode(courseNode1, student1);
    addCourseStudentNode(courseNode1, student2);
    addCourseStudentNode(courseNode2, student1);
    addCourseStudentNode(courseNode2, student3);

    // Print data
    printCourses(root);
    system("pause");

    // Login menu
    loginMenu(root, studentNode);
    return 0;
}