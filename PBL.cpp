// Campus Connect - Student Networking System (Team Brainstormers)
//
// Data structures:
//   Student records  -> Linked list (always kept sorted by ID)
//   Sorting          -> Merge Sort
//   Search by ID     -> Binary Search
//   Pending requests -> Queue
//   Accepted list    -> Linked list
//
// OOP concepts demonstrated:
//   Encapsulation  -> all data members are private, accessed via getters/setters
//   Abstraction    -> list/queue classes hide how they work behind simple functions
//   Constructors / Destructors -> every class has constructors, lists free memory

#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

// =====================================================
//  Student - also acts as a linked list node
// =====================================================
class Student {
private:
    string studentId;
    string name;
    string branch;
    string skills;
    int year;
    Student* next;          // link to the next student in the list

public:
    Student() {
        studentId = "";
        name = "";
        branch = "";
        skills = "";
        year = 0;
        next = NULL;
    }

    Student(string id, string studentName, string studentBranch,
            int studentYear, string studentSkills) {
        studentId = id;
        name = studentName;
        branch = studentBranch;
        year = studentYear;
        skills = studentSkills;
        next = NULL;
    }

    // getters (const: they only read, never modify the object)
    const string& getId() const     { return studentId; }
    const string& getName() const   { return name; }
    const string& getBranch() const { return branch; }
    const string& getSkills() const { return skills; }
    int getYear() const             { return year; }
    Student* getNext() const        { return next; }

    // setter (only the one the program actually uses)
    void setNext(Student* nextStudent) { next = nextStudent; }

    void display() const {
        cout << "ID: " << studentId
             << " | Name: " << name
             << " | Branch: " << branch
             << " | Year: " << year
             << " | Skills: " << skills << endl;
    }
};

// =====================================================
//  Merge Sort (works on a vector of student pointers)
// =====================================================
const string& getSortKey(Student* student, bool sortByName) {
    if (sortByName)
        return student->getName();
    return student->getId();
}

// merge two sorted halves: [low..mid] and [mid+1..high]
void mergeRanges(vector<Student*>& students, int low, int mid, int high, bool sortByName) {
    vector<Student*> temp;
    int left = low;
    int right = mid + 1;

    // "<=" keeps the sort stable (equal keys keep their original order)
    while (left <= mid && right <= high) {
        if (getSortKey(students[left], sortByName) <= getSortKey(students[right], sortByName))
            temp.push_back(students[left++]);
        else
            temp.push_back(students[right++]);
    }
    while (left <= mid)
        temp.push_back(students[left++]);
    while (right <= high)
        temp.push_back(students[right++]);

    for (int k = 0; k < (int)temp.size(); k++)
        students[low + k] = temp[k];
}

void mergeSort(vector<Student*>& students, int low, int high, bool sortByName) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(students, low, mid, sortByName);
        mergeSort(students, mid + 1, high, sortByName);
        mergeRanges(students, low, mid, high, sortByName);
    }
}

// =====================================================
//  StudentList - linked list that stores all students
//  The list is always kept sorted by ID.
// =====================================================
class StudentList {
private:
    Student* head;

public:
    StudentList() {
        head = NULL;
    }

    // destructor frees every node
    ~StudentList() {
        while (head != NULL) {
            Student* nodeToDelete = head;
            head = head->getNext();
            delete nodeToDelete;
        }
    }

    bool isEmpty() const {
        return head == NULL;
    }

    // insert a new student at the correct position so the list stays sorted by ID
    void addStudent(Student* newStudent) {
        if (head == NULL || newStudent->getId() < head->getId()) {
            newStudent->setNext(head);
            head = newStudent;
            return;
        }
        Student* current = head;
        while (current->getNext() != NULL &&
               current->getNext()->getId() < newStudent->getId()) {
            current = current->getNext();
        }
        newStudent->setNext(current->getNext());
        current->setNext(newStudent);
    }

    // copy the list into a vector (already in ID order)
    vector<Student*> toVector() const {
        vector<Student*> studentArray;
        Student* current = head;
        while (current != NULL) {
            studentArray.push_back(current);
            current = current->getNext();
        }
        return studentArray;
    }

