#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

enum class MealType { BREAKFAST, LUNCH, DINNER };
enum class ReserveDay { SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY };
enum class ReservationStatus { NOT_PAID, SUCCESS, CANCELLED, FAILED };
enum class TransactionType { TRANSFER, PAYMENT };
enum class TransactionStatus { PENDING, COMPLETED, FAILED };
enum class SessionStatus { AUTHENTICATED, ANONYMOUS };

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

class ShoppingCart
{
    vector<Reservation *> _reservations;

public:
    void addReservation(Reservation *reservation)
    {
        if (reservation)
            _reservations.push_back(reservation);
    }

    void removeReservation(int ID)
    {
        for (auto it = _reservations.begin(); it != _reservations.end(); ++it)
        {
            if ((*it)->get_status() == ReservationStatus::NOT_PAID &&
                (*it)->get_created_at() == ID)
            {
                _reservations.erase(it);
                cout << "Reservation removed from shopping cart." << endl;
                return;
            }
        }
        cout << "Reservation not found in shopping cart." << endl;
    }

    void viewShoppingCartItems() const
    {
        if (_reservations.empty())
        {
            cout << "Shopping cart is empty." << endl;
            return;
        }

        for (const auto &res : _reservations)
        {
            if (res && res->get_status() == ReservationStatus::NOT_PAID)
                res->print();
        }
    }

    void clear()
    {
        _reservations.clear();
        cout << "Shopping cart cleared." << endl;
    }

    vector<Reservation *> getReservations() const
    {
        return _reservations;
    }
};

class Storage
{
    vector<Meal *> meals;
    vector<DiningHall *> halls;
    vector<Student *> students;
    vector<Admin *> admins;

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

    void addStudent(Student *student)
    {
        students.push_back(student);
    }

    void addAdmin(Admin *admin)
    {
        admins.push_back(admin);
    }

    vector<Student *> getStudents() const
    {
        return students;
    }

    vector<Admin *> getAdmins() const
    {
        return admins;
    }
};

class Panel
{
public:
    void showMenu()
    {
        cout << "1. View Reservations\n2. Reserve Meal\n3. Cancel Reservation" << endl;
    }

    void viewReservations(Student &student)
    {
        for (auto res : student.get_reserves())
            res->print();
    }
};

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
        time_t now = time(nullptr);
        tm *now_tm = localtime(&now);
        if (res_tm->tm_year == now_tm->tm_year &&
            res_tm->tm_mon == now_tm->tm_mon &&
            res_tm->tm_mday == now_tm->tm_mday &&
            res->get_meal()->get_meal_type() == meal->get_meal_type())
        {
            cout << "Meal already reserved today." << endl;
            return;
        }
    }

    if (balance < meal->get_price())
    {
        cout << "Insufficient balance." << endl;
        return;
    }

    balance -= meal->get_price();
    Reservation *r = new Reservation(reserves.size() + 1, this, hall, meal);
    reserves.push_back(r);
    cout << "Reservation successful." << endl;
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
    else if (status == ReservationStatus::FAILED)
        cout << "FAILED";
    else
        cout << "NOT_PAID";
    cout << " - Time: " << ctime(&created_at);
}

bool Reservation::cancel()
{
    if (status == ReservationStatus::CANCELLED)
    {
        cout << "Already cancelled." << endl;
        return false;
    }
    status = ReservationStatus::CANCELLED;
    cout << "Cancelled successfully." << endl;
    return true;
}

Meal *Reservation::get_meal() const { return meal; }
time_t Reservation::get_created_at() const { return created_at; }
ReservationStatus Reservation::get_status() const { return status; }

class Transaction
{
    int _transactionID;
    string _trackingCode;
    float _amount;
    TransactionType _type;
    TransactionStatus _status;
    time_t _createdAt;

public:
    Transaction(int id, string track, float amt, TransactionType type, TransactionStatus status)
        : _transactionID(id), _trackingCode(track), _amount(amt), _type(type), _status(status), _createdAt(time(nullptr)) {}

    int getTransactionID() const { return _transactionID; }
    string getTrackingCode() const { return _trackingCode; }
    float getAmount() const { return _amount; }
    TransactionType getType() const { return _type; }
    TransactionStatus getStatus() const { return _status; }
    time_t getCreatedAt() const { return _createdAt; }

    void setTransactionID(int id) { _transactionID = id; }
    void setTrackingCode(const string &code) { _trackingCode = code; }
    void setAmount(float amt) { _amount = amt; }
    void setType(TransactionType t) { _type = t; }
    void setStatus(TransactionStatus s) { _status = s; }

    void print() const
    {
        cout << "Transaction #" << _transactionID << " - Amount: " << _amount << " - Status: ";
        if (_status == TransactionStatus::PENDING)
            cout << "PENDING";
        else if (_status == TransactionStatus::COMPLETED)
            cout << "COMPLETED";
        else
            cout << "FAILED";
        cout << " - Time: " << ctime(&_createdAt);
    }
};

class SessionBase
{
protected:
    time_t _createdAt;
    time_t _lastLogin;
    SessionStatus _status;

public:
    SessionBase() : _createdAt(time(nullptr)), _lastLogin(0), _status(SessionStatus::ANONYMOUS) {}

    virtual void load_session() = 0;
    virtual void save_session() = 0;
    virtual void login(string username, string password) = 0;
    virtual void logout() = 0;

    time_t getCreatedAt() const { return _createdAt; }
    time_t getLastLogin() const { return _lastLogin; }
    SessionStatus getStatus() const { return _status; }

    void setCreatedAt(time_t t) { _createdAt = t; }
    void setLastLogin(time_t t) { _lastLogin = t; }
    void setStatus(SessionStatus s) { _status = s; }
};

class StudentSession : public SessionBase
{
    Student *_student;

public:
    StudentSession(Student *student) : _student(student) {}

    void load_session() override {}
    void save_session() override {}
    void login(string username, string password) override {}
    void logout() override {}
};

class AdminSession : public SessionBase
{
    Admin *_admin;

public:
    AdminSession(Admin *admin) : _admin(admin) {}

    void load_session() override {}
    void save_session() override {}
    void login(string username, string password) override {}
    void logout() override {}
};

int main()
{
    return 0;
}
