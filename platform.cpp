#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// User base class
class User {
private:
    string username;
    string password;
    string email;

public:
    User(const string &username, const string &password, const string &email)
        : username(username), password(password), email(email) {}

    string getUsername() const {
        return username;
    }

    void setUsername(const string &username) {
        this->username = username;
    }

    string getPassword() const {
        return password;
    }

    void setPassword(const string &password) {
        this->password = password;
    }

    string getEmail() const {
        return email;
    }

    void setEmail(const string &email) {
        this->email = email;
    }

    bool authenticate(const string &password) const {
        return this->password == password;
    }
    virtual ~User() {}
};

// Forward declaration
class Course;

// Student class
class Student : public User {
private:
    vector<Course*> enrolledCourses;

public:
    Student(const string &username, const string &password, const string &email)
        : User(username, password, email) {}

    void enrollCourse(Course *course) {
        enrolledCourses.push_back(course);
    }

    const vector<Course*>& getEnrolledCourses() const {
        return enrolledCourses;
    }

    void viewLectures() const;
};

// Lecture class
class Lecture {
private:
    string title;
    string content;
    int duration; // in minutes

public:
    Lecture(const string &title, const string &content, int duration)
        : title(title), content(content), duration(duration) {}

    string getTitle() const {
        return title;
    }

    string getContent() const {
        return content;
    }

    int getDuration() const {
        return duration;
    }
};

// Instructor class
class Instructor : public User {
private:
    vector<Course*> createdCourses;

public:
    Instructor(const string &username, const string &password, const string &email)
        : User(username, password, email) {}

    void createCourse(Course *course) {
        createdCourses.push_back(course);
    }

    void editCourse(int courseIndex, const string &newTitle, const string &newDescription);

    void deleteCourse(int courseIndex);

    void addLectureToCourse(int courseIndex, Lecture *lecture);

    void viewEnrolledStudents(int courseIndex) const;

    void uploadMaterialToCourse(int courseIndex, const string &material);

    const vector<Course*>& getCreatedCourses() const {
        return createdCourses;
    }
};

// Course class
class Course {
private:
    string title;
    string description;
    Instructor *instructor;
    vector<Lecture*> lectures;
    vector<Student*> enrolledStudents;
    vector<string> materials; // List to hold course materials

public:
    Course(const string &title, const string &description, Instructor *instructor)
        : title(title), description(description), instructor(instructor) {}

    void addLecture(Lecture *lecture) {
        lectures.push_back(lecture);
    }

    void enrollStudent(Student *student) {
        enrolledStudents.push_back(student);
        student->enrollCourse(this);
    }

    void addMaterial(const string &material) {
        materials.push_back(material);
    }

    string getTitle() const {
        return title;
    }

    void setTitle(const string &title) {
        this->title = title;
    }

    string getDescription() const {
        return description;
    }

    void setDescription(const string &description) {
        this->description = description;
    }

    Instructor* getInstructor() const {
        return instructor;
    }

    const vector<Lecture*>& getLectures() const {
        return lectures;
    }

    const vector<Student*>& getEnrolledStudents() const {
        return enrolledStudents;
    }

    const vector<string>& getMaterials() const {
        return materials;
    }
};

void Instructor::editCourse(int courseIndex, const string &newTitle, const string &newDescription) {
    if (courseIndex >= 0 && courseIndex < createdCourses.size()) {
        createdCourses[courseIndex]->setTitle(newTitle);
        createdCourses[courseIndex]->setDescription(newDescription);
        cout << "Course updated successfully!" << endl;
    } else {
        cout << "Invalid course index!" << endl;
    }
}

void Instructor::deleteCourse(int courseIndex) {
    if (courseIndex >= 0 && courseIndex < createdCourses.size()) {
        createdCourses.erase(createdCourses.begin() + courseIndex);
        cout << "Course deleted successfully!" << endl;
    } else {
        cout << "Invalid course index!" << endl;
    }
}

void Instructor::addLectureToCourse(int courseIndex, Lecture *lecture) {
    if (courseIndex >= 0 && courseIndex < createdCourses.size()) {
        createdCourses[courseIndex]->addLecture(lecture);
    } else {
        cout << "Invalid course index!" << endl;
    }
}

void Instructor::viewEnrolledStudents(int courseIndex) const {
    if (courseIndex >= 0 && courseIndex < createdCourses.size()) {
        Course *course = createdCourses[courseIndex];
        cout << "Students enrolled in " << course->getTitle() << ":" << endl;
        for (Student *student : course->getEnrolledStudents()) {
            cout << student->getUsername() << endl;
        }
    } else {
        cout << "Invalid course index!" << endl;
    }
}

void Instructor::uploadMaterialToCourse(int courseIndex, const string &material) {
    if (courseIndex >= 0 && courseIndex < createdCourses.size()) {
        createdCourses[courseIndex]->addMaterial(material);
        cout << "Material uploaded successfully!" << endl;
    } else {
        cout << "Invalid course index!" << endl;
    }
}

void Student::viewLectures() const {
    for (Course *course : enrolledCourses) {
        cout << "Lectures for course: " << course->getTitle() << endl;
        for (Lecture *lecture : course->getLectures()) {
            cout << lecture->getTitle() << ": " << lecture->getDuration() << " minutes" << endl;
        }
    }
}

// CoursePlatform class
class CoursePlatform {
private:
    vector<User*> users;
    vector<Course*> courses;

public:
    ~CoursePlatform() {
        for (User *user : users) {
            delete user;
        }
        for (Course *course : courses) {
            delete course;
        }
    }

    void registerUser(User *user) {
        users.push_back(user);
    }

