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
    int getStudent() const;
};

class Resevation
{
    int reservation_id;
//    Student student;
//    DiningHall dhall;
//    enum status;
//    time_t created_at;

public:
    Resevation();
    void print() const;
    bool cancel();
    void setReservation();
    int getReservation();
};

int main()
{
    return 0;
}