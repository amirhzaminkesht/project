#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum class MealType {BREAKFAST, LUNCH, DINNER};
enum class ReserveDay {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};
enum class ReservationStatus {SUCCESS, CANCELLED, FAILED};

class User
{
    int _userID;
    string _name;
    string _lastName;
    string _hashedPassword;
public:
    User(int userID, string name, string lastName, string hashedPassword);
    int get_user_id() const;
    string get_name() const;
    string get_last_name() const;
    string get_hashed_password() const;
    void set_user_id(int id);
    void set_name(const string& name);
    void set_last_name(const string& lastName);
    void set_hashed_password(const string& password);
    virtual void print() const;
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

class Reservation;

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

User:: User(int userID, string name, string lastName, string hashedPassword): _userID(userID), _name(name), _lastName(lastName), _hashedPassword(hashedPassword) {}

int User:: get_user_id() const {return _userID;}
string User:: get_name() const {return _name;}
string User:: get_last_name() const {return _lastName;}
string User:: get_hashed_password() const {return _hashedPassword;}

void User:: set_user_id(int id) {_userID = id;}
void User:: set_name(const string& name) {_name = name;}
void User:: set_last_name(const string& lastName) {_lastName = lastName;}
void User:: set_hashed_password(const string& password) {_hashedPassword = password;}

void User:: print() const
{
    cout << "User Info:\n";
    cout << "Name: " << _name << " " << _lastName << endl;
}

Meal:: Meal(int meal_id, string name, float price, MealType meal_type): meal_id(meal_id), name(name), price(price), meal_type(meal_type) {}

int Meal:: get_meal_id() const {return meal_id;}
string Meal:: get_name() const {return name;}
float Meal:: get_price() const {return price;}
MealType Meal:: get_meal_type() const {return meal_type;}
vector<string> Meal:: get_side_items() const {return side_items;}

void Meal:: set_meal_id(int id) {meal_id = id;}
void Meal:: set_name(const string& name) {this->name = name;}
void Meal:: set_price(float price) {this->price = price;}
void Meal:: set_meal_type(MealType meal_type) {this->meal_type = meal_type;}

void Meal:: update_price(float new_price) {price = new_price;}
void Meal:: add_side_item(const string& item) {side_items.push_back(item);}

void Meal:: print() const
{
    cout << "Meal Info:\n";
    cout << "meal_id: " << meal_id << endl;
    cout << "name: " << name << endl;
    cout << "price: " << price << endl;
}

int generateReservationId()
{
    static int id_counter = 1;
    return id_counter++;
}

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

Student:: Student(int user_id, string student_id, string name, string email, float balance, bool is_active): user_id(user_id), student_id(student_id), name(name), email(email), balance(balance), is_active(is_active) {}

int Student:: get_user_id() const {return user_id;}
string Student:: get_student_id() const {return student_id;}
string Student:: get_name() const {return name;}
string Student:: get_email() const {return email;}
float Student:: get_balance() const {return balance;}
bool Student:: get_is_active() const {return is_active;}
vector<Reservation*>& Student:: get_reservations() {return reservations;}

void Student:: set_user_id(int id) {user_id = id;}
void Student:: set_student_id(const string& id) {student_id = id;}
void Student:: set_name(const string& name) {this->name = name;}
void Student:: set_email(const string& email) {this->email = email;}
void Student:: set_balance(float balance) {this->balance = balance;}
void Student:: set_is_active(bool is_active) {this->is_active = is_active;}

void Student:: reserve_meal(Meal *meal, DiningHall *dininghall)
{
    time_t now = time(nullptr);
    tm *now_tm = localtime(&now);

    for(Reservation *res : reservations)
    {
        time_t res_created = res->get_created_at();
        tm *res_tm = localtime(&res_created);
        if((now_tm->tm_year == res_tm->tm_year) && (now_tm->tm_mon == res_tm->tm_mon) && (now_tm->tm_mday == res_tm->tm_mday))
        {
            if(res->get_meal()->get_meal_type() == meal->get_meal_type())
            {
                cout << "Error: You have already reserved a meal for this period today.\n";
                return;
            }
        }
    }

    if(balance < meal->get_price())
    {
        cout << "Error: Insufficient balance to reserve this meal.\n";
        return;
    }

    balance -= meal->get_price();

    int new_reservation_id = generateReservationId();
    Reservation *newReservation = new Reservation(new_reservation_id, this, dininghall, meal);
    reservations.push_back(newReservation);

    cout << "Reservation successful!\n";
    newReservation->print();
}

bool Student:: cancel_reservation(int index)
{
    if(index < 0 || index >= static_cast<int>(reservations.size()))
    {
        cout << "Error: Invalid reservation index.\n";
        return false;
    }
    bool success = reservations[index]->cancel();
    return success;
}

void Student:: print() const
{
    cout << "Student Info:\n";
    cout << "Student ID: " << student_id << "\n";
    cout << "Name: " << name << "\n";
    cout << "Email: " << email << "\n";
    cout << "Balance: " << balance << "\n";
}

Reservation:: Reservation(int reservation_id, Student *student, DiningHall *dHall, Meal *meal): reservation_id(reservation_id), student(student), dHall(dHall), meal(meal), status(ReservationStatus:: SUCCESS)
{
    created_at = time(nullptr);
}

int Reservation:: get_reservation_id() const {return reservation_id;}
Student *Reservation:: get_student() const {return student;}
DiningHall *Reservation:: get_dHall() const {return dHall;}
Meal *Reservation:: get_meal() const {return meal;}
ReservationStatus Reservation:: get_status() const {return status;}
time_t Reservation:: get_created_at() const {return created_at;}

void Reservation:: set_reservation_id(int id) {reservation_id = id;}
void Reservation:: set_status(ReservationStatus status) {this->status = status;}

bool Reservation:: cancel()
{
    if(status == ReservationStatus:: CANCELLED)
    {
        cout << "Error: Reservation already cancelled.\n";
        return false;
    }
    status = ReservationStatus:: CANCELLED;
    cout << "Reservation cancelled successfully.\n";
    return true;
}

void Reservation:: print() const
{
    cout << "Reservation Info:\n";
    cout << "reservation_id: " << reservation_id << endl;
    cout << "status: " << (status == ReservationStatus:: SUCCESS? "SUCCESS": "CANCELLED") << endl;
}

int main()
{
    cout << "Meal Reservation System Running!\n";

    DiningHall hall1(1, "Omid", "4th Avenue, Birjand University, Birjand", 500);

    Meal breakfast(101, "Butter", 6000, MealType:: BREAKFAST);
    Meal lunch(102, "Kebab", 15000, MealType:: LUNCH);

    Student student1(1, "S12345", "Amir", "amir@example.com", 100.0, true);

    student1.reserve_meal(&breakfast, &hall1);

    student1.reserve_meal(&breakfast, &hall1);

    student1.reserve_meal(&lunch, &hall1);

    cout << "\n--- Final Student Information ---\n";
    student1.print();

    cout << "\nCancelling first reservation...\n";
    student1.cancel_reservation(0);

    return 0;
}