    // Binary Search by student ID (returns NULL if not found)
    // No sorting needed here because the list is already sorted.
    Student* findStudent(const string& id) const {
        vector<Student*> sortedStudents = toVector();

        int low = 0;
        int high = (int)sortedStudents.size() - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (sortedStudents[mid]->getId() == id)
                return sortedStudents[mid];
            else if (sortedStudents[mid]->getId() < id)
                low = mid + 1;
            else
                high = mid - 1;
        }
        return NULL;
    }

    void displayAll() const {
        Student* current = head;
        while (current != NULL) {
            current->display();
            current = current->getNext();
        }
    }
};

// =====================================================
//  ConnectionRequest - one request stored in the queue
// =====================================================
class ConnectionRequest {
private:
    string senderId;
    string receiverId;
    string status;

public:
    ConnectionRequest() {
        senderId = "";
        receiverId = "";
        status = "Pending";
    }

    ConnectionRequest(string fromId, string toId) {
        senderId = fromId;
        receiverId = toId;
        status = "Pending";
    }

    const string& getSenderId() const   { return senderId; }
    const string& getReceiverId() const { return receiverId; }
    const string& getStatus() const     { return status; }
    void setStatus(string newStatus)    { status = newStatus; }
};

// =====================================================
//  RequestQueue - queue of pending requests
// =====================================================
class RequestQueue {
private:
    queue<ConnectionRequest> pendingRequests;

public:
    bool isEmpty() const {
        return pendingRequests.empty();
    }

    void addRequest(ConnectionRequest request) {
        pendingRequests.push(request);
    }

    ConnectionRequest getFrontRequest() const {
        return pendingRequests.front();
    }

    void removeFrontRequest() {
        pendingRequests.pop();
    }

    // true if a request between the two students is already waiting
    bool isPending(const string& idOne, const string& idTwo) const {
        queue<ConnectionRequest> tempQueue = pendingRequests;
        while (!tempQueue.empty()) {
            ConnectionRequest request = tempQueue.front();
            tempQueue.pop();
            if ((request.getSenderId() == idOne && request.getReceiverId() == idTwo) ||
                (request.getSenderId() == idTwo && request.getReceiverId() == idOne))
                return true;
        }
        return false;
    }

    void displayAll(const StudentList& students) const {
        queue<ConnectionRequest> tempQueue = pendingRequests;
        int requestNumber = 1;
        while (!tempQueue.empty()) {
            ConnectionRequest request = tempQueue.front();
            tempQueue.pop();
            Student* sender = students.findStudent(request.getSenderId());
            Student* receiver = students.findStudent(request.getReceiverId());
            cout << requestNumber << ". "
                 << sender->getName() << " (" << request.getSenderId() << ") -> "
                 << receiver->getName() << " (" << request.getReceiverId() << ")  ["
                 << request.getStatus() << "]" << endl;
            requestNumber++;
        }
    }
};

// =====================================================
//  Connection (node) and ConnectionList - accepted connections
// =====================================================
class Connection {
private:
    string firstId;
    string secondId;
    Connection* next;

public:
    Connection(string idOne, string idTwo) {
        firstId = idOne;
        secondId = idTwo;
        next = NULL;
    }

    const string& getFirstId() const  { return firstId; }
    const string& getSecondId() const { return secondId; }
    Connection* getNext() const       { return next; }
    void setNext(Connection* nextConnection) { next = nextConnection; }
};

class ConnectionList {
private:
    Connection* head;

public:
    ConnectionList() {
        head = NULL;
    }

    ~ConnectionList() {
        while (head != NULL) {
            Connection* nodeToDelete = head;
            head = head->getNext();
            delete nodeToDelete;
        }
    }

    // insert at the beginning of the list
    void addConnection(const string& idOne, const string& idTwo) {
        Connection* newConnection = new Connection(idOne, idTwo);
        newConnection->setNext(head);
        head = newConnection;
    }

    bool isEmpty() const {
        return head == NULL;
    }

    bool alreadyConnected(const string& idOne, const string& idTwo) const {
        Connection* current = head;
        while (current != NULL) {
            if ((current->getFirstId() == idOne && current->getSecondId() == idTwo) ||
                (current->getFirstId() == idTwo && current->getSecondId() == idOne))
                return true;
            current = current->getNext();
        }
        return false;
    }

    void viewConnections(const StudentList& students) const {
        Connection* current = head;
        while (current != NULL) {
            Student* first = students.findStudent(current->getFirstId());
            Student* second = students.findStudent(current->getSecondId());
            cout << first->getName() << " <-> " << second->getName() << endl;
            current = current->getNext();
        }
    }
};

