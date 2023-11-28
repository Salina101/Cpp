#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>

using namespace std;

class User
{
public:
    string username;
    string password;
    string role;

    virtual ~User() {}

    User(const string &username, const string &password, const string &role)
        : username(username), password(password), role(role) {}
};

class Admin : public User
{
public:
    Admin(const string &username, const string &password)
        : User(username, password, "Admin") {}
};

class Student : public User
{
public:
    Student(const string &username, const string &password)
        : User(username, password, "Student") {}
};

class Teacher : public User
{
public:
    Teacher(const string &username, const string &password)
        : User(username, password, "Teacher") {}
};

class MultipleChoiceQuiz
{
public:
    string question;
    vector<string> options;
    int correctOption;

    MultipleChoiceQuiz(const string &question, const vector<string> &options, int correctOption)
        : question(question), options(options), correctOption(correctOption) {}
};

class Attendance
{
public:
    string date;
    vector<string> presentStudents;

    Attendance(const string &date) : date(date) {}
};

vector<User> users;
vector<MultipleChoiceQuiz> multipleChoiceQuizzes;
vector<Attendance> attendanceRecords;
User *currentUser = nullptr;

void readUsersFromFile()
{
    ifstream userFile("users.txt");
    if (userFile.is_open())
    {
        string line;
        while (getline(userFile, line))
        {
            istringstream iss(line);
            string username, password, role;
            getline(iss, username, ',');
            getline(iss, password, ',');
            getline(iss, role, ',');
            if (role == "Admin")
            {
                users.push_back(Admin(username, password));
            }
            else if (role == "Student")
            {
                users.push_back(Student(username, password));
            }
            else if (role == "Teacher")
            {
                users.push_back(Teacher(username, password));
            }
        }
        userFile.close();
    }
}

void writeUsersToFile()
{
    ofstream userFile("users.txt");
    if (userFile.is_open())
    {
        for (const User &user : users)
        {
            userFile << user.username << "," << user.password << "," << user.role << "\n";
        }
        userFile.close();
    }
}

void createUser(vector<User> &users)
{
    string username, password, role;
    cout << "Enter the new user's username: ";
    cin >> username;

    for (const User &user : users)
    {
        if (user.username == username)
        {
            cout << "Username already exists. User not created." << endl;
            return;
        }
    }

    cout << "Enter the new user's password: ";
    cin >> password;

    cout << "Enter the new user's role (Admin, Teacher, or Student): ";
    cin >> role;

    if (role == "Admin" || role == "Teacher" || role == "Student")
    {
        users.push_back(User(username, password, role));
        cout << "User created successfully." << endl;
    }
    else
    {
        cout << "Invalid role. User not created." << endl;
    }
}

void addMultipleChoiceQuiz()
{
    if (currentUser == nullptr || currentUser->role != "Teacher")
    {
        cout << "You are not authorized to add quizzes." << endl;
        return;
    }

    string question;
    vector<string> options(4);
    int correctOption;

    cout << "Enter the multiple-choice question: ";
    cin.ignore();
    getline(cin, question);

    cout << "Enter four options for the question:" << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "Option " << i + 1 << ": ";
        getline(cin, options[i]);
    }

    cout << "Enter the correct option (1 to 4): ";
    cin >> correctOption;

    if (correctOption < 1 || correctOption > 4)
    {
        cout << "Invalid correct option. Quiz not added." << endl;
        return;
    }

    multipleChoiceQuizzes.push_back(MultipleChoiceQuiz(question, options, correctOption));
    cout << "Multiple-choice quiz added successfully." << endl;
}

void markAttendance()
{
    if (currentUser == nullptr || currentUser->role != "Teacher")
    {
        cout << "You are not authorized to mark attendance." << endl;
        return;
    }

    string date = "2023-11-01"; // You can use a timestamp here

    cout << "Marking attendance for " << date << endl;

    Attendance attendance(date);

    for (User &user : users)
    {
        if (user.role == "Student")
        {
            string present;
            cout << "Is " << user.username << " present? (yes/no): ";
            cin >> present;
            if (present == "yes")
            {
                attendance.presentStudents.push_back(user.username);
            }
        }
    }

    attendanceRecords.push_back(attendance);
    cout << "Attendance marked successfully." << endl;
}

