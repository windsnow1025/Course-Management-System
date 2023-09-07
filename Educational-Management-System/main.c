#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

typedef struct Student {
    char id[64];
    char name[64];
    char phone[64];
    char password[64];
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
    char id[64];
    char password[64];
} Teacher;

typedef struct Course {
    char id[64];
    char name[64];
    int credit;
    Teacher* teacher;
    char time[64];
    char location[64];
    CourseStudentNode* courseStudentNode;
} Course;

typedef struct CourseNode {
    Course* data;
    int height;
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
    course->courseStudentNode = NULL;
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
    node->height = 1;
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

/* Function for AVL Tree */

//int max(int a, int b) {
//	return (a > b) ? a : b;
//}

int height(CourseNode* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

CourseNode* rightRotate(CourseNode* y) {
    CourseNode* x = y->left;
    CourseNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

CourseNode* leftRotate(CourseNode* x) {
    CourseNode* y = x->right;
    CourseNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(CourseNode* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

void balance(CourseNode** node) {
    if (*node == NULL)
        return;

    /* Update height of this ancestor node */
    (*node)->height = 1 + max(height((*node)->left), height((*node)->right));

    /* Get the balance factor of this ancestor node to check whether this node became unbalanced */
    int balance = getBalance(*node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance((*node)->left) >= 0)
        *node = rightRotate(*node);

    // Right Right Case
    if (balance < -1 && getBalance((*node)->right) <= 0)
        *node = leftRotate(*node);

    // Left Right Case
    if (balance > 1 && getBalance((*node)->left) < 0) {
        (*node)->left = leftRotate((*node)->left);
        *node = rightRotate(*node);
    }

    // Right Left Case
    if (balance < -1 && getBalance((*node)->right) > 0) {
        (*node)->right = rightRotate((*node)->right);
        *node = leftRotate(*node);
    }
}

void balanceTree(CourseNode** node) {
    if (*node == NULL)
        return;

    // Balance the left subtree
    balanceTree(&((*node)->left));

    // Balance the right subtree
    balanceTree(&((*node)->right));

    // Balance this node
    balance(node);
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
    balanceTree(rootPtr);
}

void addCourseStudentNode(CourseNode* courseNode, Student* student) {
    CourseStudentNode* courseStudentNode = createCourseStudentNode(student);
    if (courseNode == NULL) {
        return;
    }
    if (courseNode->data->courseStudentNode == NULL) {
        courseNode->data->courseStudentNode = courseStudentNode;
        return;
    }
    CourseStudentNode* currStudent = courseNode->data->courseStudentNode;
    while (currStudent != NULL) {
        if (strcmp(currStudent->data->id, student->id) == 0) {
            printf("该课程中已存在此学生.\n");
            free(courseStudentNode);
            return;
        }
        if (currStudent->next == NULL) {
            currStudent->next = courseStudentNode;
            printf("课程报名成功.\n");
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
        printf("课程不存在.\n");
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
            // node with only one child or no child
            *rootPtr = root->right;
            free(root);
            printf("课程删除成功.\n");
        }
        else if (root->right == NULL) {
            // node with only one child or no child
            *rootPtr = root->left;
            free(root);
            printf("课程删除成功.\n");
        }
        else {
            // node with two children: Get the inorder successor (smallest in the right subtree)
            CourseNode* temp = minValueNode(root->right);
            // Copy the inorder successor's content to this node
            root->data = temp->data;
            // Delete the inorder successor
            deleteCourseNode(&(root->right), temp->data->id);
        }
    }
    balanceTree(rootPtr);
}

void delCourseStudentNode(CourseNode* courseNode, char* studentId) {
    if (courseNode == NULL) {
        printf("课程不存在.\n");
        return;
    }
    CourseStudentNode* prevStudent = NULL;
    CourseStudentNode* currStudent = courseNode->data->courseStudentNode;
    while (currStudent != NULL) {
        if (strcmp(currStudent->data->id, studentId) == 0) {
            if (prevStudent == NULL) {
                courseNode->data->courseStudentNode = currStudent->next;
            }
            else {
                prevStudent->next = currStudent->next;
            }
            free(currStudent);
            printf("课程取消报名成功.\n");
            return;
        }
        prevStudent = currStudent;
        currStudent = currStudent->next;
    }
    printf("该课程中不存在此学生.\n");
}

/* Function to modify nodes */

void modifyStudentScore(CourseNode* courseNode, char* studentId, int newScore) {
    if (courseNode == NULL) {
        printf("课程不存在.\n");
        return;
    }
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        if (strcmp(student->data->id, studentId) == 0) {
            student->score = newScore;
            printf("学生 %s 在课程 %s 中的分数已被修改为 %d.\n", studentId, courseNode->data->id, newScore);
            return;
        }
        student = student->next;
    }
    printf("该课程中不存在此学生.\n");
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
    CourseStudentNode* student = courseNode->data->courseStudentNode;
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
    CourseStudentNode* student = courseNode->data->courseStudentNode;
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
        printf("课程不存在.\n");
        return;
    }
    printf("------------------------------------------\n");
    printf("课程编号: %s\n", courseNode->data->id);
    printf("课程名: %s\n", courseNode->data->name);
    printf("课程学分: %d\n", courseNode->data->credit);
    printf("课程教师: %s\n", courseNode->data->teacher->id);
    printf("课程时间: %s\n", courseNode->data->time);
    printf("课程地点: %s\n", courseNode->data->location);
    printf("学生:\n");
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        printf("\t学号: %s, 分数: %d\n", student->data->id, student->score);
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
        printf("课程不存在.\n");
        return;
    }
    printf("Students in course %s:\n", courseNode->data->id);
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        printf("------------------------------------------\n");
        printf("学号: %s\n", student->data->id);
        printf("学生名: %s\n", student->data->name);
        printf("学生手机号: %s\n", student->data->phone);
        printf("学生分数: %d\n", student->score);
        student = student->next;
    }
}

void printCourseIds(CourseNode* root) {
    if (root == NULL) {
        return;
    }
    printCourseIds(root->left);
    printf("课程编号: %s\n", root->data->id);
    printCourseIds(root->right);
}

void printStudentIds(CourseStudentNode* head) {
    CourseStudentNode* curr = head;
    while (curr != NULL) {
        printf("学号: %s\n", curr->data->id);
        curr = curr->next;
    }
}

void printCourseScoreStatistics(CourseNode* courseNode) {
    if (courseNode == NULL) {
        printf("课程不存在.\n");
        return;
    }
    int count90 = 0, count80 = 0, count60 = 0, countBelow60 = 0;
    CourseStudentNode* student = courseNode->data->courseStudentNode;
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
    printf("课程分数统计 %s:\n", courseNode->data->id);
    printf("90 - 100: %d\n", count90);
    printf("80 - 89: %d\n", count80);
    printf("60 - 79: %d\n", count60);
    printf("0 - 60: %d\n", countBelow60);
}

/* Function to login */

Student* studentLogin(StudentNode* studentNode) {
    char id[64];
    char password[64];
    printf("请输入学号: ");
    scanf("%s", id);
    while ((getchar()) != '\n');
    printf("请输入密码: ");
    scanf("%s", password);
    while ((getchar()) != '\n');
    while (studentNode != NULL) {
        if (strcmp(studentNode->data->id, id) == 0 && strcmp(studentNode->data->password, password) == 0) {
            printf("登录成功.\n");
            return studentNode->data;
        }
        studentNode = studentNode->next;
    }
    printf("登录失败.\n");
    return NULL;
}

Teacher* teacherLogin(CourseNode* root) {
    char id[64];
    char password[64];
    printf("请输入教师编号: ");
    scanf("%s", id);
    while ((getchar()) != '\n');
    printf("请输入密码: ");
    scanf("%s", password);
    while ((getchar()) != '\n');
    Teacher* teacher = searchTeacherInCourse(root, id, password);
    if (teacher != NULL) {
        printf("登录成功.\n");
        return teacher;
    }
    printf("登录失败.\n");
    return NULL;
}

int adminLogin() {
    char username[64];
    char password[64];
    printf("请输入用户名: ");
    scanf("%s", username);
    while ((getchar()) != '\n');
    printf("请输入密码: ");
    scanf("%s", password);
    while ((getchar()) != '\n');
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        printf("登录成功.\n");
        return 1;
    }
    printf("登录失败.\n");
    return 0;
}

