#include <iostream>

using namespace std;

#pragma warning(disable : 4996)

typedef struct Student {
    string id;
    string name;
    string phone;
    string password;
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
    string id;
    string password;
} Teacher;

typedef struct Course {
    string id;
    string name;
    int credit;
    Teacher* teacher;
    string time;
    string location;
    CourseStudentNode* courseStudentNode;
} Course;

typedef struct CourseNode {
    Course* data;
    int height;
    struct CourseNode* left;
    struct CourseNode* right;
} CourseNode;

/* Function to create basic data structures */

Student* createStudent(string id, string name, string phone, string password) {
    Student* student = new Student;
    student->id = id;
    student->name = name;
    student->phone = phone;
    student->password = password;
    return student;
}

Teacher* createTeacher(string id, string password) {
    Teacher* teacher = new Teacher;
    teacher->id = id;
    teacher->password = password;
    return teacher;
}

Course* createCourse(string id, string name, int credit, Teacher* teacher, string time, string location) {
    Course* course = new Course;
    course->id = id;
    course->name = name;
    course->credit = credit;
    course->teacher = teacher;
    course->time = time;
    course->location = location;
    course->courseStudentNode = NULL;
    return course;
}

/* Function to create nodes */

StudentNode* createStudentNode(Student* student) {
    StudentNode* node = new StudentNode;
    node->data = student;
    node->next = NULL;
    return node;
}

