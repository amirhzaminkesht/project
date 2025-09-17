#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

enum class MealType {BREAKFAST, LUNCH, DINNER};
enum class ReserveDay {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};
enum class ReservationStatus {NOT_PAID, SUCCESS, CANCELLED, FAILED};
enum class TransactionType {PAYMENT};
enum class TransactionStatus {PENDING, COMPLETED, FAILED};
enum class SessionStatus {AUTHENTICATED, ANONYMOUS};

class Reservation;
class Meal;
class DiningHall;
class Transaction;
class TransactionManager;

/* ==== User ==== */
class User
{
    int user_id;
    string name;
    string last_name;
    string password;
public:
    User(int, const string&, const string&, const string&);
    virtual ~User() = default;
    virtual void print() const;
    int get_user_id() const;
    string get_name() const;
    bool check_password(const string&) const;
};

/* ==== Student ==== */
class Student: public User
{
    string student_id;
    float balance;
    bool active;
    vector<Reservation*> reserves;
public:
    Student(int, const string&, const string&, const string&, const string&, float);
    void print() const override;
    float get_balance() const;
    void set_balance(float);
    void add_reserve(Reservation*);
    vector<Reservation*>& get_reserves();
    string get_sid() const;
};

/* ==== Admin ==== */
class Admin: public User
{
public:
    Admin(int, const string&, const string&, const string&);
    void print() const override;
};

/* ==== Meal ==== */
class Meal
{
    int id;
    string name;
    float price;
    MealType type;
    ReserveDay day;
    bool active;
public:
    Meal(int, const string&, float, MealType, ReserveDay);
    void print() const;
    int get_id() const;
    float get_price() const;
    string get_name() const;
};

/* ==== DiningHall ==== */
class DiningHall
{
    int id;
    string name;
    int capacity;
public:
    DiningHall(int, const string&, int);
    void print() const;
    int get_id() const;
    int get_capacity() const;
    void dec();
    string get_name() const;
};

/* ==== Reservation ==== */
class Reservation
{
    int id;
    Student* student;
    Meal* meal;
    DiningHall* hall;
    ReservationStatus status;
public:
    Reservation(int, Student*, Meal*, DiningHall*);
    void print() const;
    int get_id() const;
    Meal* get_meal() const;
    DiningHall* get_hall() const;
    ReservationStatus get_status() const;
    void set_status(ReservationStatus);
    bool cancel();
    Student* get_student() const;
};

/* ==== ShoppingCart ==== */
class ShoppingCart
{
    vector<Reservation*> items;
public:
    void add(Reservation*);
    void view();
    vector<Reservation*> get();
    void removeById(int);
    void clear();
    bool empty() const;
    Transaction* confirm(Student*, class TransactionManager&);
};

/* ==== Transaction ==== */
class Transaction
{
    int id;
    float amount;
    TransactionStatus status;
public:
    Transaction(int, float, TransactionStatus);
    void print() const;
    int get_id() const;
    float get_amount() const;
    TransactionStatus get_status() const;
};

/* ==== TransactionManager ==== */
class TransactionManager
{
    vector<Transaction*> txns;
public:
    ~TransactionManager();
    void add(Transaction*);
    void view();
    int nextId() const;
    vector<Transaction*> get();
};

/* ==== PaymentProcessor ==== */
class PaymentProcessor
{
    TransactionManager& tm;
public:
    PaymentProcessor(TransactionManager&);
    void process(Student*, ShoppingCart&);
};

/* ==== Storage ==== */
class Storage
{
    vector<Student*> students;
    vector<Meal*> meals;
    vector<DiningHall*> halls;
    vector<Reservation*> reservations;
    vector<Transaction*> transactions;
    int studentCounter, mealCounter, hallCounter, reservationCounter, transactionCounter;

    Storage():studentCounter(0), mealCounter(0), hallCounter(0), reservationCounter(0), transactionCounter(0) {}
public:
    static Storage& instance();
    void addStudent(Student*);
    void addMeal(Meal*);
    void addHall(DiningHall*);
    void addReservation(Reservation*);
    void addTransaction(Transaction*);

