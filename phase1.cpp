#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <memory>
#include <algorithm>

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

class User {
    int user_id;
    string name;
    string last_name;
    string hashed_password;
public:
    User(int, const string&, const string&, const string&);
    virtual ~User() = default;
    virtual void print() const;
    int get_user_id() const;
    string get_name() const;
    string get_last_name() const;
    string get_hashed_password() const;
    void set_user_id(int);
    void set_name(const string&);
    void set_last_name(const string&);
    void set_hashed_password(const string&);
};

class ShoppingCart;
class TransactionManager;

class Student : public User {
    string student_id;
    string email;
    string phone;
    float balance;
    bool is_active;
    vector<Reservation*> reserves;
public:
    Student(int, const string&, const string&, const string&,
            const string&, const string&, const string&, float);
    void print() const override;
    Reservation* reserve_meal(Meal*, DiningHall*);
    void activate();
    void deactivate();
    bool get_is_active() const;
    float get_balance() const;
    void set_balance(float);
    vector<Reservation*>& get_reserves();
};

class Admin : public User {
public:
    Admin(int, const string&, const string&, const string&);
    void print() const override;
};

class Meal {
    int meal_id;
    string name;
    float price;
    bool is_active;
    MealType meal_type;
    ReserveDay reserve_day;
    vector<string> side_items;
public:
    Meal(int, const string&, float, MealType, ReserveDay);
    void activate();
    void deactivate();
    void add_side_item(const string&);
    void print() const;
    float get_price() const;
    MealType get_meal_type() const;
    int get_meal_id() const;
};

class DiningHall {
    int hall_id;
    string name;
    string address;
    int capacity;
public:
    DiningHall(int, const string&, const string&, int);
    void print() const;
    int get_hall_id() const;
};

class Reservation {
    int reservation_id;
    Student* student;
    DiningHall* dHall;
    Meal* meal;
    ReservationStatus status;
    time_t created_at;
public:
    Reservation(int, Student*, DiningHall*, Meal*);
    void print() const;
    bool cancel();
    Meal* get_meal() const;
    time_t get_created_at() const;
    ReservationStatus get_status() const;
    void set_status(ReservationStatus s);
    int get_reservation_id() const;
    Student* get_student() const;
    DiningHall* get_dhall() const;
};

class ShoppingCart {
    vector<Reservation*> _reservations;
public:
    void addReservation(Reservation* reservation);
    void removeReservationById(int ID);
    void viewShoppingCartItems() const;
    void clear();
    vector<Reservation*> getReservations() const;
    bool empty() const;
};

class Storage {
    vector<Meal*> meals;
    vector<DiningHall*> halls;
    vector<Student*> students;
    vector<Admin*> admins;
    int mealIDCounter = 0;
    int diningHallIDCounter = 0;
    int studentIDCounter = 0;
    int adminIDCounter = 0;
    Storage() = default;
public:
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    static Storage& getInstance();
    int nextMealID();
    int nextDiningHallID();
    int nextStudentID();
    int nextAdminID();
    void addMeal(Meal* meal);
    void addDiningHall(DiningHall* hall);
    void addStudent(Student* student);
    void addAdmin(Admin* admin);
    vector<Student*> getStudents() const;
    vector<Admin*> getAdmins() const;
    vector<Meal*> getMeals() const;
    vector<DiningHall*> getDiningHalls() const;
};

class Transaction {
    int _transactionID;
    string _trackingCode;
    float _amount;
    TransactionType _type;
    TransactionStatus _status;
    time_t _createdAt;
public:
    Transaction(int id, const string& track, float amt,
                TransactionType type, TransactionStatus status);
    int getTransactionID() const;
    string getTrackingCode() const;
    float getAmount() const;
    TransactionType getType() const;
    TransactionStatus getStatus() const;
    time_t getCreatedAt() const;
    void setStatus(TransactionStatus s);
    void print() const;
};

class TransactionManager {
    vector<Transaction*> _transactions;
public:
    ~TransactionManager();
    void addTransaction(Transaction* transaction);
    void viewAllTransactions() const;
    Transaction* findTransactionByID(int id) const;
    vector<Transaction*> getAllTransactions() const;
    int nextTransactionID() const;
};

