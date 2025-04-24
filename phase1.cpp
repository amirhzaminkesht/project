#include <iostream>
using namespace std;

class Student
{
    int user_id;
    string student_id;
    string name;
    string email;
    float balance;
    bool is_active;
    public:
    Student();
    void print() const;
//    reserve_meal(meal);
//    bool cancel_reservation(reservation);
    void setStudent();
    int getStudent();
};

int main()
{
    return 0;
}