CourseNode* createCourseNode(Course* course) {
    CourseNode* node = new CourseNode;
    node->data = course;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

CourseStudentNode* createCourseStudentNode(Student* student) {
    CourseStudentNode* node = new CourseStudentNode;
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
    if (node->data->id < root->data->id) {
        addCourseNode(&(root->left), node);
    }
    else if (node->data->id > root->data->id) {
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
        if (currStudent->data->id == student->id) {
            cout << "该课程中已存在此学生." << endl;
            free(courseStudentNode);
            return;
        }
        if (currStudent->next == NULL) {
            currStudent->next = courseStudentNode;
            cout << "课程报名成功." << endl;
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

void deleteCourseNode(CourseNode** rootPtr, string id) {
    CourseNode* root = *rootPtr;
    if (root == NULL) {
        cout << "课程不存在." << endl;
        return;
    }
    if (id < root->data->id) {
        deleteCourseNode(&(root->left), id);
    }
    else if (id > root->data->id) {
        deleteCourseNode(&(root->right), id);
    }
    else {
        if (root->left == NULL) {
            // node with only one child or no child
            *rootPtr = root->right;
            free(root);
            cout << "课程删除成功." << endl;
        }
        else if (root->right == NULL) {
            // node with only one child or no child
            *rootPtr = root->left;
            free(root);
            cout << "课程删除成功." << endl;
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

void delCourseStudentNode(CourseNode* courseNode, string studentId) {
    if (courseNode == NULL) {
        cout << "课程不存在." << endl;
        return;
    }
    CourseStudentNode* prevStudent = NULL;
    CourseStudentNode* currStudent = courseNode->data->courseStudentNode;
    while (currStudent != NULL) {
        if (currStudent->data->id == studentId) {
            if (prevStudent == NULL) {
                courseNode->data->courseStudentNode = currStudent->next;
            }
            else {
                prevStudent->next = currStudent->next;
            }
            free(currStudent);
            cout << "课程取消报名成功." << endl;
            return;
        }
        prevStudent = currStudent;
        currStudent = currStudent->next;
    }
    cout << "该课程中不存在此学生." << endl;
}

/* Function to modify nodes */

void modifyStudentScore(CourseNode* courseNode, string studentId, int newScore) {
    if (courseNode == NULL) {
        cout << "课程不存在." << endl;
        return;
    }
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        if (student->data->id == studentId) {
            student->score = newScore;
            cout << "学生 " << studentId << " 在课程 " << courseNode->data->id << " 中的分数已被修改为 " << newScore << "." << endl;
            return;
        }
        student = student->next;
    }
    cout << "该课程中不存在此学生." << endl;
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

int queryCourseScore(CourseNode* courseNode, string studentId) {
    if (courseNode == NULL) {
        return -1;
    }
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        if (student->data->id == studentId) {
            return student->score;
        }
        student = student->next;
    }
    return -1;
}

CourseNode* searchCourse(CourseNode* root, string id) {
    if (root == NULL) {
        return NULL;
    }
    if (id < root->data->id) {
        return searchCourse(root->left, id);
    }
    else if (id > root->data->id) {
        return searchCourse(root->right, id);
    }
    else {
        return root;
    }
}

CourseNode* searchCourseByTeacherId(CourseNode* root, string teacherId) {
    if (root == NULL) {
        return NULL;
    }
    if (root->data->teacher->id == teacherId) {
        return root;
    }
    CourseNode* courseNode = searchCourseByTeacherId(root->left, teacherId);
    if (courseNode != NULL) {
        return courseNode;
    }
    return searchCourseByTeacherId(root->right, teacherId);
}

Teacher* searchTeacherInCourse(CourseNode* root, string id, string password) {
    if (root == NULL) {
        return NULL;
    }
    if (root->data->teacher->id == id && root->data->teacher->password == password) {
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
        cout << "课程不存在." << endl;
        return;
    }
    cout << "------------------------------------------" << endl;
    cout << "课程编号: " << courseNode->data->id << endl;
    cout << "课程名: " << courseNode->data->name << endl;
    cout << "课程学分: " << courseNode->data->credit << endl;
    cout << "课程教师: " << courseNode->data->teacher->id << endl;
    cout << "课程时间: " << courseNode->data->time << endl;
    cout << "课程地点: " << courseNode->data->location << endl;
    cout << "学生:" << endl;
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        cout << "\t学号: " << student->data->id << ", 分数: " << student->score << endl;
        student = student->next;
    }
    cout << "------------------------------------------" << endl;
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
        cout << "课程不存在." << endl;
        return;
    }
    cout << "课程" << courseNode->data->id << "中的学生:" << endl;
    CourseStudentNode* student = courseNode->data->courseStudentNode;
    while (student != NULL) {
        cout << "------------------------------------------" << endl;
        cout << "学号: " << student->data->id << endl;
        cout << "学生名: " << student->data->name << endl;
        cout << "学生手机号: " << student->data->phone << endl;
        cout << "学生分数: " << student->score << endl;
        student = student->next;
    }
}

void printCourseIds(CourseNode* root) {
    if (root == NULL) {
        return;
    }
    printCourseIds(root->left);
    cout << "课程编号: " << root->data->id << endl;
    printCourseIds(root->right);
}

void printStudentIds(CourseStudentNode* head) {
    CourseStudentNode* curr = head;
    while (curr != NULL) {
        cout << "学号: " << curr->data->id << endl;
        curr = curr->next;
    }
}

void printCourseScoreStatistics(CourseNode* courseNode) {
    if (courseNode == NULL) {
        cout << "课程不存在." << endl;
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
    cout << "课程分数统计 " << courseNode->data->id << ":" << endl;
    cout << "90 - 100: " << count90 << endl;
    cout << "80 - 89: " << count80 << endl;
    cout << "60 - 79: " << count60 << endl;
    cout << "0 - 60: " << countBelow60 << endl;
}

/* Function to login */

Student* studentLogin(StudentNode* studentNode) {
    string id;
    string password;
    cout << "请输入学号: ";
    getline(cin, id);
    cout << "请输入密码: ";
    getline(cin, password);
    while (studentNode != NULL) {
        if (studentNode->data->id == id && studentNode->data->password == password) {
            cout << "登录成功." << endl;
            return studentNode->data;
        }
        studentNode = studentNode->next;
    }
    cout << "登录失败." << endl;
    return NULL;
}

Teacher* teacherLogin(CourseNode* root) {
    string id;
    string password;
    cout << "请输入教师编号: ";
    getline(cin, id);
    cout << "请输入密码: ";
    getline(cin, password);
    Teacher* teacher = searchTeacherInCourse(root, id, password);
    if (teacher != NULL) {
        cout << "登录成功." << endl;
        return teacher;
    }
    cout << "登录失败." << endl;
    return NULL;
}

int adminLogin() {
    string username;
    string password;
    cout << "请输入用户名: ";
    getline(cin, username);
            cout << "请输入密码: ";
    getline(cin, password);
    if (username == "admin" && password == "admin") {
        cout << "登录成功." << endl;
        return 1;
    }
    cout << "登录失败." << endl;
    return 0;
}

/* Function to display menus */

void studentMenu(Student* student, CourseNode* root) {
    int choice;
    string courseId;
    CourseNode* courseNode;
    int score;
    while (true) {
        choice = 0;
        system("cls");
        cout << "学号: " << student->id << endl;
        cout << "1. 查询课程信息" << endl;
        cout << "2. 报名课程学习" << endl;
        cout << "3. 取消报名课程" << endl;
        cout << "4. 查询课程分数" << endl;
        cout << "5. 浏览所有课程信息" << endl;
        cout << "6. 退出登录" << endl;
        cout << "请输入选项: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            cin >> courseId;
            cin.ignore();
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                printCourse(courseNode);
            }
            else {
                cout << "课程不存在." << endl;
            }
            break;
        case 2:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            cin >> courseId;
            cin.ignore();
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                addCourseStudentNode(courseNode, student);
                printCourse(courseNode);
            }
            else {
                cout << "课程不存在." << endl;
            }
            break;
        case 3:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            cin >> courseId;
            cin.ignore();
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                delCourseStudentNode(courseNode, student->id);
                printCourse(courseNode);
            }
            else {
                cout << "课程不存在." << endl;
            }
            break;
        case 4:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            cin >> courseId;
            cin.ignore();
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                score = queryCourseScore(courseNode, student->id);
                if (score == -1) {
                    cout << "课程未报名." << endl;
				}
				else {
                    cout << "你的课程分数是 " << score << endl;
				}
            }
            else {
                cout << "课程不存在." << endl;
            }
            break;
        case 5:
            printCourses(root);
            break;
        case 6:
            return;
        default:
            cout << "无效的选择. 请重新输入." << endl;

        }
        system("pause");
    }
}