    vector<Student*>& getStudents();
    vector<Meal*>& getMeals();
    vector<DiningHall*>& getHalls();
    vector<Reservation*>& getReservations();
    vector<Transaction*>& getTransactions();

    int nextStudentId();
    int nextMealId();
    int nextHallId();
    int nextReservationId();
    int nextTransactionId();
};

/* ==== SessionBase ==== */
class SessionBase
{
protected:
    SessionStatus status;
public:
    SessionBase();
    virtual void login(const string&, const string&) = 0;
    virtual void logout() = 0;
};

/* ==== Student Session ==== */
namespace StudentSession
{
class SessionManager: public SessionBase
{
    Student* current;
    ShoppingCart cart;
public:
    static SessionManager& instance();
    void login(const string&, const string&) override;
    void logout() override;
    void load_session();
    void save_session();
    Student* get();
    void set(Student*);
    ShoppingCart& getCart();
};
}

/* ==== Admin Session ==== */
namespace AdminSession
{
class SessionManager: public SessionBase
{
    Admin* current;
public:
    static SessionManager& instance();
    void login(const string&, const string&) override;
    void logout() override;
    void load_session();
    void save_session();
    Admin* get();
    void set(Admin*);
};
}

/* ==== Panel ==== */
class Panel
{
    TransactionManager& tm;
    PaymentProcessor& pp;
public:
    Panel(TransactionManager&, PaymentProcessor&);
    void showMenu();
    void viewReservations(Student&);
    void addToShoppingCart(Student*, Meal*, DiningHall*);
    void viewShoppingCart(Student*);
    void confirmShoppingCart(Student*);
    void removeShoppingCartItem(Student*, int);
    void increaseBalance(Student*, float);
    void viewRecentTransactions();
    void cancelReservation(Student*, int);
};

/* ==== File Helpers ==== */
void saveStudent(Student*);
void saveReservation(Reservation*);
void saveTransaction(Transaction*);
void log(const string&);

/* ==== Implementations ==== */
User::User(int i, const string& n, const string& l, const string& p):
    user_id(i), name(n), last_name(l), password(p) {}

void User::print() const
{cout<<"User "<<name<<" "<<last_name<<"\n";}

int User::get_user_id() const {return user_id;}

string User::get_name() const {return name;}

bool User::check_password(const string& p) const {return p==password;}

Student::Student(int i, const string& n, const string& l, const string& p, const string& sid, float b):
    User(i, n, l, p), student_id(sid), balance(b), active(true){}

void Student::print() const
{cout << "Student " << get_name() << " balance=" << balance << " active=" << (active?"yes":"no") << "\n";}

float Student::get_balance() const {return balance;}

void Student::set_balance(float b) {balance=b;}

void Student::add_reserve(Reservation* r) {reserves.push_back(r);}

vector<Reservation*>& Student::get_reserves() {return reserves;}

string Student::get_sid() const {return student_id;}

Admin::Admin(int i, const string& n, const string& l, const string& p):User(i, n, l, p) {}

void Admin::print() const {cout << "Admin " << get_name() << "\n";}

Meal::Meal(int i, const string& n, float p, MealType t, ReserveDay d):
    id(i), name(n), price(p), type(t), day(d), active(true) {}

void Meal::print() const {cout << "Meal " << id << " " << name << " price=" << price << "\n";}

int Meal::get_id() const {return id;}

float Meal::get_price() const {return price;}

string Meal::get_name() const {return name;}

DiningHall::DiningHall(int i, const string& n, int c):
    id(i), name(n), capacity(c) {}

void DiningHall::print() const
{cout << "Hall " << id << " "<< name << " cap=" << capacity << "\n";}

int DiningHall::get_id() const {return id;}

int DiningHall::get_capacity() const {return capacity;}

void DiningHall::dec() {if(capacity > 0) capacity--;}

string DiningHall::get_name() const {return name;}