    User* loginUser(const string &username, const string &password) const {
        for (User *user : users) {
            if (user->getUsername() == username && user->authenticate(password)) {
                return user;
            }
        }
        return nullptr;
    }

    void addCourse(Course *course) {
        courses.push_back(course);
    }

    const vector<Course*>& getCourses() const {
        return courses;
    }

    static void main() {
        CoursePlatform platform;
        string input;
        int choice;

        // Sample Data
        Instructor *instructor = new Instructor("instructor1", "pass123", "instructor1@example.com");
        platform.registerUser(instructor);
        Student *student = new Student("student1", "pass123", "student1@example.com");
        platform.registerUser(student);

        Course *course = new Course("Java Programming", "Learn Java from scratch", instructor);
        platform.addCourse(course);
        instructor->createCourse(course);
        course->addLecture(new Lecture("Introduction", "Welcome to Java Programming", 30));

        while (true) {
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Exit" << endl;
            cout << "Choose an option: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                string username, password, email, userType;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                cout << "Enter email: ";
                getline(cin, email);
                cout << "Are you a student or instructor (s/i)? ";
                getline(cin, userType);

                if (userType == "s") {
                    Student *newStudent = new Student(username, password, email);
                    platform.registerUser(newStudent);
                    cout << "Student registered successfully!" << endl;
                } else if (userType == "i") {
                    Instructor *newInstructor = new Instructor(username, password, email);
                    platform.registerUser(newInstructor);
                    cout << "Instructor registered successfully!" << endl;
                } else {
                    cout << "Invalid user type!" << endl;
                }
            } else if (choice == 2) {
                string username, password;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);

                User *user = platform.loginUser(username, password);
                if (user) {
                    cout << "Login successful!" << endl;

                    if (Student *student = dynamic_cast<Student*>(user)) {
                        while (true) {
                            cout << "1. View enrolled courses" << endl;
                            cout << "2. View lectures" << endl;
                            cout << "3. Logout" << endl;
                            cout << "Choose an option: ";
                            cin >> choice;
                            cin.ignore();

                            if (choice == 1) {
                                cout << "Enrolled courses:" << endl;
                                for (Course *course : student->getEnrolledCourses()) {
                                    cout << course->getTitle() << endl;
                                }
                            } else if (choice == 2) {
                                student->viewLectures();
                            } else if (choice == 3) {
                                break;
                            } else {
                                cout << "Invalid choice!" << endl;
                            }
                        }
                    } else if (Instructor *instructor = dynamic_cast<Instructor*>(user)) {
                        while (true) {
                            cout << "1. View created courses" << endl;
                            cout << "2. Create course" << endl;
                            cout << "3. Edit course" << endl;
                            cout << "4. Delete course" << endl;
                            cout << "5. Add lecture to course" << endl;
                            cout << "6. View enrolled students" << endl;
                            cout << "7. Upload material to course" << endl;
                            cout << "8. Logout" << endl;
                            cout << "Choose an option: ";
                            cin >> choice;
                            cin.ignore();

                            if (choice == 1) {
                                cout << "Created courses:" << endl;
                                for (Course *course : instructor->getCreatedCourses()) {
                                    cout << course->getTitle() << endl;
                                }
                            } else if (choice == 2) {
                                string title, description;
                                cout << "Enter course title: ";
                                getline(cin, title);
                                cout << "Enter course description: ";
                                getline(cin, description);
                                Course *newCourse = new Course(title, description, instructor);
                                platform.addCourse(newCourse);
                                instructor->createCourse(newCourse);
                                cout << "Course created successfully!" << endl;
                            } else if (choice == 3) {
                                int courseIndex;
                                string newTitle, newDescription;
                                cout << "Enter course index: ";
                                cin >> courseIndex;
                                cin.ignore();
                                cout << "Enter new title: ";
                                getline(cin, newTitle);
                                cout << "Enter new description: ";
                                getline(cin, newDescription);
                                instructor->editCourse(courseIndex - 1, newTitle, newDescription);
                            } else if (choice == 4) {
                                int courseIndex;
                                cout << "Enter course index: ";
                                cin >> courseIndex;
                                cin.ignore();
                                instructor->deleteCourse(courseIndex - 1);
                            } else if (choice == 5) {
                                int courseIndex;
                                string title, content;
                                int duration;
                                cout << "Enter course index: ";
                                cin >> courseIndex;
                                cin.ignore();
                                cout << "Enter lecture title: ";
                                getline(cin, title);
                                cout << "Enter lecture content: ";
                                getline(cin, content);
                                cout << "Enter lecture duration (in minutes): ";
                                cin >> duration;
                                cin.ignore();
                                Lecture *lecture = new Lecture(title, content, duration);
                                instructor->addLectureToCourse(courseIndex - 1, lecture);
                            } else if (choice == 6) {
                                int courseIndex;
                                cout << "Enter course index: ";
                                cin >> courseIndex;
                                cin.ignore();
                                instructor->viewEnrolledStudents(courseIndex - 1);
                            } else if (choice == 7) {
                                int courseIndex;
                                string material;
                                cout << "Enter course index: ";
                                cin >> courseIndex;
                                cin.ignore();
                                cout << "Enter material: ";
                                getline(cin, material);
                                instructor->uploadMaterialToCourse(courseIndex - 1, material);
                            } else if (choice == 8) {
                                break;
                            } else {
                                cout << "Invalid choice!" << endl;
                            }
                        }
                    }
                } else {
                    cout << "Invalid username or password!" << endl;
                }
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice!" << endl;
            }
        }
    }
};

int main() {
    CoursePlatform::main();
    return 0;
}