// =====================================================
//  Objects used by the program
// =====================================================
StudentList students;
RequestQueue requests;
ConnectionList connections;

// =====================================================
//  Menu functions
// =====================================================
void addStudent() {
    string id, name, branch, skills;
    int year;

    cout << "Enter Student ID: ";
    cin >> id;

    if (students.findStudent(id) != NULL) {
        cout << "A student with this ID already exists.\n";
        return;
    }

    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Branch: ";
    getline(cin, branch);
    cout << "Enter Year: ";
    cin >> year;
    cin.ignore();
    cout << "Enter Skills/Interests: ";
    getline(cin, skills);

    Student* newStudent = new Student(id, name, branch, year, skills);
    students.addStudent(newStudent);
    cout << "Student added.\n";
}

void displayStudents() {
    if (students.isEmpty()) {
        cout << "No students yet.\n";
        return;
    }
    students.displayAll();
}

void sortStudents() {
    if (students.isEmpty()) {
        cout << "No students yet.\n";
        return;
    }

    int choice;
    cout << "1. Sort by ID\n2. Sort by Name\nChoice: ";
    cin >> choice;

    vector<Student*> studentArray = students.toVector();
    mergeSort(studentArray, 0, (int)studentArray.size() - 1, choice == 2);

    for (int i = 0; i < (int)studentArray.size(); i++) {
        studentArray[i]->display();
    }
}

void searchStudent() {
    string id;
    cout << "Enter Student ID to search: ";
    cin >> id;

    Student* foundStudent = students.findStudent(id);
    if (foundStudent == NULL)
        cout << "Student not found.\n";
    else
        foundStudent->display();
}

void sendRequest() {
    string senderId, receiverId;
    cout << "Enter sender ID: ";
    cin >> senderId;
    cout << "Enter receiver ID: ";
    cin >> receiverId;

    if (students.findStudent(senderId) == NULL || students.findStudent(receiverId) == NULL)
        cout << "Both students must be registered.\n";
    else if (senderId == receiverId)
        cout << "You cannot send a request to yourself.\n";
    else if (connections.alreadyConnected(senderId, receiverId))
        cout << "These students are already connected.\n";
    else if (requests.isPending(senderId, receiverId))
        cout << "A request between them is already pending.\n";
    else {
        requests.addRequest(ConnectionRequest(senderId, receiverId));
        cout << "Request sent (Pending).\n";
    }
}

void viewPendingRequests() {
    if (requests.isEmpty()) {
        cout << "No pending requests.\n";
        return;
    }
    requests.displayAll(students);
}

// handles the request at the front of the queue
void acceptOrRejectRequest() {
    if (requests.isEmpty()) {
        cout << "No pending requests.\n";
        return;
    }

    ConnectionRequest request = requests.getFrontRequest();
    Student* sender = students.findStudent(request.getSenderId());
    Student* receiver = students.findStudent(request.getReceiverId());

    cout << sender->getName() << " wants to connect with " << receiver->getName() << ".\n";
    cout << "1. Accept\n2. Reject\nChoice: ";
    int choice;
    cin >> choice;

    requests.removeFrontRequest();

    if (choice == 1) {
        request.setStatus("Accepted");
        connections.addConnection(request.getSenderId(), request.getReceiverId());
        cout << "Request accepted. They are now connected.\n";
    } else {
        request.setStatus("Rejected");
        cout << "Request rejected.\n";
    }
}

void viewConnections() {
    if (connections.isEmpty()) {
        cout << "No connections yet.\n";
        return;
    }
    connections.viewConnections(students);
}

// =====================================================
//  Main
// =====================================================
int main() {
    int choice;
    do {
        cout << "\n===== CAMPUS CONNECT =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display Students\n";
        cout << "3. Sort Students\n";
        cout << "4. Search Student by ID\n";
        cout << "5. Send Connection Request\n";
        cout << "6. View Pending Requests\n";
        cout << "7. Accept/Reject Next Request\n";
        cout << "8. View Connections\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent();            break;
            case 2: displayStudents();       break;
            case 3: sortStudents();          break;
            case 4: searchStudent();         break;
            case 5: sendRequest();           break;
            case 6: viewPendingRequests();   break;
            case 7: acceptOrRejectRequest(); break;
            case 8: viewConnections();       break;
            case 0: cout << "Goodbye!\n";    break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