class PaymentProcessor {
    TransactionManager& _txnManager;
public:
    PaymentProcessor(TransactionManager& manager);
    void processCart(Student* student, ShoppingCart& cart);
};

class SessionBase {
protected:
    time_t _createdAt;
    time_t _lastLogin;
    SessionStatus _status;
public:
    SessionBase();
    virtual ~SessionBase() = default;
    virtual void load_session() = 0;
    virtual void save_session() = 0;
    virtual void login(const string& username, const string& password) = 0;
    virtual void logout() = 0;
    time_t getCreatedAt() const;
    time_t getLastLogin() const;
    SessionStatus getStatus() const;
    void setCreatedAt(time_t t);
    void setLastLogin(time_t t);
    void setStatus(SessionStatus s);
};

namespace StudentSession {
class SessionManager : public SessionBase {
    Student* _currentStudent = nullptr;
    ShoppingCart _shopping_cart;
    SessionManager() = default;
public:
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;
    static SessionManager& instance();
    void load_session() override;
    void save_session() override;
    void login(const string& username, const string& password) override;
    void logout() override;
    Student* currentStudent();
    ShoppingCart& shoppingCart();
};
}

namespace AdminSession {
class SessionManager : public SessionBase {
    Admin* _currentAdmin = nullptr;
    SessionManager() = default;
public:
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;
    static SessionManager& instance();
    void load_session() override;
    void save_session() override;
    void login(const string& username, const string& password) override;
    void logout() override;
    Admin* currentAdmin();
};
}

class Panel {
    TransactionManager& _txnManager;
    PaymentProcessor& _paymentProcessor;
public:
    Panel(TransactionManager& tm, PaymentProcessor& pp);
    void showMenu();
    void viewReservations(Student& student);
    void addToShoppingCart(Student* student, Meal* meal, DiningHall* hall);
    void viewShoppingCart(Student* student);
    void confirmShoppingCart(Student* student);
    void removeShoppingCartItem(Student* student, int reservationId);
    void increaseBalance(Student* student, float amount);
    void viewRecentTransactions();
    void cancelReservation(Student* student, int reservationId);
};

/* ==== implementations ==== */

User::User(int uid, const string& n, const string& l, const string& p)
    : user_id(uid), name(n), last_name(l), hashed_password(p) {}

int User::get_user_id() const { return user_id; }
string User::get_name() const { return name; }
string User::get_last_name() const { return last_name; }
string User::get_hashed_password() const { return hashed_password; }
void User::set_user_id(int id) { user_id = id; }
void User::set_name(const string& n) { name = n; }
void User::set_last_name(const string& l) { last_name = l; }
void User::set_hashed_password(const string& p) { hashed_password = p; }
void User::print() const {
    cout << "User: " << name << " " << last_name << "\n";
}

Student::Student(int uid, const string& n, const string& l,
                 const string& p, const string& sid,
                 const string& e, const string& ph, float b)
    : User(uid, n, l, p), student_id(sid), email(e),
      phone(ph), balance(b), is_active(true) {}

void Student::print() const {
    cout << "Student: " << get_name()
         << " - Email: " << email
         << " - Balance: " << balance
         << " - Active: " << (is_active ? "Yes" : "No")
         << "\n";
}

Reservation* Student::reserve_meal(Meal* meal, DiningHall* hall) {
    static int resCounter = 1;
    Reservation* r = new Reservation(resCounter++, this, hall, meal);
    r->set_status(ReservationStatus::NOT_PAID);
    cout << "Created NOT_PAID reservation id="
         << r->get_reservation_id() << "\n";
    return r;
}

void Student::activate() { is_active = true; }
void Student::deactivate() { is_active = false; }
bool Student::get_is_active() const { return is_active; }
float Student::get_balance() const { return balance; }
void Student::set_balance(float b) { balance = b; }
vector<Reservation*>& Student::get_reserves() { return reserves; }

Admin::Admin(int uid, const string& n, const string& l, const string& p)
    : User(uid, n, l, p) {}

