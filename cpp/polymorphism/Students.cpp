#include <iostream>
#include <string>

using namespace std;

class Person {
    protected:
    string name;

    public:
    Person(string s) : name {s} {
        
    }
    virtual void print_record() = 0; //pure virutal
    virtual ~Person() {}
};

class Student : public virtual Person {
    protected:
    int id_number;

    public:
    Student(string s, int num) : Person(s), id_number{num} {

    }
    void print_record() {cout << name << "\t" << id_number;}
};

class Teacher : public virtual Person{
    protected:
    string department;
    public:
    Teacher(string s, string dep) : Person(s), department{dep}{

    }
    void print_record() {cout << name << "\t" << department;}
};

class StudentTeacher : public Student, public Teacher {
    public:
    StudentTeacher(string n, int num, string d) : Person(n), Student(n, num), Teacher(n, d) {}
    void print_record() {cout << name << "\t" << id_number << "\t" << department;}
};

int main(int argc, char* argv[]) {
    cout.setf(std::ios_base::fixed);
    Student st1{"Fred", 1234};
    Student st2 = Student{"Alice", 1235};
    Person* p1 = &st1;
    Person& ref = st2;
    Teacher t1{"Martin", "CompSci"};
    p1->print_record();
    ref.print_record();
    StudentTeacher s_t {"Paul", 1267, "IT"};
    s_t.print_record();
}