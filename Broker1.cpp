#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Date {
    int day;
    int month;
    int year;
};
enum class OrderStatus {
    Open,
    Closed
};
enum class OrderType {
    None,
    Buy,
    Sell
};

class Active {
    string name;
    string typeOfContract;
    string category;
    double price;
    float unitValue;

public:
    Active() : name(""), typeOfContract(""), category(""), price(0.0), unitValue(0.5) {}

    void setName(string newName) {
        name = newName;
    }

    string getName() const {
        return name;
    }

    void setContractType(string newType) {
        typeOfContract = newType;
    }

    string getContractType() const {
        return typeOfContract;
    }

    void setPrice(double marketPrice) {
        price = marketPrice;
    }

    double getMarketPrice() const {
        return price;
    }
    
    void setUnitValue(float value) {
        unitValue = value;
    }
    
    float getUnitValue() const {
        return unitValue;
    }
};

class Order {
	OrderStatus status;
    static int nextId;
    int id;
    float quantity;
    double nominalValue;
    string activeName;
    OrderType orderType;
    Date openingDate;
    Date closingDate;
    
   
public:
    Order()
	    : status(OrderStatus::Open), id(nextId++), quantity(0), nominalValue(0.0), activeName(""), orderType(OrderType::Buy) {}
	
	Order(float orderQuantity, double value, const string& name, OrderType type, const Date& date)
	    : status(OrderStatus::Open), id(nextId++), quantity(orderQuantity), nominalValue(value), activeName(name), orderType(type), openingDate(date), closingDate({0, 0, 0}) {}
	
	    OrderStatus getStatus() const {
	        return status;
	    }
	
	    void setStatus(OrderStatus newStatus) {
	        status = newStatus;
	    }
    int getId() const {
        return id;
    }

    float getQuantity() const {
        return quantity;
    }

    double getNominalValue() const {
        return nominalValue;
    }

    string getActiveName() const {
        return activeName;
    }

    OrderType getOrderType() const {
        return orderType;
    }
    
    void setClosingDate(const Date& date) {
        closingDate = date;
    }
    
    Date getOpeningDate() const {
        return openingDate;
    }
    
    Date getClosingDate() const {
        return closingDate;
    }
};

int Order::nextId = 1;

class Wallet {
    double balance;

public:
    Wallet() : balance(0.0) {}

    void setBalance(double newBalance) {
        balance = newBalance;
    }

    double getBalance() const {
        return balance;
    }

    void subtractFromBalance(double amount) {
        balance -= amount;
    }

    void addToBalance(double amount) {
        balance += amount;
    }
};

class User {
    string name;
    bool operate;
    vector<Order> orders;
    Wallet wallet;

public:
    User() {}

    void setName(string newName) {
        name = newName;
    }

    void setOperate(bool newOperate) {
        operate = newOperate;
    }

    bool getOperate() const {
        return operate;
    }

    Wallet& getWallet() {
        return wallet;
    }

   
       void addOrder(const Order& order) {
           orders.push_back(order);
   
           wallet.subtractFromBalance(order.getNominalValue());
   
           cout << "Added order with ID: " << order.getId() << endl;
       }
   
       void closeOrder(int orderId, const Date& closingDate) {
           auto it = find_if(orders.begin(), orders.end(), [&](const Order& order) {
               return order.getId() == orderId;
           });
   
           if (it != orders.end()) {
               Order& closedOrder = *it;
               closedOrder.setStatus(OrderStatus::Closed);
               closedOrder.setClosingDate(closingDate);
   
               // No es necesario restar el valor nominal nuevamente al balance
               // wallet.subtractFromBalance(closedOrder.getNominalValue());
   
               cout << "Closed order with ID: " << orderId << endl;
           } else {
               cout << "Order with ID " << orderId << " not found." << endl;
           }
       }
	
	