void Admin::print() const {
    cout << "Admin: " << get_name() << "\n";
}

Meal::Meal(int id, const string& n, float p, MealType t, ReserveDay d)
    : meal_id(id), name(n), price(p),
      is_active(true), meal_type(t), reserve_day(d) {}

void Meal::activate() { is_active = true; }
void Meal::deactivate() { is_active = false; }
void Meal::add_side_item(const string& item) {
    side_items.push_back(item);
}
void Meal::print() const {
    cout << "Meal: " << name
         << " (id=" << meal_id << ") - Price: "
         << price << "\n";
}
float Meal::get_price() const { return price; }
MealType Meal::get_meal_type() const { return meal_type; }
int Meal::get_meal_id() const { return meal_id; }

DiningHall::DiningHall(int id, const string& n, const string& a, int c)
    : hall_id(id), name(n), address(a), capacity(c) {}

void DiningHall::print() const {
    cout << "Dining Hall: " << name
         << " (id=" << hall_id << ") - Capacity: "
         << capacity << "\n";
}
int DiningHall::get_hall_id() const { return hall_id; }

Reservation::Reservation(int id, Student* s, DiningHall* h, Meal* m)
    : reservation_id(id), student(s), dHall(h),
      meal(m), status(ReservationStatus::NOT_PAID),
      created_at(time(nullptr)) {}

void Reservation::print() const {
    cout << "Reservation #" << reservation_id
         << " - Student: "
         << (student ? student->get_name() : "N/A")
         << " - Meal: "
         << (meal ? to_string(meal->get_meal_id()) : string("N/A"))
         << " - Status: ";
    if (status == ReservationStatus::SUCCESS) {
        cout << "SUCCESS";
    } else if (status == ReservationStatus::CANCELLED) {
        cout << "CANCELLED";
    } else if (status == ReservationStatus::FAILED) {
        cout << "FAILED";
    } else {
        cout << "NOT_PAID";
    }
    cout << " - Time: " << ctime(&created_at);
}

bool Reservation::cancel() {
    if (status == ReservationStatus::CANCELLED) {
        cout << "Already cancelled.\n";
        return false;
    }
    status = ReservationStatus::CANCELLED;
    cout << "Cancelled successfully.\n";
    return true;
}

Meal* Reservation::get_meal() const { return meal; }
time_t Reservation::get_created_at() const { return created_at; }
ReservationStatus Reservation::get_status() const { return status; }
void Reservation::set_status(ReservationStatus s) { status = s; }
int Reservation::get_reservation_id() const { return reservation_id; }
Student* Reservation::get_student() const { return student; }
DiningHall* Reservation::get_dhall() const { return dHall; }

void ShoppingCart::addReservation(Reservation* reservation) {
    if (reservation) {
        _reservations.push_back(reservation);
        cout << "Added reservation (id="
             << reservation->get_reservation_id()
             << ") to shopping cart.\n";
    }
}

void ShoppingCart::removeReservationById(int ID) {
    auto it = find_if(_reservations.begin(),
                      _reservations.end(),
                      [ID](Reservation* r) {
                          return r && r->get_reservation_id() == ID;
                      });
    if (it != _reservations.end()) {
        cout << "Removed reservation id="
             << (*it)->get_reservation_id()
             << " from shopping cart.\n";
        _reservations.erase(it);
    } else {
        cout << "Reservation not found in shopping cart.\n";
    }
}

void ShoppingCart::viewShoppingCartItems() const {
    if (_reservations.empty()) {
        cout << "Shopping cart is empty.\n";
        return;
    }
    cout << "Shopping cart items:\n";
    for (const auto& res : _reservations) {
        if (res) res->print();
    }
}

void ShoppingCart::clear() {
    _reservations.clear();
    cout << "Shopping cart cleared.\n";
}

vector<Reservation*> ShoppingCart::getReservations() const {
    return _reservations;
}

bool ShoppingCart::empty() const { return _reservations.empty(); }

Storage& Storage::getInstance() {
    static Storage instance;
    return instance;
}