Reservation::Reservation(int i, Student* s, Meal* m, DiningHall* h):
    id(i), student(s), meal(m), hall(h), status(ReservationStatus::NOT_PAID) {}

void Reservation::print() const
    {cout << "Res#" << id << " stu=" << student->get_user_id() << " meal=" << meal->get_id()
    << " hall=" << hall->get_id() << " status=" << (int)status << "\n";}

int Reservation::get_id() const {return id;}

Meal* Reservation::get_meal() const {return meal;}

DiningHall* Reservation::get_hall() const {return hall;}

ReservationStatus Reservation::get_status() const {return status;}

void Reservation::set_status(ReservationStatus s) {status=s;}

bool Reservation::cancel()
    {if(status == ReservationStatus::SUCCESS)
        {status = ReservationStatus::CANCELLED; return true;}
    return false;}

Student* Reservation::get_student() const {return student;}

void ShoppingCart::add(Reservation* r) {items.push_back(r);}

void ShoppingCart::view()
    {if(items.empty())
        {cout<<"Cart empty\n"; return;}
    for(auto r:items) r->print();}

vector<Reservation*> ShoppingCart::get() {return items;}
void ShoppingCart::removeById(int id)
    {items.erase(remove_if(items.begin(), items.end(), [id](Reservation* r){return r->get_id() == id;}), items.end());}

void ShoppingCart::clear() {items.clear();}

bool ShoppingCart::empty() const {return items.empty();}

Transaction* ShoppingCart::confirm(Student* s, TransactionManager& tm)
    {if(!s) return nullptr;
    float total = 0;
    for(auto r: items)
        if(r->get_status() == ReservationStatus::NOT_PAID)
            total += r->get_meal()->get_price();

    if(s->get_balance() < total)
        {cout << "Not enough balance\n";
        for (auto r: items) r->set_status(ReservationStatus::FAILED);
        return nullptr;}
    for(auto r: items)
        {s->set_balance(s->get_balance() - r->get_meal()->get_price());
        r->set_status(ReservationStatus::SUCCESS);
        s->add_reserve(r);}
    Transaction* t = new Transaction(tm.nextId(), total, TransactionStatus::COMPLETED);
    tm.add(t);
    clear();
    return t;}

void StudentSession::SessionManager::load_session()
    {ifstream f("student.session");
    if(!f) return;
    string id;
    getline(f, id);
    f.close();
    if(!id.empty())
        cout << "Restored student session id=" << id << "\n";}

void StudentSession::SessionManager::save_session()
    {ofstream f("student.session");
    if(current) f << current->get_user_id();
    f.close();}

void AdminSession::SessionManager::load_session()
    {ifstream f("admin.session");
    if(!f) return;
    string id;
    getline(f, id);
    f.close();
    if(!id.empty())
        cout << "Restored admin session id=" << id << "\n";}

void AdminSession::SessionManager::save_session()
    {ofstream f("admin.session");
    if(current) f << current->get_user_id();
    f.close();}

void Panel::viewRecentTransactions()
    {cout << "Recent Transactions:\n";
    tm.view();}

void Panel::cancelReservation(Student* s,int id)
    {if(!s) return;
    for(auto r: s->get_reserves())
        {if(r->get_id() == id)
            {if(r->cancel())
                cout << "Reservation " << id << " cancelled\n";
            else
                cout << "Reservation " << id << " not cancellable\n";
        return;}}
    cout << "Reservation not found\n";}

Transaction::Transaction(int i, float a, TransactionStatus s):
    id(i), amount(a), status(s) {}

void Transaction::print() const
    {cout << "Txn#" << id << " amt=" << amount << " status=" << (int)status << "\n";}

int Transaction::get_id() const {return id;}

float Transaction::get_amount() const {return amount;}

TransactionStatus Transaction::get_status() const {return status;}

TransactionManager::~TransactionManager() {for(auto t:txns) delete t;}

void TransactionManager::add(Transaction* t) {txns.push_back(t);}

