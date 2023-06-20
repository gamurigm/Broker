#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class OrderType {
    Buy,
    Sell
};

class Active {
    int id;
    string name;
    string typeOfContract;
    string category;
    double price;

public:
    Active() : id(0), name(""), typeOfContract(""), category(""), price(0.0) {}

    void setName(string newName) {
        name = newName;
    }

    string getName() {
        return name;
    }

    void setContractType(string newType) {
        typeOfContract = newType;
    }

    string getContractType() {
        return typeOfContract;
    }

    void setPrice(double marketPrice) {
        price = marketPrice;
    }

    double getMarketPrice() {
        return price;
    }
};

class Order {
    int id;
    int quantity;
    double balance;

public:
    Order() {}
    Order(int orderId, int orderQuantity, double orderBalance)
        : id(orderId), quantity(orderQuantity), balance(orderBalance) {}

    int getId() const {
        return id;
    }

    int getQuantity() const {
        return quantity;
    }

    double getBalance() const {
        return balance;
    }
};

class User {
    int id;
    string name;
    bool operate;
    vector<Order> orders;

public:
    User() {}

    void setName(string newName) {
        name = newName;
    }

    bool Operate(bool newOperate) {
        operate = newOperate;
        return operate;
    }

    void addOrder(Order order) {
        orders.push_back(order);
    }

    void printOrders() const {
        cout << "User Orders:\n";
        for (const auto& order : orders) {
            cout << "ID: " << order.getId() << ", Quantity: " << order.getQuantity() << ", Balance: " << order.getBalance() << endl;
        }
    }
};

OrderType MakeOrder(User& user) {
    if (user.Operate(true)) {
        int choice;
        cout << "Choose an option:\n";
        cout << "1. Buy\n";
        cout << "2. Sell\n";
        cin >> choice;
        return (choice == 1) ? OrderType::Buy : OrderType::Sell;
    }

    return OrderType::Buy;  // Default value if the operation couldn't be performed
}

bool ExecuteOrder(User& user) {
    OrderType orderType = MakeOrder(user);

    if (orderType == OrderType::Buy) {
        cout << "Buy order." << endl;
        int orderId, quantity;
        double balance;
        cout << "Enter order ID: ";
        cin >> orderId;
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter balance: ";
        cin >> balance;
        Order order(orderId, quantity, balance);
        user.addOrder(order);
        return true;
    } else if (orderType == OrderType::Sell) {
        cout << "Sell order." << endl;
        int orderId, quantity;
        double balance;
        cout << "Enter order ID: ";
        cin >> orderId;
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter balance: ";
        cin >> balance;
        Order order(orderId, quantity, balance);
        user.addOrder(order);
        return true;
    } else {
        cout << "Order execution failed." << endl;
        return false;
    }
}

int main() {
    Active* gold = new Active();
    User user1;

    gold->setContractType("CFD");
    cout << gold->getContractType() << endl;

    ExecuteOrder(user1);

    user1.printOrders();

    delete gold;

    return 0;
}
