#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct StringNode {
    string value;
    StringNode* next;
    StringNode(const string& v) : value(v), next(NULL) {}
};

struct ConnectionNode {
    int studentID;
    ConnectionNode* next;
    ConnectionNode(int id) : studentID(id), next(NULL) {}
};

class Student {
public:
    int userID;
    string name, branch;
    int year;
    StringNode* skills;
    StringNode* interests;
    ConnectionNode* connections;

    Student(int id, const string& n, const string& b, int y)
        : userID(id), name(n), branch(b), year(y),
          skills(NULL), interests(NULL), connections(NULL) {}

    void addSkill(const string& value) {
        StringNode* p = skills;
        while (p != NULL) {
            if (p->value == value) return;
            p = p->next;
        }
        StringNode* node = new StringNode(value);
        node->next = skills;
        skills = node;
    }

    void addInterest(const string& value) {
        StringNode* p = interests;
        while (p != NULL) {
            if (p->value == value) return;
            p = p->next;
        }
        StringNode* node = new StringNode(value);
        node->next = interests;
        interests = node;
    }

    bool hasSkill(const string& value) const {
        StringNode* p = skills;
        while (p != NULL) {
            if (p->value == value) return true;
            p = p->next;
        }
        return false;
    }

    bool hasConnection(int id) const {
        ConnectionNode* p = connections;
        while (p != NULL) {
            if (p->studentID == id) return true;
            p = p->next;
        }
        return false;
    }

    void addConnection(int id) {
        if (hasConnection(id)) return;
        ConnectionNode* node = new ConnectionNode(id);
        node->next = connections;
        connections = node;
    }

    void displayProfile() const {
        cout << "\nID: " << userID << "\nName: " << name
             << "\nBranch: " << branch << "\nYear: " << year;
        cout << "\nSkills: ";
        if (skills == NULL) cout << "None";
        for (StringNode* p = skills; p != NULL; p = p->next)
            cout << p->value << (p->next ? ", " : "");
        cout << "\nInterests: ";
        if (interests == NULL) cout << "None";
        for (StringNode* p = interests; p != NULL; p = p->next)
            cout << p->value << (p->next ? ", " : "");
        cout << "\n";
    }

    void viewConnections() const {
        if (connections == NULL) {
            cout << "No accepted connections.\n";
            return;
        }
        cout << "Connected student IDs: ";
        for (ConnectionNode* p = connections; p != NULL; p = p->next)
            cout << p->studentID << (p->next ? " -> " : "");
        cout << "\n";
    }

    ~Student() {
        while (skills != NULL) {
            StringNode* t = skills;
            skills = skills->next;
            delete t;
        }
        while (interests != NULL) {
            StringNode* t = interests;
            interests = interests->next;
            delete t;
        }
        while (connections != NULL) {
            ConnectionNode* t = connections;
            connections = connections->next;
            delete t;
        }
    }
};

struct StudentNode {
    Student* student;
    StudentNode* next;
    StudentNode(Student* s) : student(s), next(NULL) {}
};

class ConnectionRequest {
public:
    int senderID, receiverID;
    ConnectionRequest* next;
    ConnectionRequest(int sender, int receiver)
        : senderID(sender), receiverID(receiver), next(NULL) {}
};

class CampusConnect {
private:
    StudentNode* students;
    ConnectionRequest* requestFront;
    ConnectionRequest* requestRear;
    int studentCount;

    StudentNode* merge(StudentNode* a, StudentNode* b) {
        if (a == NULL) return b;
        if (b == NULL) return a;
        StudentNode* head;
        if (a->student->name <= b->student->name) {
            head = a;
            head->next = merge(a->next, b);
        } else {
            head = b;
            head->next = merge(a, b->next);
        }
        return head;
    }

    void split(StudentNode* source, StudentNode** front, StudentNode** back) {
        StudentNode* slow = source;
        StudentNode* fast = source->next;
        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        *front = source;
        *back = slow->next;
        slow->next = NULL;
    }

