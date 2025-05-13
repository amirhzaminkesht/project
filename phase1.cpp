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

class Admin : public User
{
public:
    Admin(int userID, string name, string lastName, string hashedPassword);
    void print() const override;
};

class Meal
{
    int _mealID;
    string _name;
    float _price;
    bool _isActive;
    MealType _mealType;
    ReserveDay _reserveDay;
    vector<string> _sideItems;
public:
    Meal(int mealID, string name, float price, MealType mealType, ReserveDay reserveDay);
    int get_meal_id() const;
    string get_name() const;
    float get_price() const;
    bool get_is_active() const;
    MealType get_meal_type() const;
    ReserveDay get_reserve_day() const;
    vector<string> get_side_items() const;
    void set_meal_id(int id);
    void set_name(const string& name);
    void set_price(float price);
    void set_meal_type(MealType meal_type);
    void set_reserve_day(ReserveDay day);
    void set_is_active(bool active);
    void update_price(float new_price);
    void add_side_item(const string& item);
    void activate();
    void deactivate();
    void print() const;
};

class DiningHall
{
    int _hallID;
    string _name;
    string _address;
    int _capacity;
public:
    DiningHall(int hallID, string name, string address, int capacity);
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

class Student : public User
{
    string _studentID;
    string _email;
    float _balance;
    bool _isActive;
    vector<Reservation*> _reservations;
public:
    Student(int userID, string name, string lastName, string hashedPassword, string studentID, string email, float balance, bool isActive);
    string get_student_id() const;
    string get_email() const;
    float get_balance() const;
    bool get_is_active() const;
    vector<Reservation*>& get_reservations();
    void set_student_id(const string& id);
    void set_email(const string& email);
    void set_balance(float balance);
    void set_is_active(bool isActive);
    void reserve_meal(Meal* meal, DiningHall* dininghall);
    bool cancel_reservation(int index);
    void print() const override;
};

class Reservation
{
    int _reservationID;
    Student* _student;
    DiningHall* _dHall;
    Meal* _meal;
    ReservationStatus _status;
    time_t _createdAt;
public:
    Reservation(int reservationID, Student* student, DiningHall* dHall, Meal* meal);
    int get_reservation_id() const;
    Student* get_student() const;
    DiningHall* get_dHall() const;
    Meal* get_meal() const;
    ReservationStatus get_status() const;
    time_t get_created_at() const;
    void set_reservation_id(int id);
    void set_status(ReservationStatus status);
    bool cancel();
    void print() const;
};

int generateReservationId()
{
    static int id = 1;
    return id++;
}

User::User(int userID, string name, string lastName, string hashedPassword): _userID(userID), _name(name), _lastName(lastName), _hashedPassword(hashedPassword) {}
int User::get_user_id() const { return _userID; }
string User::get_name() const { return _name; }
string User::get_last_name() const { return _lastName; }
string User::get_hashed_password() const { return _hashedPassword; }
void User::set_user_id(int id) { _userID = id; }
void User::set_name(const string& name) { _name = name; }
void User::set_last_name(const string& lastName) { _lastName = lastName; }
void User::set_hashed_password(const string& password) { _hashedPassword = password; }
void User::print() const
{
    cout << "User Info:\n";
    cout << "Name: " << _name << " " << _lastName << endl;
}

Admin::Admin(int userID, string name, string lastName, string hashedPassword): User(userID, name, lastName, hashedPassword) {}
void Admin::print() const
{
    cout << "Admin: " << get_name() << " " << get_last_name() << endl;
}

Meal::Meal(int mealID, string name, float price, MealType mealType, ReserveDay reserveDay): _mealID(mealID), _name(name), _price(price), _mealType(mealType), _reserveDay(reserveDay), _isActive(true) {}
int Meal::get_meal_id() const { return _mealID; }
string Meal::get_name() const { return _name; }
float Meal::get_price() const { return _price; }
bool Meal::get_is_active() const { return _isActive; }
MealType Meal::get_meal_type() const { return _mealType; }
ReserveDay Meal::get_reserve_day() const { return _reserveDay; }
vector<string> Meal::get_side_items() const { return _sideItems; }
void Meal::set_meal_id(int id) { _mealID = id; }
void Meal::set_name(const string& name) { _name = name; }
void Meal::set_price(float price) { _price = price; }
void Meal::set_meal_type(MealType meal_type) { _mealType = meal_type; }
void Meal::set_reserve_day(ReserveDay day) { _reserveDay = day; }
void Meal::set_is_active(bool active) { _isActive = active; }
void Meal::update_price(float new_price) { _price = new_price; }
void Meal::add_side_item(const string& item) { _sideItems.push_back(item); }
void Meal::activate() { _isActive = true; }
void Meal::deactivate() { _isActive = false; }
void Meal::print() const
{
    cout << "Meal Info:\n";
    cout << "meal_id: " << _mealID << endl;
    cout << "name: " << _name << endl;
    cout << "price: " << _price << endl;
    cout << "active: " << (_isActive ? "Yes" : "No") << endl;
}

DiningHall::DiningHall(int hallID, string name, string address, int capacity): _hallID(hallID), _name(name), _address(address), _capacity(capacity) {}
int DiningHall::get_hall_id() const { return _hallID; }
string DiningHall::get_name() const { return _name; }
string DiningHall::get_address() const { return _address; }
int DiningHall::get_capacity() const { return _capacity; }
void DiningHall::set_hall_id(int id) { _hallID = id; }
void DiningHall::set_name(const string& name) { _name = name; }
void DiningHall::set_address(const string& address) { _address = address; }
void DiningHall::set_capacity(int capacity) { _capacity = capacity; }
void DiningHall::print() const
{
    cout << "DiningHall Info:\n";
    cout << "hall_id: " << _hallID << endl;
    cout << "name: " << _name << endl;
    cout << "address: " << _address << endl;
    cout << "capacity: " << _capacity << endl;
}

Student::Student(int userID, string name, string lastName, string hashedPassword, string studentID, string email, float balance, bool isActive): User(userID, name, lastName, hashedPassword), _studentID(studentID), _email(email), _balance(balance), _isActive(isActive) {}
string Student::get_student_id() const { return _studentID; }
string Student::get_email() const { return _email; }
float Student::get_balance() const { return _balance; }
bool Student::get_is_active() const { return _isActive; }
vector<Reservation*>& Student::get_reservations() { return _reservations; }
void Student::set_student_id(const string& id) { _studentID = id; }
void Student::set_email(const string& email) { _email = email; }
void Student::set_balance(float balance) { _balance = balance; }
void Student::set_is_active(bool isActive) { _isActive = isActive; }
void Student::reserve_meal(Meal* meal, DiningHall* dininghall)
{
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);
    for (Reservation* res : _reservations)
    {
        time_t res_created = res->get_created_at();
        tm* res_tm = localtime(&res_created);
        if (now_tm->tm_year == res_tm->tm_year && now_tm->tm_mon == res_tm->tm_mon && now_tm->tm_mday == res_tm->tm_mday)
        {
            if (res->get_meal()->get_meal_type() == meal->get_meal_type())
            {
                cout << "Error: You have already reserved a meal for this period today.\n";
                return;
            }
        }
    }
    if (_balance < meal->get_price())
    {
        cout << "Error: Insufficient balance to reserve this meal.\n";
        return;
    }
    _balance -= meal->get_price();
    int new_id = generateReservationId();
    Reservation* res = new Reservation(new_id, this, dininghall, meal);
    _reservations.push_back(res);
    cout << "Reservation successful!\n";
    res->print();
}
bool Student::cancel_reservation(int index)
{
    if (index < 0 || index >= static_cast<int>(_reservations.size()))
    {
        cout << "Error: Invalid reservation index.\n";
        return false;
    }
    return _reservations[index]->cancel();
}
void Student::print() const
{
    cout << "Student Info:\n";
    cout << "Name: " << get_name() << " " << get_last_name() << endl;
    cout << "Student ID: " << _studentID << endl;
    cout << "Email: " << _email << endl;
    cout << "Balance: " << _balance << endl;
}

