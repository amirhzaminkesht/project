#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

enum class MealType { BREAKFAST, LUNCH, DINNER };
enum class ReserveDay { SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY };
enum class ReservationStatus { SUCCESS, CANCELLED, FAILED };

class Reservation;
class Meal;
class DiningHall;

class User
{
    int user_id;
    string name;
    string last_name;
    string hashed_password;

public:
    User(int, string, string, string);
    virtual void print() const;
    int get_user_id() const;
    string get_name() const;
    string get_last_name() const;
    string get_hashed_password() const;
    void set_user_id(int);
    void set_name(const string &);
    void set_last_name(const string &);
    void set_hashed_password(const string &);
};

class Student : public User
{
    string student_id;
    string email;
    string phone;
    float balance;
    bool is_active;
    vector<Reservation *> reserves;

public:
    Student(int, string, string, string, string, string, string, float);
    void print() const override;
    void reserve_meal(Meal *, DiningHall *);
    void activate();
    void deactivate();
    bool get_is_active() const;
    float get_balance() const;
    vector<Reservation *> &get_reserves();
};

class Admin : public User
{
public:
    Admin(int, string, string, string);
    void print() const override;
};

class Meal
{
    int meal_id;
    string name;
    float price;
    bool is_active;
    MealType meal_type;
    ReserveDay reserve_day;
    vector<string> side_items;

public:
    Meal(int, string, float, MealType, ReserveDay);
    void activate();
    void deactivate();
    void add_side_item(const string &);
    void print() const;
    float get_price() const;
    MealType get_meal_type() const;
};

class DiningHall
{
    int hall_id;
    string name;
    string address;
    int capacity;

public:
    DiningHall(int, string, string, int);
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
    Reservation(int, Student *, DiningHall *, Meal *);
    void print() const;
    bool cancel();
    Meal *get_meal() const;
    time_t get_created_at() const;
    ReservationStatus get_status() const;
};

class Storage
{
    vector<Meal *> meals;
    vector<DiningHall *> halls;

    Storage() {}

public:
    static Storage &getInstance()
    {
        static Storage instance;
        return instance;
    }

    void addMeal(Meal *meal)
    {
        meals.push_back(meal);
    }

    void addDiningHall(DiningHall *hall)
    {
        halls.push_back(hall);
    }
};

class Panel
{
public:
    void showMenu()
    {
        cout << "1. View Reservations\n2. Reserve Meal\n3. Cancel Reservation\n";
    }

    void viewReservations(Student &student)
    {
        for (auto res : student.get_reserves())
            res->print();
    }
};

// ---------------- Implementations ---------------- //

User::User(int uid, string n, string l, string p) : user_id(uid), name(n), last_name(l), hashed_password(p) {}
int User::get_user_id() const { return user_id; }
string User::get_name() const { return name; }
string User::get_last_name() const { return last_name; }
string User::get_hashed_password() const { return hashed_password; }
void User::set_user_id(int id) { user_id = id; }
void User::set_name(const string &n) { name = n; }
void User::set_last_name(const string &l) { last_name = l; }
void User::set_hashed_password(const string &p) { hashed_password = p; }
void User::print() const { cout << "User: " << name << " " << last_name << endl; }

Student::Student(int uid, string n, string l, string p, string sid, string e, string ph, float b)
    : User(uid, n, l, p), student_id(sid), email(e), phone(ph), balance(b), is_active(true) {}

void Student::print() const
{
    cout << "Student: " << get_name() << " - Email: " << email << " - Balance: " << balance << endl;
}

bool Student::get_is_active() const { return is_active; }
float Student::get_balance() const { return balance; }
vector<Reservation *> &Student::get_reserves() { return reserves; }

void Student::activate() { is_active = true; }
void Student::deactivate() { is_active = false; }

void Student::reserve_meal(Meal *meal, DiningHall *hall)
{
    for (auto res : reserves)
    {
        time_t res_time = res->get_created_at();
        tm *res_tm = localtime(&res_time);
        tm *now_tm = localtime(&(time_t){time(nullptr)});
        if (res_tm->tm_year == now_tm->tm_year &&
            res_tm->tm_mon == now_tm->tm_mon &&
            res_tm->tm_mday == now_tm->tm_mday &&
            res->get_meal()->get_meal_type() == meal->get_meal_type())
        {
            cout << "Meal already reserved today.\n";
            return;
        }
    }

    if (balance < meal->get_price())
    {
        cout << "Insufficient balance.\n";
        return;
    }

    balance -= meal->get_price();
    Reservation *r = new Reservation(reserves.size() + 1, this, hall, meal);
    reserves.push_back(r);
    cout << "Reservation successful.\n";
    r->print();
}

Admin::Admin(int uid, string n, string l, string p) : User(uid, n, l, p) {}
void Admin::print() const { cout << "Admin: " << get_name() << endl; }

Meal::Meal(int id, string n, float p, MealType t, ReserveDay d)
    : meal_id(id), name(n), price(p), is_active(true), meal_type(t), reserve_day(d) {}

void Meal::activate() { is_active = true; }
void Meal::deactivate() { is_active = false; }
void Meal::add_side_item(const string &item) { side_items.push_back(item); }
void Meal::print() const { cout << "Meal: " << name << " - Price: " << price << endl; }
float Meal::get_price() const { return price; }
MealType Meal::get_meal_type() const { return meal_type; }

DiningHall::DiningHall(int id, string n, string a, int c) : hall_id(id), name(n), address(a), capacity(c) {}
void DiningHall::print() const { cout << "Dining Hall: " << name << " - Capacity: " << capacity << endl; }

Reservation::Reservation(int id, Student *s, DiningHall *h, Meal *m)
    : reservation_id(id), student(s), dHall(h), meal(m), status(ReservationStatus::SUCCESS),
      created_at(time(nullptr)) {}

void Reservation::print() const
{
    cout << "Reservation #" << reservation_id << " - Status: ";
    if (status == ReservationStatus::SUCCESS)
        cout << "SUCCESS";
    else if (status == ReservationStatus::CANCELLED)
        cout << "CANCELLED";
    else
        cout << "FAILED";
    cout << " - Time: " << ctime(&created_at);
}

bool Reservation::cancel()
{
    if (status == ReservationStatus::CANCELLED)
    {
        cout << "Already cancelled.\n";
        return false;
    }
    status = ReservationStatus::CANCELLED;
    cout << "Cancelled successfully.\n";
    return true;
}

Meal *Reservation::get_meal() const { return meal; }
time_t Reservation::get_created_at() const { return created_at; }
ReservationStatus Reservation::get_status() const { return status; }

// ---------------- Main Function ---------------- //

int main()
{
    Panel panel;
    panel.showMenu();

    DiningHall *hall = new DiningHall(1, "Omid", "Birjand", 400);
    Meal *meal = new Meal(1, "Kebab", 20000, MealType::LUNCH, ReserveDay::MONDAY);

    Storage::getInstance().addDiningHall(hall);
    Storage::getInstance().addMeal(meal);

    Student student(1, "Behtash", "Rahimi", "hashed", "S101", "b@example.com", "0912", 30000);
    student.print();

    student.reserve_meal(meal, hall);
    panel.viewReservations(student);

    if (!student.get_reserves().empty())
    {
        student.get_reserves()[0]->cancel();
        student.get_reserves()[0]->print();
    }

    return 0;
}