int Storage::nextMealID() { return ++mealIDCounter; }
int Storage::nextDiningHallID() { return ++diningHallIDCounter; }
int Storage::nextStudentID() { return ++studentIDCounter; }
int Storage::nextAdminID() { return ++adminIDCounter; }

void Storage::addMeal(Meal* meal) { meals.push_back(meal); }
void Storage::addDiningHall(DiningHall* hall) { halls.push_back(hall); }
void Storage::addStudent(Student* student) { students.push_back(student); }
void Storage::addAdmin(Admin* admin) { admins.push_back(admin); }

vector<Student*> Storage::getStudents() const { return students; }
vector<Admin*> Storage::getAdmins() const { return admins; }
vector<Meal*> Storage::getMeals() const { return meals; }
vector<DiningHall*> Storage::getDiningHalls() const { return halls; }

Transaction::Transaction(int id, const string& track, float amt,
                         TransactionType type, TransactionStatus status)
    : _transactionID(id), _trackingCode(track), _amount(amt),
      _type(type), _status(status), _createdAt(time(nullptr)) {}

int Transaction::getTransactionID() const { return _transactionID; }
string Transaction::getTrackingCode() const { return _trackingCode; }
float Transaction::getAmount() const { return _amount; }
TransactionType Transaction::getType() const { return _type; }
TransactionStatus Transaction::getStatus() const { return _status; }
time_t Transaction::getCreatedAt() const { return _createdAt; }
void Transaction::setStatus(TransactionStatus s) { _status = s; }

void Transaction::print() const {
    cout << "Transaction #" << _transactionID
         << " - Amount: " << _amount
         << " - Status: ";
    if (_status == TransactionStatus::PENDING) {
        cout << "PENDING";
    } else if (_status == TransactionStatus::COMPLETED) {
        cout << "COMPLETED";
    } else {
        cout << "FAILED";
    }
    cout << " - Time: " << ctime(&_createdAt);
}

TransactionManager::~TransactionManager() {
    for (auto t : _transactions) delete t;
}

void TransactionManager::addTransaction(Transaction* transaction) {
    if (transaction) _transactions.push_back(transaction);
}

void TransactionManager::viewAllTransactions() const {
    if (_transactions.empty()) {
        cout << "No transactions found.\n";
        return;
    }
    for (const auto& txn : _transactions) {
        if (txn) txn->print();
    }
}

Transaction* TransactionManager::findTransactionByID(int id) const {
    for (const auto& txn : _transactions) {
        if (txn && txn->getTransactionID() == id) return txn;
    }
    return nullptr;
}

vector<Transaction*> TransactionManager::getAllTransactions() const {
    return _transactions;
}

int TransactionManager::nextTransactionID() const {
    return static_cast<int>(_transactions.size()) + 1;
}

PaymentProcessor::PaymentProcessor(TransactionManager& manager)
    : _txnManager(manager) {}

void PaymentProcessor::processCart(Student* student, ShoppingCart& cart) {
    if (!student) {
        cout << "Invalid student.\n";
        return;
    }

    auto reservations = cart.getReservations();
    if (reservations.empty()) {
        cout << "Shopping cart is empty.\n";
        return;
    }

    float total = 0.0f;
    for (auto r : reservations) {
        if (r && r->get_status() == ReservationStatus::NOT_PAID) {
            total += r->get_meal()->get_price();
        }
    }

    if (student->get_balance() < total) {
        cout << "Insufficient balance to process all reservations. "
             << "Needed: " << total
             << " - Available: " << student->get_balance()
             << "\n";
        for (auto r : reservations) {
            if (r && r->get_status() == ReservationStatus::NOT_PAID) {
                r->set_status(ReservationStatus::FAILED);
            }
        }
        return;
    }

    for (auto r : reservations) {
        if (!r) continue;
        if (r->get_status() != ReservationStatus::NOT_PAID) continue;

        float price = r->get_meal()->get_price();
        float new_balance = student->get_balance() - price;
        student->set_balance(new_balance);

        r->set_status(ReservationStatus::SUCCESS);

        int txnID = _txnManager.nextTransactionID();
        string tracking = "TRX" + to_string(txnID)
                          + "-" + to_string(time(nullptr));
        Transaction* txn = new Transaction(
            txnID, tracking, price,
            TransactionType::PAYMENT,
            TransactionStatus::COMPLETED
        );
        _txnManager.addTransaction(txn);

        student->get_reserves().push_back(r);

        cout << "Payment successful for reservation id="
             << r->get_reservation_id() << ".\n";
        txn->print();
    }

    cart.clear();
}