void teacherMenu(Teacher* teacher, CourseNode* root) {
    int choice;
    string courseId;
    string studentId;
    int currentScore;
    int newScore;
    CourseNode* courseNode = searchCourseByTeacherId(root, teacher->id);
    while (true) {
        choice = 0;
        system("cls");
        cout << "教师编号: " << teacher->id << endl;
        cout << "1. 查询我的课程信息" << endl;
        cout << "2. 查询我的学生信息" << endl;
        cout << "3. 输入学生分数" << endl;
        cout << "4. 修改学生分数" << endl;
        cout << "5. 学生分数统计" << endl;
        cout << "6. 退出登录" << endl;
        cout << "请输入选项: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1:
            printCourse(courseNode);
            break;
        case 2:
            printCourseStudents(courseNode);
            break;
        case 3:
            cout << "现有学生:" << endl;
            printStudentIds(courseNode->data->courseStudentNode);
            cout << "请输入学号: ";
            getline(cin, studentId);
            currentScore = queryCourseScore(courseNode, studentId);
            if (currentScore == 0) {
                newScore = 0;
                cout << "请输入新分数: ";
                cin >> newScore;
                cin.ignore();
                modifyStudentScore(courseNode, studentId, newScore);
                printCourse(courseNode);
            }
            else if (currentScore == -1) {
				cout << "学生不存在." << endl;
			}
            else {
                cout << "分数已存在. 请选择\"修改学生分数\"." << endl;
            }
            break;
        case 4:
            cout << "现有学生:" << endl;
            printStudentIds(courseNode->data->courseStudentNode);
            cout << "请输入学号: ";
            getline(cin, studentId);
            currentScore = queryCourseScore(courseNode, studentId);
            if (currentScore != 0) {
                cout << "请输入新分数: ";
                cin >> newScore;
                cin.ignore();
                modifyStudentScore(courseNode, studentId, newScore);
                printCourse(courseNode);
            }
            else if (currentScore == -1) {
                cout << "学生不存在." << endl;
			}
            else {
                cout << "分数不存在. 请选择\"输入学生分数\"." << endl;
            }
            break;
        case 5:
            printCourseScoreStatistics(courseNode);
            break;
        case 6:
            return;
        default:
            cout << "无效的选择. 请重新输入." << endl;
        }
        system("pause");
    }
}