    void mergeSort(StudentNode** head) {
        if (*head == NULL || (*head)->next == NULL) return;
        StudentNode* a;
        StudentNode* b;
        split(*head, &a, &b);
        mergeSort(&a);
        mergeSort(&b);
        *head = merge(a, b);
    }

    bool queueHasRequest(int sender, int receiver) const {
        for (ConnectionRequest* p = requestFront; p != NULL; p = p->next)
            if ((p->senderID == sender && p->receiverID == receiver) ||
                (p->senderID == receiver && p->receiverID == sender))
                return true;
        return false;
    }

    void enqueueRequest(int sender, int receiver) {
        ConnectionRequest* node = new ConnectionRequest(sender, receiver);
        if (requestRear == NULL) requestFront = requestRear = node;
        else {
            requestRear->next = node;
            requestRear = node;
        }
    }

    ConnectionRequest* dequeueRequest() {
        if (requestFront == NULL) return NULL;
        ConnectionRequest* node = requestFront;
        requestFront = requestFront->next;
        if (requestFront == NULL) requestRear = NULL;
        node->next = NULL;
        return node;
    }

public:
    CampusConnect() : students(NULL), requestFront(NULL), requestRear(NULL),
                      studentCount(0) {}

    Student* findStudentByID(int id) const {
        for (StudentNode* p = students; p != NULL; p = p->next)
            if (p->student->userID == id) return p->student;
        return NULL;
    }

