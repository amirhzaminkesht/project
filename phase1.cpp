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

class Meal
{
    int meal_id;
    string name;
    float price;
//    enum meal_type;
//    vector side_items;
public:
    Meal();
    void print() const;
//    void update_price(float new_price);
//    void add_side_item(str item);
    void setMeal();
    int getMeal() const;
};

class DiningHall
{
    int hall_id;
    string name;
    string address;
    int capacity;
public:
    DiningHall();
    void print() const;
    void setDiningHall();
    int getDiningHall() const;
};

int main()
{
    return 0;
}