/* Function to display menus */

void studentMenu(Student* student, CourseNode* root) {
    int choice;
    char courseId[64];
    CourseNode* courseNode;
    int score;
    while (1) {
        choice = 0;
        system("cls");
        printf("学号: %s\n", student->id);
        printf("1. 查询课程信息\n");
        printf("2. 报名课程学习\n");
        printf("3. 取消报名课程\n");
        printf("4. 查询课程分数\n");
        printf("5. 浏览所有课程信息\n");
        printf("6. 退出登录\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        while ((getchar()) != '\n');
        switch (choice) {
        case 1:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                printCourse(courseNode);
            }
            else {
                printf("课程不存在.\n");
            }
            break;
        case 2:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                addCourseStudentNode(courseNode, student);
                printCourse(courseNode);
            }
            else {
                printf("课程不存在.\n");
            }
            break;
        case 3:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                delCourseStudentNode(courseNode, student->id);
                printCourse(courseNode);
            }
            else {
                printf("课程不存在.\n");
            }
            break;
        case 4:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                score = queryCourseScore(courseNode, student->id);
                if (score == -1) {
					printf("课程未报名.\n");
				}
				else {
					printf("你的课程分数是 %d.\n", score);
				}
            }
            else {
                printf("课程不存在.\n");
            }
            break;
        case 5:
            printCourses(root);
            break;
        case 6:
            return;
        default:
            printf("无效的选择. 请重新输入.\n");
        }
        system("pause");
    }
}