/* SessionBase */
SessionBase::SessionBase()
    : _createdAt(time(nullptr)),
      _lastLogin(0),
      _status(SessionStatus::ANONYMOUS) {}

time_t SessionBase::getCreatedAt() const { return _createdAt; }
time_t SessionBase::getLastLogin() const { return _lastLogin; }
SessionStatus SessionBase::getStatus() const { return _status; }
void SessionBase::setCreatedAt(time_t t) { _createdAt = t; }
void SessionBase::setLastLogin(time_t t) { _lastLogin = t; }
void SessionBase::setStatus(SessionStatus s) { _status = s; }

/* StudentSession implementations */
namespace StudentSession {

SessionManager& SessionManager::instance() {
    static SessionManager inst;
    return inst;
}

void SessionManager::load_session() {}
void SessionManager::save_session() {}

void SessionManager::login(const string& username, const string& password) {
    auto& st = Storage::getInstance();
    for (auto s : st.getStudents()) {
        if (s && s->get_name() == username) {
            _currentStudent = s;
            _status = SessionStatus::AUTHENTICATED;
            _lastLogin = time(nullptr);
            cout << "Student '" << username << "' logged in.\n";
            return;
        }
    }
    cout << "Student login failed for username: " << username << "\n";
}

void SessionManager::logout() {
    if (_currentStudent) {
        cout << "Student '" << _currentStudent->get_name()
             << "' logged out.\n";
    }
    _currentStudent = nullptr;
    _status = SessionStatus::ANONYMOUS;
}

Student* SessionManager::currentStudent() { return _currentStudent; }
ShoppingCart& SessionManager::shoppingCart() { return _shopping_cart; }

} // namespace StudentSession

/* AdminSession implementations */
namespace AdminSession {

SessionManager& SessionManager::instance() {
    static SessionManager inst;
    return inst;
}

void SessionManager::load_session() {}
void SessionManager::save_session() {}

void SessionManager::login(const string& username, const string& password) {
    auto& st = Storage::getInstance();
    for (auto a : st.getAdmins()) {
        if (a && a->get_name() == username) {
            _currentAdmin = a;
            _status = SessionStatus::AUTHENTICATED;
            _lastLogin = time(nullptr);
            cout << "Admin '" << username << "' logged in.\n";
            return;
        }
    }
    cout << "Admin login failed for username: " << username << "\n";
}

void SessionManager::logout() {
    if (_currentAdmin) {
        cout << "Admin '" << _currentAdmin->get_name()
             << "' logged out.\n";
    }
    _currentAdmin = nullptr;
    _status = SessionStatus::ANONYMOUS;
}

Admin* SessionManager::currentAdmin() { return _currentAdmin; }

} // namespace AdminSession

Panel::Panel(TransactionManager& tm, PaymentProcessor& pp)
    : _txnManager(tm), _paymentProcessor(pp) {}

void Panel::showMenu() {
    cout << "1. View Reservations\n"
         << "2. Add Reservation to Cart\n"
         << "3. View Shopping Cart\n"
         << "4. Confirm Shopping Cart (Pay)\n"
         << "5. Remove Item from Cart\n"
         << "6. View Recent Transactions\n"
         << "7. Increase Balance\n"
         << "8. Cancel Reservation (history)\n"
         << "9. Exit\n";
}

void Panel::viewReservations(Student& student) {
    cout << "Reservations for student: " << student.get_name() << "\n";
    if (student.get_reserves().empty()) {
        cout << "No past reservations.\n";
        return;
    }
    for (auto res : student.get_reserves()) {
        if (res) res->print();
    }
}