    void registerStudent() {
        int id, year;
        string name, branch;
        cout << "Student ID: ";
        if (!(cin >> id)) { invalidInput(); return; }
        if (findStudentByID(id) != NULL) {
            cout << "This ID already exists.\n";
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Name: "; getline(cin, name);
        cout << "Branch: "; getline(cin, branch);
        cout << "Year: ";
        if (!(cin >> year)) { invalidInput(); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Student* s = new Student(id, name, branch, year);
        StudentNode* node = new StudentNode(s);
        node->next = students;
        students = node;
        studentCount++;
        cout << "Student registered. Add skills and interests from the menu.\n";
    }

    void addSkillOrInterest(bool addSkill) {
        int id;
        string value;
        cout << "Student ID: ";
        if (!(cin >> id)) { invalidInput(); return; }
        Student* s = findStudentByID(id);
        if (s == NULL) { cout << "Student not found.\n"; return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << (addSkill ? "Enter skill: " : "Enter interest: ");
        getline(cin, value);
        if (addSkill) s->addSkill(value);
        else s->addInterest(value);
        cout << "Updated successfully.\n";
    }

    void displayAllStudents() const {
        if (students == NULL) { cout << "No students registered.\n"; return; }
        for (StudentNode* p = students; p != NULL; p = p->next)
            p->student->displayProfile();
    }

    void searchStudent() const {
        int choice;
        cout << "1. Search by ID\n2. Search by branch\n3. Search by skill\nChoice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input.\n"; return; }
        if (choice == 1) {
            int id;
            cout << "Enter ID: ";
            if (!(cin >> id)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid ID.\n"; return; }
            Student* s = findStudentByID(id);
            if (s) s->displayProfile(); else cout << "Student not found.\n";
            return;
        }
        if (choice != 2 && choice != 3) { cout << "Invalid choice.\n"; return; }
        string key;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << (choice == 2 ? "Enter branch: " : "Enter skill: ");
        getline(cin, key);
        bool found = false;
        for (StudentNode* p = students; p != NULL; p = p->next) {
            bool match = (choice == 2 && p->student->branch == key) ||
                         (choice == 3 && p->student->hasSkill(key));
            if (match) { p->student->displayProfile(); found = true; }
        }
        if (!found) cout << "No matching students found.\n";
    }

    void sortStudents() {
        mergeSort(&students);
        cout << "Students sorted by name using Merge Sort.\n";
        displayAllStudents();
    }

    void sendRequest() {
        int sender, receiver;
        cout << "Your student ID: ";
        if (!(cin >> sender)) { invalidInput(); return; }
        cout << "Receiver student ID: ";
        if (!(cin >> receiver)) { invalidInput(); return; }
        Student* from = findStudentByID(sender);
        Student* to = findStudentByID(receiver);
        if (from == NULL || to == NULL) { cout << "Student ID not found.\n"; return; }
        if (sender == receiver) { cout << "You cannot connect with yourself.\n"; return; }
        if (from->hasConnection(receiver)) { cout << "Already connected.\n"; return; }
        if (queueHasRequest(sender, receiver)) { cout << "A request already exists between these students.\n"; return; }
        enqueueRequest(sender, receiver);
        cout << "Request added to the pending-request queue.\n";
    }

    void viewPendingRequests() const {
        if (requestFront == NULL) { cout << "No pending requests.\n"; return; }
        cout << "Pending requests (front to rear):\n";
        for (ConnectionRequest* p = requestFront; p != NULL; p = p->next)
            cout << p->senderID << " -> " << p->receiverID << "\n";
    }

    void processNextRequest() {
        if (requestFront == NULL) { cout << "No pending requests.\n"; return; }
        ConnectionRequest* req = dequeueRequest();
        cout << "Request from student " << req->senderID
             << " to student " << req->receiverID << "\n";
        cout << "1. Accept\n2. Reject\nChoice: ";
        int choice;
        if (!(cin >> choice)) { invalidInput(); delete req; return; }
        if (choice == 1) {
            Student* sender = findStudentByID(req->senderID);
            Student* receiver = findStudentByID(req->receiverID);
            if (sender != NULL && receiver != NULL) {
                sender->addConnection(receiver->userID);
                receiver->addConnection(sender->userID);
                cout << "Request accepted. Both students are connected.\n";
            }
        } else if (choice == 2) {
            cout << "Request rejected.\n";
        } else {
            cout << "Invalid choice. Request removed from queue.\n";
        }
        delete req;
    }

    void viewConnections() const {
        int id;
        cout << "Student ID: ";
        if (!(cin >> id)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid ID.\n"; return; }
        Student* s = findStudentByID(id);
        if (s == NULL) { cout << "Student not found.\n"; return; }
        s->viewConnections();
    }

    static void invalidInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter the correct type of value.\n";
    }

    ~CampusConnect() {
        while (requestFront != NULL) {
            ConnectionRequest* t = requestFront;
            requestFront = requestFront->next;
            delete t;
        }
        while (students != NULL) {
            StudentNode* t = students;
            students = students->next;
            delete t->student;
            delete t;
        }
    }
};

int main() {
    CampusConnect app;
    int choice;
    do {
        cout << "\n========== CAMPUSCONNECT ==========\n";
        cout << "1. Register student\n";
        cout << "2. Add skill\n";
        cout << "3. Add interest\n";
        cout << "4. Display all student profiles\n";
        cout << "5. Search students\n";
        cout << "6. Sort students by name (Merge Sort)\n";
        cout << "7. Send connection request\n";
        cout << "8. View pending requests (Queue)\n";
        cout << "9. Process next request\n";
        cout << "10. View accepted connections\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            CampusConnect::invalidInput();
            continue;
        }
        switch (choice) {
            case 1: app.registerStudent(); break;
            case 2: app.addSkillOrInterest(true); break;
            case 3: app.addSkillOrInterest(false); break;
            case 4: app.displayAllStudents(); break;
            case 5: app.searchStudent(); break;
            case 6: app.sortStudents(); break;
            case 7: app.sendRequest(); break;
            case 8: app.viewPendingRequests(); break;
            case 9: app.processNextRequest(); break;
            case 10: app.viewConnections(); break;
            case 0: cout << "Exiting CampusConnect.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