void teacherMenu(Teacher* teacher, CourseNode* root) {
    int choice;
    char courseId[64];
    char studentId[64];
    int currentScore;
    int newScore;
    CourseNode* courseNode = searchCourseByTeacherId(root, teacher->id);
    while (1) {
        choice = 0;
        system("cls");
        printf("教师编号: %s\n", teacher->id);
        printf("1. 查询我的课程信息\n");
        printf("2. 查询我的学生信息\n");
        printf("3. 输入学生分数\n");
        printf("4. 修改学生分数\n");
        printf("5. 学生分数统计\n");
        printf("6. 退出登录\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        while ((getchar()) != '\n');
        switch (choice) {
        case 1:
            printCourse(courseNode);
            break;
        case 2:
            printCourseStudents(courseNode);
            break;
        case 3:
            printf("现有学生:\n");
            printStudentIds(courseNode->data->courseStudentNode);
            printf("请输入学号: ");
            scanf("%s", studentId);
            while ((getchar()) != '\n');
            currentScore = queryCourseScore(courseNode, studentId);
            if (currentScore == 0) {
                newScore = 0;
                printf("请输入新分数: ");
                scanf("%d", &newScore);
                while ((getchar()) != '\n');
                modifyStudentScore(courseNode, studentId, newScore);
                printCourse(courseNode);
            }
            else if (currentScore == -1) {
				printf("学生不存在.\n");
			}
            else {
                printf("分数已存在. 请选择“修改学生分数”.\n");
            }
            break;
        case 4:
            printf("现有学生:\n");
            printStudentIds(courseNode->data->courseStudentNode);
            printf("请输入学号: ");
            scanf("%s", studentId);
            while ((getchar()) != '\n');
            currentScore = queryCourseScore(courseNode, studentId);
            if (currentScore != 0) {
                printf("请输入新分数: ");
                scanf("%d", &newScore);
                while ((getchar()) != '\n');
                modifyStudentScore(courseNode, studentId, newScore);
                printCourse(courseNode);
            }
            else if (currentScore == -1) {
                printf("学生不存在.\n");
			}
            else {
                printf("分数不存在. 请选择“输入学生分数”.\n");
            }
            break;
        case 5:
            printCourseScoreStatistics(courseNode);
            break;
        case 6:
            return;
        default:
            printf("无效的选择. 请重新输入.\n");
        }
        system("pause");
    }
}

void adminMenu(CourseNode** rootPtr) {
    CourseNode* root = *rootPtr;
    int choice;
    char courseId[64];
    char courseName[64];
    int courseCredit;
    char teacherId[64];
    char teacherPassword[64];
    char courseTime[64];
    char courseLocation[64];
    CourseNode* courseNode;
    while (1) {
        choice = 0;
        root = *rootPtr;
        system("cls");
        printf("1. 创建课程信息\n");
        printf("2. 修改课程信息\n");
        printf("3. 删除课程信息\n");
        printf("4. 查询课程总数\n");
        printf("5. 查询课程中的学生总数\n");
        printf("6. 浏览所有课程信息\n");
        printf("7. 退出登录\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        while ((getchar()) != '\n');
        switch (choice) {
        case 1:
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            printf("请输入课程名: ");
            fgets(courseName, sizeof(courseName), stdin);
            courseName[strcspn(courseName, "\n")] = 0;
            printf("请输入课程学分: ");
            scanf("%d", &courseCredit);
            while ((getchar()) != '\n');
            printf("请输入教师编号: ");
            scanf("%s", teacherId);
            while ((getchar()) != '\n');
            printf("请输入教师密码: ");
            scanf("%s", teacherPassword);
            while ((getchar()) != '\n');
            printf("请输入上课时间: ");
            fgets(courseTime, sizeof(courseTime), stdin);
            courseTime[strcspn(courseTime, "\n")] = 0;
            printf("请输入上课地点: ");
            fgets(courseLocation, sizeof(courseLocation), stdin);
            courseLocation[strcspn(courseLocation, "\n")] = 0;
            Teacher* teacher = createTeacher(teacherId, teacherPassword);
            Course* course = createCourse(courseId, courseName, courseCredit, teacher, courseTime, courseLocation);
            CourseNode* courseNode = createCourseNode(course);
            addCourseNode(rootPtr, courseNode);
            printf("课程创建成功.\n");
            printCourse(courseNode);
            break;
        case 2:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                printCourse(courseNode);
                printf("请输入需修改的课程属性:\n");
                printf("1. 课程编号\n");
                printf("2. 课程名\n");
                printf("3. 课程学分\n");
                printf("4. 教师编号\n");
                printf("5. 教师密码\n");
                printf("6. 上课时间\n");
                printf("7. 上课地点\n");
                printf("请输入选项: ");
                int attrChoice;
                scanf("%d", &attrChoice);
                while ((getchar()) != '\n');
                switch (attrChoice) {
                case 1:
                    printf("请输入新课程编号: ");
                    scanf("%s", courseId);
                    while ((getchar()) != '\n');
                    Teacher* teacher = createTeacher(courseNode->data->teacher->id, courseNode->data->teacher->password);
                    Course* course = createCourse(courseId, courseNode->data->name, courseNode->data->credit, teacher, courseNode->data->time, courseNode->data->location);
                    CourseNode* newCourseNode = createCourseNode(course);
                    CourseStudentNode* student = courseNode->data->courseStudentNode;
                    deleteCourseNode(rootPtr, courseNode->data->id);
                    addCourseNode(rootPtr, newCourseNode);
                    while (student != NULL) {
						addCourseStudentNode(newCourseNode, student->data);
						student = student->next;
					}
                    courseNode = newCourseNode;
                    break;
                case 2:
                    printf("请输入新课程名: ");
                    fgets(courseName, sizeof(courseName), stdin);
                    courseName[strcspn(courseName, "\n")] = 0;
                    strcpy(courseNode->data->name, courseName);
                    break;
                case 3:
                    printf("请输入新课程学分: ");
                    scanf("%d", &courseCredit);
                    while ((getchar()) != '\n');
                    courseNode->data->credit = courseCredit;
                    break;
                case 4:
                    printf("请输入新教师编号: ");
                    scanf("%s", teacherId);
                    while ((getchar()) != '\n');
                    strcpy(courseNode->data->teacher->id, teacherId);
                    break;
                case 5:
                    printf("请输入新教师密码: ");
                    scanf("%s", teacherPassword);
                    while ((getchar()) != '\n');
                    strcpy(courseNode->data->teacher->password, teacherPassword);
                    break;
                case 6:
                    printf("请输入新上课时间: ");
                    fgets(courseTime, sizeof(courseTime), stdin);
                    courseTime[strcspn(courseTime, "\n")] = 0;
                    strcpy(courseNode->data->time, courseTime);
                    break;
                case 7:
                    printf("请输入新上课地点: ");
                    fgets(courseLocation, sizeof(courseLocation), stdin);
                    courseLocation[strcspn(courseLocation, "\n")] = 0;
                    strcpy(courseNode->data->location, courseLocation);
                    break;
                default:
                    printf("无效的选择. 请重新输入.\n");
                }
                printf("课程信息更新成功.\n");
                printCourse(courseNode);
            }
            else {
                printf("课程不存在.\n");
            }
            break;
        case 3:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            deleteCourseNode(rootPtr, courseId);
            printCourses(root);
            break;
        case 4:
            printf("课程总数: %d\n", countCourses(root));
            break;
        case 5:
            printf("现有课程:\n");
            printCourseIds(root);
            printf("请输入课程编号: ");
            scanf("%s", courseId);
            while ((getchar()) != '\n');
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                printf("课程 %s 中的学生总数 %d\n", courseId, countCourseStudents(courseNode));
            }
            else {
                printf("课程不存在.\n");
            }
            break;
        case 6:
            printCourses(root);
            break;
        case 7:
            return;
        default:
            printf("无效的选择. 请重新输入.\n");
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
        choice = 0;
        system("cls");
        printf("1. 学生登录\n");
        printf("2. 教师登录\n");
        printf("3. 管理员登录\n");
        printf("4. 退出\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        while ((getchar()) != '\n');
        switch (choice) {
        case 1:
            student = studentLogin(studentNode);
            if (student != NULL) {
                studentMenu(student, courseNode);
            }
            break;
        case 2:
            teacher = teacherLogin(courseNode);
            if (teacher != NULL) {
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
            printf("无效的选择. 请重新输入.\n");
        }
        system("pause");
    }
}

int main() {
    // Create data
    Teacher* teacher1 = createTeacher("T001", "123456");
    Teacher* teacher2 = createTeacher("T002", "123456");
    Teacher* teacher3 = createTeacher("T003", "123456");
    Teacher* teacher4 = createTeacher("T004", "123456");
    Student* student1 = createStudent("2135061128", "杨同学", "13312345678", "123456");
    Student* student2 = createStudent("2135061018", "张同学", "14912345678", "123456");
    Student* student3 = createStudent("2135061130", "李同学", "15312345678", "123456");
    Student* student4 = createStudent("2135061206", "王同学", "18012345678", "123456");
    Student* student5 = createStudent("2135061224", "刘同学", "18112345678", "123456");
    Course* course1 = createCourse("22000210", "高等数学", 6, teacher1, "星期一 10:00", "一教 101");
    Course* course2 = createCourse("22000622", "线性代数", 3, teacher2, "星期二 14:00", "一教 102");
    Course* course3 = createCourse("22000172", "概率与统计", 3, teacher3, "星期三 10:00", "一教 103");
    Course* course4 = createCourse("12002920", "数据结构", 3, teacher4, "星期四 14:00", "五教 204");

    // Create nodes
    StudentNode* studentNode1 = createStudentNode(student1);
    StudentNode* studentNode2 = createStudentNode(student2);
    StudentNode* studentNode3 = createStudentNode(student3);
    StudentNode* studentNode4 = createStudentNode(student4);
    StudentNode* studentNode5 = createStudentNode(student5);
    CourseNode* courseNode1 = createCourseNode(course1);
    CourseNode* courseNode2 = createCourseNode(course2);
    CourseNode* courseNode3 = createCourseNode(course3);
    CourseNode* courseNode4 = createCourseNode(course4);

    // Build the course tree
    CourseNode* root = NULL;
    addCourseNode(&root, courseNode1);
    addCourseNode(&root, courseNode2);
    addCourseNode(&root, courseNode3);
    addCourseNode(&root, courseNode4);

    // Build the student linked list
    StudentNode* studentNode = NULL;
    addStudentNode(&studentNode, studentNode1);
    addStudentNode(&studentNode, studentNode2);
    addStudentNode(&studentNode, studentNode3);
    addStudentNode(&studentNode, studentNode4);
    addStudentNode(&studentNode, studentNode5);

    // Add students to courses
    addCourseStudentNode(courseNode1, student1);
    addCourseStudentNode(courseNode1, student2);
    addCourseStudentNode(courseNode2, student1);
    addCourseStudentNode(courseNode2, student3);
    addCourseStudentNode(courseNode3, student4);
    addCourseStudentNode(courseNode3, student5);
    addCourseStudentNode(courseNode4, student2);
    addCourseStudentNode(courseNode4, student4);

    // Print data
    printCourses(root);
    system("pause");

    // Login menu
    loginMenu(root, studentNode);
    return 0;
}