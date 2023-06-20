#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

enum class OrderType {
    Buy,
    Sell
};

class Active {
    string name;
    string typeOfContract;
    string category;
    double price;

public:
    Active() : name(""), typeOfContract(""), category(""), price(0.0) {}

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
    static int nextId;
    int id;
    float quantity;
    double nominalValue;
    OrderType orderType;

public:
    Order() : id(nextId++), quantity(0.0), nominalValue(0.0), orderType(OrderType::Buy) {}
    Order(float orderQuantity, double value, OrderType type)
        : id(nextId++), quantity(orderQuantity), nominalValue(value), orderType(type) {}

    int getId() const {
        return id;
    }

    float getQuantity() const {
        return quantity;
    }

    double getNominalValue() const {
        return nominalValue;
    }

    OrderType getOrderType() const {
        return orderType;
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

    bool Operate(bool newOperate) {
        operate = newOperate;
        return operate;
    }

    Wallet& getWallet() {
        return wallet;
    }

    void addOrder(Order order) {
        orders.push_back(order);
    }

    void printOrders() const {
        cout << "User Orders:\n";
        for (const auto& order : orders) {
            cout << "ID: " << order.getId()
                 << ", Type: " << static_cast<int>(order.getOrderType())
                 << ", Quantity: " << order.getQuantity()
                 << ", Nominal Value: " << order.getNominalValue() << endl;
        }
        cout << "Wallet Balance: " << wallet.getBalance() << endl;
    }
};

OrderType MakeOrder(User& user) {
    if (user.Operate(true)) {
        int choice;
        do {
            cout << "Choose an option:\n";
            cout << "1. Buy\n";
            cout << "2. Sell\n";
            cin >> choice;
        } while (choice != 1 && choice != 2);

        return (choice == 1) ? OrderType::Buy : OrderType::Sell;
    }

    return OrderType::Buy;  // Default value if the operation couldn't be performed
}

bool ExecuteOrder(User& user) {
    OrderType orderType = MakeOrder(user);

    if (orderType == OrderType::Buy) {
        cout << "Buy order." << endl;
        float quantity;
        double nominalValue;

        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter nominal value: ";
        cin >> nominalValue;

        Order order(quantity, nominalValue, orderType);
        user.addOrder(order);
        user.getWallet().subtractFromBalance(nominalValue);
        return true;
    } else if (orderType == OrderType::Sell) {
        cout << "Sell order." << endl;
        float quantity;
        double nominalValue;

        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter nominal value: ";
        cin >> nominalValue;

        Order order(quantity, nominalValue, orderType);
        user.addOrder(order);
        user.getWallet().subtractFromBalance(nominalValue);
        return true;
    } else {
        cout << "Order execution failed." << endl;
        return false;
    }
}

int main() {
    Active* gold = new Active();
    gold->setContractType("CFD");

    // Crear usuarios
    User user1, user2, user3, user4, user5;

    // Establecer nombres de usuarios
    user1.setName("User 1");
    user2.setName("User 2");
    user3.setName("User 3");
    user4.setName("User 4");
    user5.setName("User 5");

    // Establecer operación activa para todos los usuarios
    user1.Operate(true);
    user2.Operate(true);
    user3.Operate(true);
    user4.Operate(true);
    user5.Operate(true);

    // Establecer saldo inicial en la billetera de cada usuario
    user1.getWallet().setBalance(10000);
    user2.getWallet().setBalance(10000);
    user3.getWallet().setBalance(10000);
    user4.getWallet().setBalance(10000);
    user5.getWallet().setBalance(10000);

    // Crear cola de usuarios
    queue<User> userQueue;
    userQueue.push(user1);
    userQueue.push(user2);
    userQueue.push(user3);
    userQueue.push(user4);
    userQueue.push(user5);

    // Procesar órdenes para cada usuario en la cola
    while (!userQueue.empty()) {
        User currentUser = userQueue.front();
        userQueue.pop();
        ExecuteOrder(currentUser);
        currentUser.printOrders();
    }

    delete gold;

    return 0;
}