    void printOrders() const {
	    cout << "User Transactions:\n";
	    for (const auto& order : orders) {
	        cout << "ID: " << order.getId() << ", Type: " << static_cast<int>(order.getOrderType())
	             << ", Quantity: " << order.getQuantity() << ", Nominal Value: " << order.getNominalValue()
	             << ", Active Name: " << order.getActiveName()
	             << ", Status: " << (order.getStatus() == OrderStatus::Open ? "Open" : "Closed")
	             << ", Opening Date: " << order.getOpeningDate().day << "/" << order.getOpeningDate().month << "/" << order.getOpeningDate().year
	             << ", Closing Date: " << order.getClosingDate().day << "/" << order.getClosingDate().month << "/" << order.getClosingDate().year
	             << endl;
	    }
	}
	

    void printWalletBalance() const {
        cout << "Wallet Balance: " << wallet.getBalance() << endl;
    }
};

OrderType MakeOrder(User& user) {
    if (user.getOperate()) {
        int choice;
        cout << "Choose an option:\n";
        cout << "1. Buy\n";
        cout << "2. Sell\n";
        cin >> choice;
        return (choice == 1) ? OrderType::Buy : OrderType::Sell;
    }

    return OrderType::None;
}

bool ExecuteOrder(User& user, Active* activeList[], int numActives) {
    OrderType orderType = MakeOrder(user);

    void (User::*executeFunction)(const Order&) = nullptr;

    if (orderType == OrderType::Buy) {
        cout << "Buy order." << endl;
        executeFunction = &User::addOrder;
    } else if (orderType == OrderType::Sell) {
        cout << "Sell order." << endl;
        executeFunction = &User::addOrder;
    } else {
        cout << "Order execution failed." << endl;
        return false;
    }

    if (executeFunction != nullptr) {
        float quantity;
        int activeIndex;
        int day, month, year;

        cout << "Enter quantity: ";
        cin >> quantity;

        cout << "Choose the active:\n";
        for (int i = 0; i < numActives; ++i) {
            cout << i + 1 << ". " << activeList[i]->getName() << " (Unit Value: " << activeList[i]->getUnitValue() << ")\n";
        }
        cin >> activeIndex;
        activeIndex--;  // Adjust for 0-based indexing

        cout << "Enter opening date (day month year): ";
        cin >> day >> month >> year;
        Date openingDate = { day, month, year };

        double nominalValue = quantity * activeList[activeIndex]->getUnitValue(); // Calculate nominal value

        Order order(quantity, nominalValue, activeList[activeIndex]->getName(), orderType, openingDate);
        (user.*executeFunction)(order);
        return true;
    }

    return false;
}

int main() {
    Active* gold = new Active();
    Active* silver = new Active();
    Active* nasdaq100 = new Active();
    Active* usdinx = new Active();

    gold->setName("Gold");
    silver->setName("Silver");
    nasdaq100->setName("Nasdaq100");
    usdinx->setName("USDINX");

    User user1, user2, user3;

    user1.setName("User 1");
    user2.setName("User 2");
    user3.setName("User 3");

    user1.setOperate(true);
    user1.getWallet().setBalance(1000);

    user2.setOperate(false);
    user2.getWallet().setBalance(500);

    user3.setOperate(true);
    user3.getWallet().setBalance(6000);

    queue<User> users;
    users.push(user1);
    users.push(user2);
    users.push(user3);

    Active* activeList[] = { gold, silver, nasdaq100, usdinx };
    int numActives = sizeof(activeList) / sizeof(activeList[0]);

    while (!users.empty()) {
        User& currentUser = users.front();
        users.pop();
        ExecuteOrder(currentUser, activeList, numActives);
        currentUser.printOrders();
        currentUser.printWalletBalance();

        int closeOrderId;
        cout << "Enter the ID of the order to close (0 to skip): ";
        cin >> closeOrderId;

        if (closeOrderId != 0) {
            int day, month, year;
            cout << "Enter closing date (day month year): ";
            cin >> day >> month >> year;
            Date closingDate = { day, month, year };

            currentUser.closeOrder(closeOrderId, closingDate);
            currentUser.printOrders();
            currentUser.printWalletBalance();
        }
    }

    delete gold;
    delete silver;
    delete nasdaq100;
    delete usdinx;

    return 0;
    
}
