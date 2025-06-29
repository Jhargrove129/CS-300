#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>     // For toupper, isalpha
#include <cstdlib>    // For system()
#include <thread>     // For sleep_for
#include <chrono>     // For chrono literals

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000; // default time for sleep in milliseconds

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* right;
        Node* left;

        Node() : right(nullptr), left(nullptr) {}
        Node(const Course& aCourse) : course(aCourse), right(nullptr), left(nullptr) {}
    };

    Node* root;
    int size = 0;

    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        cout << node->course.courseId << ", " << node->course.courseName << endl;
        inOrder(node->right);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void InOrder() { inOrder(root); }

    void Insert(const Course& aCourse) {
        if (!root) {
            root = new Node(aCourse);
        } else {
            Node* currentNode = root;
            while (currentNode) {
                if (aCourse.courseId < currentNode->course.courseId) {
                    if (!currentNode->left) {
                        currentNode->left = new Node(aCourse);
                        break;
                    }
                    currentNode = currentNode->left;
                } else {
                    if (!currentNode->right) {
                        currentNode->right = new Node(aCourse);
                        break;
                    }
                    currentNode = currentNode->right;
                }
            }
        }
        ++size;
    }

    void Remove(const string& courseId) {
        Node* parent = nullptr;
        Node* current = root;

        while (current) {
            if (current->course.courseId == courseId) {
                // Node to delete found
                if (!current->left && !current->right) {
                    if (!parent) root = nullptr;
                    else if (parent->left == current) parent->left = nullptr;
                    else parent->right = nullptr;
                } else if (!current->right) {
                    if (!parent) root = current->left;
                    else if (parent->left == current) parent->left = current->left;
                    else parent->right = current->left;
                } else if (!current->left) {
                    if (!parent) root = current->right;
                    else if (parent->left == current) parent->left = current->right;
                    else parent->right = current->right;
                } else {
                    Node* successor = current->right;
                    while (successor->left) successor = successor->left;
                    Course succCourse = successor->course;
                    Remove(successor->course.courseId);
                    current->course = succCourse;
                }
                return;
            } else if (courseId < current->course.courseId) {
                parent = current;
                current = current->left;
            } else {
                parent = current;
                current = current->right;
            }
        }
        cout << "\nValue not found" << endl;
    }

    Course Search(const string& courseId) {
        Node* current = root;
        while (current) {
            if (current->course.courseId == courseId) {
                return current->course;
            } else if (courseId < current->course.courseId) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return Course{};
    }

    int Size() { return size; }
};

vector<string> Split(const string& lineFeed) {
    char delim = ',';
    vector<string> lineTokens;
    string temp;
    for (size_t i = 0; i <= lineFeed.length(); ++i) {
        if (i == lineFeed.length() || lineFeed[i] == delim) {
            lineTokens.push_back(temp);
            temp.clear();
        } else {
            temp += lineFeed[i];
        }
    }
    return lineTokens;
}

void loadCourses(const string& csvPath, BinarySearchTree* courseList) {
    ifstream inFS(csvPath);
    string line;

    if (!inFS.is_open()) {
        cout << "Could not open file. Please check inputs. " << endl;
        return;
    }

    while (getline(inFS, line)) {
        Course aCourse;
        vector<string> stringTokens = Split(line);

        if (stringTokens.size() < 2) {
            cout << "\nError. Skipping line." << endl;
        } else {
            aCourse.courseId = stringTokens[0];
            aCourse.courseName = stringTokens[1];
            for (size_t i = 2; i < stringTokens.size(); ++i) {
                aCourse.preList.push_back(stringTokens[i]);
            }
            courseList->Insert(aCourse);
        }
    }
    inFS.close();
}

void displayCourse(const Course& aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";
    if (aCourse.preList.empty()) {
        cout << "none" << endl;
    } else {
        for (size_t i = 0; i < aCourse.preList.size(); ++i) {
            cout << aCourse.preList[i];
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1)
                cout << ", ";
        }
        cout << endl;
    }
}

void convertCase(string& toConvert) {
    for (char& c : toConvert) {
        if (isalpha(static_cast<unsigned char>(c)))
            c = toupper(static_cast<unsigned char>(c));
    }
}

void sleepms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main(int argc, char* argv[]) {
    string csvPath, aCourseKey;
    switch (argc) {
        case 2: csvPath = argv[1]; break;
        case 3: csvPath = argv[1]; aCourseKey = argv[2]; break;
        default: csvPath = "FinalProject_Sample_input.csv";
    }

    BinarySearchTree* courseList = new BinarySearchTree();

    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        aCourseKey.clear();
        string anyKey;
        choice = 0;

        try {
            cin >> choice;
            if ((choice > 0 && choice < 5) || choice == 9)
                goodInput = true;
            else {
                goodInput = false;
                throw 1;
            }

            switch (choice) {
                case 1:
                    loadCourses(csvPath, courseList);
                    cout << courseList->Size() << " courses read" << endl;
                    sleepms(GLOBAL_SLEEP_TIME);
                    break;
                case 2:
                    courseList->InOrder();
                    cout << "\nEnter 'y' to continue..." << endl;
                    cin >> anyKey;
                    break;
                case 3:
                    cout << "\nWhat course do you want to know about? " << endl;
                    cin >> aCourseKey;
                    convertCase(aCourseKey);
                    course = courseList->Search(aCourseKey);
                    if (!course.courseId.empty()) {
                        displayCourse(course);
                    } else {
                        cout << "\nCourse ID " << aCourseKey << " not found." << endl;
                    }
                    sleepms(GLOBAL_SLEEP_TIME);
                    break;
                case 4:
                    cout << "\nWhat course do you want delete? " << endl;
                    cin >> aCourseKey;
                    convertCase(aCourseKey);
                    courseList->Remove(aCourseKey);
                    sleepms(GLOBAL_SLEEP_TIME);
                    break;
                case 9:
                    break;
                default:
                    throw 2;
            }
        }
        catch (int) {
            cout << "\nPlease check your input." << endl;
            sleepms(GLOBAL_SLEEP_TIME);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        system("cls");
    }

    cout << "Good bye." << endl;
    sleepms(GLOBAL_SLEEP_TIME);
    return 0;
}
