#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum class MealType {BREAKFAST, LUNCH, DINNER};
enum class ReservationStatus {SUCCESS, CANCELLED, FAILED};

class Student
{
    int user_id;
    string student_id;
    string name;
    string email;
    float balance;
    bool is_active;
    vector<Reservation*> reservations;
public:
    Student(int user_id, string student_id, string name, string email, float balance, bool is_active);

    int get_user_id() const;
    string get_student_id() const;
    string get_name() const;
    string get_email() const;
    float get_balance() const;
    bool get_is_active() const;
    vector<Reservation*>& get_reservations();
    
    void set_user_id(int id);
    void set_student_id(const string& id);
    void set_name(const string& name);
    void set_email(const string& email);
    void set_balance(float balance);
    void set_is_active(bool is_active);
    
    void reserve_meal(Meal *meal, DiningHall *dininghall);
    bool cancel_reservation(int index);
    void print() const;
};

class Reservation
{
    int reservation_id;
    Student *student;
    DiningHall *dHall;
    Meal *meal;
    ReservationStatus status;
    time_t created_at;
public:
    Reservation(int reservation_id, Student *student, DiningHall *dHall, Meal *meal);
    
    int get_reservation_id() const;
    Student *get_student() const;
    DiningHall *get_dHall() const;
    Meal *get_meal() const;
    ReservationStatus get_status() const;
    time_t get_created_at() const;

    void set_reservation_id(int id);
    void set_status(ReservationStatus status);

    bool cancel();
    void print() const;
};

class Meal
{
    int meal_id;
    string name;
    float price;
    MealType meal_type;
    vector<string> side_items;
public:
    Meal(int meal_id, string name, float price, MealType meal_type);
    
    int get_meal_id() const;
    string get_name() const;
    float get_price() const;
    MealType get_meal_type() const;
    vector<string> get_side_items() const;
    
    void set_meal_id(int id);
    void set_name(const string &name);
    void set_price(float price);
    void set_meal_type(MealType meal_type);

    void update_price(float new_price);
    void add_side_item(const string& item);
    void print() const;
};

class DiningHall
{
    int hall_id;
    string name;
    string address;
    int capacity;
public:
    DiningHall(int hall_id, string name, string address, int capacity);

    int get_hall_id() const;
    string get_name() const;
    string get_address() const;
    int get_capacity() const;
    
    void set_hall_id(int id);
    void set_name(const string& name);
    void set_address(const string& address);
    void set_capacity(int capacity);
    
    void print() const;
};

DiningHall:: DiningHall(int hall_id, string name, string address, int capacity): hall_id(hall_id), name(name), address(address), capacity(capacity) {}

int DiningHall:: get_hall_id() const {return hall_id;}
string DiningHall:: get_name() const {return name;}
string DiningHall:: get_address() const {return address;}
int DiningHall:: get_capacity() const {return capacity;}

void DiningHall:: set_hall_id(int id) {hall_id = id;}
void DiningHall:: set_name(const string& name) {this->name = name;}
void DiningHall:: set_address(const string& address) {this->address = address;}
void DiningHall:: set_capacity(int capacity) {this->capacity = capacity;}

void DiningHall:: print() const
{
    cout << "DiningHall Info:\n";
    cout << "hall_id: " << hall_id << endl;
    cout << "name: " << name << endl;
    cout << "address: " << address << endl;
    cout << "capacity: " << capacity << endl;
}



int main()
{
    return 0;
}