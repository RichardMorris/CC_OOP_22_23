#include <iostream>
#include <string>

using namespace std;

// abstract base class
class Person {
    protected:
    string name;

    public:
        Person(string s) : name {s} {}
        virtual void print_record() = 0; // pure virtual
        virtual ~Person() {} 
};

class Student : public virtual Person {
    protected:
    int id_number;

    public:
        Student(string s, int num) : Person(s), id_number{num} { } 
        void print_record() { cout << name << "\t" << id_number << endl; }
};

class Teacher : public virtual Person {
    protected:
    string department;
    public:
        Teacher(string s, string dep) : Person(s), department{dep} { } 
        void print_record() { cout << name << "\t" << department << endl; }
};

// multiple inheretance
class StudentTeacher : public Student, public Teacher {
    public:
        StudentTeacher(string n, int num, string d) :
            Person(n),
            Student(n,num),
            Teacher(n,d) {}
        void print_record() { cout << name << "\t" << id_number << "\t" << department << endl; }

};

void foo(Person* p) {
    p->print_record();
}

void bar(Person& p) {
    p.print_record();
}


int main(int argc, char* argv[]) {

    Student st1{"Fred", 1234};
    Student st2 = Student{"Alice", 1235};
    Person* p1 = &st1;
    Person& ref = st2;
//    ref.print_record();
    foo(p1);
    bar(ref);
    Teacher t1{"Martin", "CompSci"};
    bar(t1);
    //foobar(t1);
    StudentTeacher s_t { "Paul", 1236, "IT"};
    bar(s_t);
}