void adminMenu(CourseNode** rootPtr) {
    CourseNode* root = *rootPtr;
    int choice;
    string courseId;
    string courseName;
    int courseCredit;
    string teacherId;
    string teacherPassword;
    string courseTime;
    string courseLocation;
    CourseNode* courseNode;
    while (true) {
        choice = 0;
        root = *rootPtr;
        system("cls");
        cout << "1. 创建课程信息" << endl;
        cout << "2. 修改课程信息" << endl;
        cout << "3. 删除课程信息" << endl;
        cout << "4. 查询课程总数" << endl;
        cout << "5. 查询课程中的学生总数" << endl;
        cout << "6. 浏览所有课程信息" << endl;
        cout << "7. 退出登录" << endl;
        cout << "请输入选项: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1: {
            cout << "请输入课程编号: ";
            getline(cin, courseId);
            cout << "请输入课程名: ";
            getline(cin, courseName);
            cout << "请输入课程学分: ";
            cin >> courseCredit;
            cin.ignore();
            cout << "请输入教师编号: ";
            getline(cin, teacherId);
            cout << "请输入教师密码: ";
            getline(cin, teacherPassword);
            cout << "请输入上课时间: ";
            getline(cin, courseTime);
            cout << "请输入上课地点: ";
            getline(cin, courseLocation);
            Teacher *teacher = createTeacher(teacherId, teacherPassword);
            Course *course = createCourse(courseId, courseName, courseCredit, teacher, courseTime, courseLocation);
            CourseNode *courseNode = createCourseNode(course);
            addCourseNode(rootPtr, courseNode);
            cout << "课程创建成功." << endl;
            printCourse(courseNode);
            break;
        }
        case 2:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            getline(cin, courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                int attrChoice;
                printCourse(courseNode);
                cout << "请输入需修改的课程属性:" << endl;
                cout << "1. 课程编号" << endl;
                cout << "2. 课程名" << endl;
                cout << "3. 课程学分" << endl;
                cout << "4. 教师编号" << endl;
                cout << "5. 教师密码" << endl;
                cout << "6. 上课时间" << endl;
                cout << "7. 上课地点" << endl;
                cout << "请输入选项: ";
                cin >> attrChoice;
                cin.ignore();
                switch (attrChoice) {
                case 1: {
                    cout << "请输入新课程编号: ";
                    getline(cin, courseId);
                    Teacher *teacher = createTeacher(courseNode->data->teacher->id, courseNode->data->teacher->password);
                    Course *course = createCourse(courseId, courseNode->data->name, courseNode->data->credit, teacher, courseNode->data->time, courseNode->data->location);
                    CourseNode *newCourseNode = createCourseNode(course);
                    CourseStudentNode *student = courseNode->data->courseStudentNode;
                    deleteCourseNode(rootPtr, courseNode->data->id);
                    addCourseNode(rootPtr, newCourseNode);
                    while (student != NULL) {
                        addCourseStudentNode(newCourseNode, student->data);
                        student = student->next;
                    }
                    courseNode = newCourseNode;
                    break;
                }
                case 2:
                    cout << "请输入新课程名: ";
                    getline(cin, courseName);
                    courseNode->data->name = courseName;
                    break;
                case 3:
                    cout << "请输入新课程学分: ";
                    cin >> courseCredit;
                    cin.ignore();
                    courseNode->data->credit = courseCredit;
                    break;
                case 4:
                    cout << "请输入新教师编号: ";
                    getline(cin, teacherId);
                    courseNode->data->teacher->id = teacherId;
                    break;
                case 5:
                    cout << "请输入新教师密码: ";
                    getline(cin, teacherPassword);
                    courseNode->data->teacher->password = teacherPassword;
                    break;
                case 6:
                    cout << "请输入新上课时间: ";
                    getline(cin, courseTime);
                    courseNode->data->time = courseTime;
                    break;
                case 7:
                    cout << "请输入新上课地点: ";
                    getline(cin, courseLocation);
                    courseNode->data->location = courseLocation;
                    break;
                default:
                    cout << "无效的选择. 请重新输入." << endl;
                }
                cout << "课程信息更新成功." << endl;
                printCourse(courseNode);
            }
            else {
                cout << "课程不存在." << endl;
            }
            break;
        case 3:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            getline(cin, courseId);
            deleteCourseNode(rootPtr, courseId);
            printCourses(root);
            break;
        case 4:
            cout << "课程总数: " << countCourses(root) << endl;
            break;
        case 5:
            cout << "现有课程:" << endl;
            printCourseIds(root);
            cout << "请输入课程编号: ";
            getline(cin, courseId);
            courseNode = searchCourse(root, courseId);
            if (courseNode != NULL) {
                cout << "课程 " << courseId << " 中的学生总数 " << countCourseStudents(courseNode) << endl;
            }
            else {
                cout << "课程不存在." << endl;
            }
            break;
        case 6:
            printCourses(root);
            break;
        case 7:
            return;
        default:
            cout << "无效的选择. 请重新输入." << endl;
        }
        system("pause");
    }
}

void loginMenu(CourseNode* courseNode, StudentNode* studentNode) {
    int choice;
    Student* student;
    Teacher* teacher;
    int admin;
    while (true) {
        choice = 0;
        system("cls");
        cout << "1. 学生登录" << endl;
        cout << "2. 教师登录" << endl;
        cout << "3. 管理员登录" << endl;
        cout << "4. 退出" << endl;
        cout << "请输入选项: ";
        cin >> choice;
        cin.ignore();
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
            cout << "无效的选择. 请重新输入." << endl;
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