Reservation::Reservation(int reservationID, Student* student, DiningHall* dHall, Meal* meal): _reservationID(reservationID), _student(student), _dHall(dHall), _meal(meal), _status(ReservationStatus::SUCCESS)
{
    _createdAt = time(nullptr);
}
int Reservation::get_reservation_id() const { return _reservationID; }
Student* Reservation::get_student() const { return _student; }
DiningHall* Reservation::get_dHall() const { return _dHall; }
Meal* Reservation::get_meal() const { return _meal; }
ReservationStatus Reservation::get_status() const { return _status; }
time_t Reservation::get_created_at() const { return _createdAt; }
void Reservation::set_reservation_id(int id) { _reservationID = id; }
void Reservation::set_status(ReservationStatus status) { _status = status; }
bool Reservation::cancel()
{
    if (_status == ReservationStatus::CANCELLED)
    {
        cout << "Error: Reservation already cancelled.\n";
        return false;
    }
    _status = ReservationStatus::CANCELLED;
    cout << "Reservation cancelled successfully.\n";
    return true;
}
void Reservation::print() const
{
    cout << "Reservation Info:\n";
    cout << "reservation_id: " << _reservationID << endl;
    cout << "status: " << (_status == ReservationStatus::SUCCESS ? "SUCCESS" : "CANCELLED") << endl;
}

int main()
{
    DiningHall hall(1, "Omid Hall", "Birjand University", 300);
    Meal lunch(101, "Kebab", 20000, MealType::LUNCH, ReserveDay::MONDAY);
    Student student(1, "Ali", "Rezaei", "hashed_pass", "S123", "ali@mail.com", 50000, true);
    Admin admin(2, "Admin", "User", "hashed_admin");

    student.reserve_meal(&lunch, &hall);
    student.print();
    admin.print();

    if (!student.get_reservations().empty())
    {
        student.get_reservations()[0]->cancel();
        student.get_reservations()[0]->print();
    }

    return 0;
}