void TransactionManager::view() {for(auto t:txns) t->print();}

int TransactionManager::nextId() const {return txns.size()+1;}

vector<Transaction*> TransactionManager::get() {return txns;}

PaymentProcessor::PaymentProcessor(TransactionManager& t):
    tm(t) {}

void PaymentProcessor::process(Student* s, ShoppingCart& cart)
    {if (!s) return;
    float total = 0.0f;
    for(auto r : cart.get())
        {if(r && r->get_status() == ReservationStatus::NOT_PAID)
            total += r->get_meal()->get_price();}
    if(s->get_balance() < total)
        {cout << "Not enough balance\n";
        for(auto r : cart.get())
            if (r) r->set_status(ReservationStatus::FAILED);
        log("Payment failed");
        return;}
    for(auto r : cart.get())
        {if (!r) continue;
        s->set_balance(s->get_balance() - r->get_meal()->get_price());
        r->set_status(ReservationStatus::SUCCESS);
        s->add_reserve(r);
        Transaction* t = new Transaction(tm.nextId(), r->get_meal()->get_price(), TransactionStatus::COMPLETED);
        tm.add(t);
        saveReservation(r);
        saveTransaction(t);
        log("Reservation paid id=" + to_string(r->get_id()));}
    cart.clear();
    saveStudent(s);}

/* ==== Storage Implementation ==== */
Storage& Storage::instance()
{static Storage s; return s;}

void Storage::addStudent(Student* s){students.push_back(s);}

void Storage::addMeal(Meal* m) {meals.push_back(m);}

void Storage::addHall(DiningHall* h) {halls.push_back(h);}

void Storage::addReservation(Reservation* r) {reservations.push_back(r);}

void Storage::addTransaction(Transaction* t) {transactions.push_back(t);}

vector<Student*>& Storage::getStudents() {return students;}

vector<Meal*>& Storage::getMeals() {return meals;}

vector<DiningHall*>& Storage::getHalls() {return halls;}

vector<Reservation*>& Storage::getReservations() {return reservations;}

vector<Transaction*>& Storage::getTransactions() {return transactions;}

int Storage::nextStudentId() {return ++studentCounter;}

int Storage::nextMealId() {return ++mealCounter;}

int Storage::nextHallId() {return ++hallCounter;}

int Storage::nextReservationId() {return ++reservationCounter;}

int Storage::nextTransactionId() {return ++transactionCounter;}

SessionBase::SessionBase():status(SessionStatus::ANONYMOUS) {}

StudentSession::SessionManager& StudentSession::SessionManager::instance()
    {static SessionManager s; return s;}

void StudentSession::SessionManager::login(const string& u, const string& p)
    {cout << "Student " << u << " logged in\n"; status = SessionStatus::AUTHENTICATED; log("Student " + u + " login");}

void StudentSession::SessionManager::logout()
    {current = nullptr; status = SessionStatus::ANONYMOUS; log("Student logout");}

Student* StudentSession::SessionManager::get() {return current;}

void StudentSession::SessionManager::set(Student* s) {current=s;}

AdminSession::SessionManager& AdminSession::SessionManager::instance()
    {static SessionManager s;
    return s;}

void AdminSession::SessionManager::login(const string& u, const string& p)
    {cout << "Admin " << u << " logged in\n";
    status = SessionStatus::AUTHENTICATED;
    log("Admin " + u + " login");}

void AdminSession::SessionManager::logout()
    {current = nullptr;
    status = SessionStatus::ANONYMOUS;
    log("Admin logout");}

Admin* AdminSession::SessionManager::get() {return current;}

void AdminSession::SessionManager::set(Admin* a) {current = a;}

ShoppingCart& StudentSession::SessionManager::getCart() {return cart;}

Panel::Panel(TransactionManager& t, PaymentProcessor& p):
    tm(t), pp(p) {}