void Panel::addToShoppingCart(Student* student, Meal* meal, DiningHall* hall) {
    if (!student || !meal || !hall) {
        cout << "Invalid parameters for adding to cart.\n";
        return;
    }

    static int tempResCounter = 100000;
    Reservation* r = new Reservation(++tempResCounter, student, hall, meal);
    r->set_status(ReservationStatus::NOT_PAID);

    auto& sess = StudentSession::SessionManager::instance();
    if (sess.currentStudent() != student) {
        cout << "Student not logged in. Logging in automatically.\n";
        sess.login(student->get_name(), "");
    }
    sess.shoppingCart().addReservation(r);
}

void Panel::viewShoppingCart(Student* student) {
    auto& sess = StudentSession::SessionManager::instance();
    if (sess.currentStudent() != student) {
        cout << "No active session for this student.\n";
        return;
    }
    sess.shoppingCart().viewShoppingCartItems();
}

void Panel::confirmShoppingCart(Student* student) {
    auto& sess = StudentSession::SessionManager::instance();
    if (sess.currentStudent() != student) {
        cout << "No active session for this student.\n";
        return;
    }
    _paymentProcessor.processCart(student, sess.shoppingCart());
}

void Panel::removeShoppingCartItem(Student* student, int reservationId) {
    auto& sess = StudentSession::SessionManager::instance();
    if (sess.currentStudent() != student) {
        cout << "No active session for this student.\n";
        return;
    }
    sess.shoppingCart().removeReservationById(reservationId);
}

void Panel::increaseBalance(Student* student, float amount) {
    if (!student) {
        cout << "Invalid student.\n";
        return;
    }
    if (amount <= 0) {
        cout << "Amount must be positive.\n";
        return;
    }
    float newb = student->get_balance() + amount;
    student->set_balance(newb);
    cout << "Balance increased. New balance: " << student->get_balance() << "\n";
}

void Panel::viewRecentTransactions() { _txnManager.viewAllTransactions(); }

void Panel::cancelReservation(Student* student, int reservationId) {
    for (auto res : student->get_reserves()) {
        if (res && res->get_reservation_id() == reservationId) {
            res->cancel();
            return;
        }
    }
    cout << "Reservation not found in student's history.\n";
}

int main() {
    Storage& storage = Storage::getInstance();

    TransactionManager txnManager;
    PaymentProcessor paymentProcessor(txnManager);
    Panel panel(txnManager, paymentProcessor);

    DiningHall* dh1 = new DiningHall(storage.nextDiningHallID(),
                                    "North Hall", "North St.", 200);
    DiningHall* dh2 = new DiningHall(storage.nextDiningHallID(),
                                    "South Hall", "South Ave.", 150);
    storage.addDiningHall(dh1);
    storage.addDiningHall(dh2);

    Meal* m1 = new Meal(storage.nextMealID(),
                       "Cheesy Omelette", 3.5f,
                       MealType::BREAKFAST, ReserveDay::MONDAY);
    Meal* m2 = new Meal(storage.nextMealID(),
                       "Chicken Lunch", 6.0f,
                       MealType::LUNCH, ReserveDay::MONDAY);
    storage.addMeal(m1);
    storage.addMeal(m2);

    Student* s1 = new Student(storage.nextStudentID(),
                              "ali", "rezaei", "hashed",
                              "s100", "ali@example.com",
                              "09120000000", 20.0f);
    storage.addStudent(s1);

    Admin* a1 = new Admin(storage.nextAdminID(),
                          "admin", "master", "hashed");
    storage.addAdmin(a1);

    auto& studentSess = StudentSession::SessionManager::instance();
    studentSess.login("ali", "");

    panel.addToShoppingCart(s1, m2, dh1);
    panel.viewShoppingCart(s1);
    panel.confirmShoppingCart(s1);
    panel.viewReservations(*s1);
    panel.viewRecentTransactions();
    panel.increaseBalance(s1, 10.0f);
    s1->print();

    delete dh1;
    delete dh2;
    delete m1;
    delete m2;
    delete s1;
    delete a1;

    return 0;
}
