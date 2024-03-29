#include <iostream>
#include <string>

// abstract class
class drivable {
    public:
        virtual void drive() = 0; // no implementation
};

class tire
{
private:
    /* data */
public:
    tire    (/* args */);
    ~tire    ();
};

tire::tire(/* args */)
{
}

tire::~tire()
{
}


class wheel {
    public:
        // wheel() {
        //     std::cout << "wheel default constructor" << std::endl;
        // }

        wheel(tire t) : t(t) {
            std::cout << "wheel constructor" << std::endl;
        }
        ~wheel() {
            std::cout << "wheel destructor" << std::endl;
        }
    
        void chaange_tire(tire t1) {
            t = t1;
        }
    private:
        tire t; // this is an association, the lifetime of the tire is not managed by the wheel

};


// car implements drivable
class car : public drivable {
    public:
        car()  
        : fl(tire()), fr(tire()), rl(tire()), rr(tire())
        {
            std::cout << "car constructor" << std::endl;
        }
        ~car() {
            std::cout << "car destructor" << std::endl;
            // wheels will be destroyed here

        }
        wheel fl; // this is a composition, the lifetime of the wheel is managed by the car
        wheel fr;
        wheel rl;
        wheel rr;

        void drive() {
            std::cout << "driving" << std::endl;
        }
};


int main() {
    car c;
    c.drive();
    return 0;
}