void Panel::showMenu()
    {cout << "1. View Reservations\n"
         << "2. Add Reservation to Cart\n"
         << "3. View Shopping Cart\n"
         << "4. Confirm Shopping Cart (Pay)\n"
         << "5. Remove Item from Cart\n"
         << "6. View Recent Transactions\n"
         << "7. Increase Balance\n"
         << "8. Cancel Reservation\n"
         << "9. Exit\n";}

void Panel::viewReservations(Student& s)
    {cout << "Reservations for " << s.get_name() << ":\n";
    if(s.get_reserves().empty())
        cout << "No past reservations\n";
    else for(auto r: s.get_reserves()) r->print();}

void Panel::addToShoppingCart(Student* s, Meal* m, DiningHall* h)
    {if(!s || !m || !h) return;
    Reservation* r = new Reservation(rand()%10000, s, m, h);
    StudentSession::SessionManager::instance().getCart().add(r);}

void Panel::viewShoppingCart(Student* s)
    {StudentSession::SessionManager::instance().getCart().view();}

void Panel::confirmShoppingCart(Student* s)
    {pp.process(s, StudentSession::SessionManager::instance().getCart());}

void Panel::removeShoppingCartItem(Student* s, int id)
    {StudentSession::SessionManager::instance().getCart().removeById(id);}

void Panel::increaseBalance(Student* s, float a)
    {s->set_balance(s->get_balance() + a);
    cout << "Balance increased: " << s->get_balance() << "\n";}

/* ==== File I/O ==== */
void saveStudent(Student* s)
    {ofstream f("students.csv", ios::app);
    f<<s->get_user_id() << "," << s->get_name() << "," << s->get_sid() << "," << s->get_balance() << "\n";}

void saveReservation(Reservation* r)
    {ofstream f("reservations.csv", ios::app);
    f<<r->get_id() << "," << r->get_student()->get_user_id()
    << "," << r->get_meal()->get_name() << ","<<r->get_hall()->get_name()
    << "," << (int)r->get_status() << "\n";}

void saveTransaction(Transaction* t)
    {ofstream f("transactions.csv", ios::app);
    f << t->get_id() << "," << t->get_amount() << "," << (int)t->get_status() << "\n";}

void log(const string& msg)
    {ofstream f("app.log", ios::app);
    time_t now=time(0);
    f << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << " " << msg << "\n";}

/* ==== Main ==== */
int main()
{
    Storage& db = Storage::instance();

    Student* s1 = new Student(db.nextStudentId(), "Ali", "Ahmadi", "1234", "981234", 100.0f);
    Meal* m1 = new Meal(db.nextMealId(), "Kabab", 25.0f, MealType::LUNCH, ReserveDay::SUNDAY);
    Meal* m2 = new Meal(db.nextMealId(), "Stew", 20.0f, MealType::DINNER, ReserveDay::MONDAY);
    DiningHall* h1 = new DiningHall(db.nextHallId(), "Main Hall", 10);

    db.addStudent(s1);
    db.addMeal(m1);
    db.addMeal(m2);
    db.addHall(h1);

    TransactionManager tm;
    PaymentProcessor pp(tm);
    Panel panel(tm, pp);

    StudentSession::SessionManager::instance().set(s1);

    cout << "\n--- MENU TEST START ---\n";

    panel.viewReservations(*s1);

    panel.addToShoppingCart(s1, m1, h1);
    panel.addToShoppingCart(s1, m2, h1);

    cout << "\nShopping Cart:\n";
    panel.viewShoppingCart(s1);

    cout << "\nConfirming cart...\n";
    panel.confirmShoppingCart(s1);

    cout << "\nReservations after payment:\n";
    panel.viewReservations(*s1);

    cout << "\nTransactions:\n";
    panel.viewRecentTransactions();

    panel.increaseBalance(s1, 50);

    cout << "\nCancelling first reservation...\n";
    if(!s1->get_reserves().empty())
        panel.cancelReservation(s1, s1->get_reserves()[0]->get_id());

    cout << "\nReservations after cancel:\n";
    panel.viewReservations(*s1);

    cout << "\n--- MENU TEST END ---\n";

    return 0;
}