void takeQuiz()
{
    if (currentUser == nullptr || currentUser->role != "Student")
    {
        cout << "You are not authorized to take quizzes." << endl;
        return;
    }

    if (multipleChoiceQuizzes.empty())
    {
        cout << "No quizzes available at the moment." << endl;
        return;
    }

    int score = 0;
    for (const MultipleChoiceQuiz &quiz : multipleChoiceQuizzes)
    {
        cout << quiz.question << endl;
        for (int i = 0; i < quiz.options.size(); i++)
        {
            cout << i + 1 << ". " << quiz.options[i] << endl;
        }

        int answer;
        cout << "Your answer (1 to 4): ";
        cin >> answer;

        if (answer < 1 || answer > 4)
        {
            cout << "Invalid answer. Quiz not submitted." << endl;
            return;
        }

        if (answer == quiz.correctOption)
        {
            cout << "Correct!" << endl;
            score++;
        }
        else
        {
            cout << "Incorrect. The correct answer is option " << quiz.correctOption << "." << endl;
        }
    }

    cout << "Quiz submitted. Your score: " << score << " out of " << multipleChoiceQuizzes.size() << endl;
}

int main()
{
    readUsersFromFile();

    while (true)
    {
        int choice;
        cout << "Main Menu:" << endl;
        cout << "1. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            for (User &user : users)
            {
                if (user.username == username && user.password == password)
                {
                    currentUser = &user;
                    cout << user.role << " login successful." << endl;
                    break;
                }
            }

            if (currentUser == nullptr)
            {
                cout << "Login failed. Invalid username or password." << endl;
            }
        }
        else if (choice == 3)
        {
            cout << "Exiting the system." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }

        if (currentUser != nullptr)
        {
            while (true)
            {
                cout << "User Menu:" << endl;
                if (currentUser->role == "Admin")
                {
                    cout << "4. Manage Users" << endl;
                    cout << "5. Logout" << endl;
                    cout << "Enter your choice: ";
                    int adminChoice;
                    cin >> adminChoice;
                    if (adminChoice == 4)
                    {
                        // Admin menu for managing users
                        cout << "Admin Menu for Managing Users:" << endl;
                        cout << "1. Create User" << endl;
                        cout << "2. Logout" << endl;
                        cout << "Enter your choice: ";
                        int adminUserChoice;
                        cin >> adminUserChoice;
                        if (adminUserChoice == 1)
                        {
                            createUser(users);
                        }
                        else if (adminUserChoice == 2)
                        {
                            currentUser = nullptr;
                            cout << "Logged out successfully." << endl;
                            break;
                        }
                        else
                        {
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    }
                    else if (adminChoice == 5)
                    {
                        currentUser = nullptr;
                        cout << "Logged out successfully." << endl;
                        break;
                    }
                    else
                    {
                        cout << "Invalid choice. Please try again." << endl;
                    }
                }
                else if (currentUser->role == "Teacher")
                {
                    int teacherChoice;
                    cout << "4. Add Multiple-Choice Quiz" << endl;
                    cout << "5. Mark Attendance" << endl;
                    cout << "6. Logout" << endl;
                    cout << "Enter your choice: ";
                    cin >> teacherChoice;
                    if (teacherChoice == 4)
                    {
                        addMultipleChoiceQuiz();
                    }
                    else if (teacherChoice == 5)
                    {
                        markAttendance();
                    }
                    else if (teacherChoice == 6)
                    {
                        currentUser = nullptr;
                        cout << "Logged out successfully." << endl;
                        break;
                    }
                    else
                    {
                        cout << "Invalid choice. Please try again." << endl;
                    }
                }
                else if (currentUser->role == "Student")
                {
                    int studentChoice;
                    cout << "4. Take Quiz" << endl;
                    cout << "5. Logout" << endl;
                    cout << "Enter your choice: ";
                    cin >> studentChoice;
                    if (studentChoice == 4)
                    {
                        takeQuiz();
                    }
                    else if (studentChoice == 5)
                    {
                        currentUser = nullptr;
                        cout << "Logged out successfully." << endl;
                        break;
                    }
                    else
                    {
                        cout << "Invalid choice. Please try again." << endl;
                    }
                }
            }
        }
    }

    writeUsersToFile();

    return 0;
}
