#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

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
    static int nextId;
    int id;
    float quantity;
    double nominalValue;
    string activeName;
    OrderType orderType;

public:
    Order()
        : id(nextId++), quantity(0), nominalValue(0.0), activeName(""), orderType(OrderType::Buy) {}

    Order(float orderQuantity, double value, const string& name, OrderType type)
        : id(nextId++), quantity(orderQuantity), nominalValue(value), activeName(name), orderType(type) {}

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

    void setOperate(bool newOperate) {
        operate = newOperate;
    }

    bool getOperate() const {
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
            cout << "ID: " << order.getId() << ", Type: " << static_cast<int>(order.getOrderType())
                 << ", Quantity: " << order.getQuantity() << ", Nominal Value: " << order.getNominalValue()
                 << ", Active Name: " << order.getActiveName() << endl;
        }
    }
    
    void printWalletBalance() const {
	        double totalBalance = wallet.getBalance();
	        for (const auto& order : orders) {
	            double nominalValue = order.getNominalValue();
	            totalBalance -= nominalValue;
	        }
	        cout << "Wallet Balance: " << totalBalance << endl;
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

    if (orderType == OrderType::Buy) {
        cout << "Buy order." << endl;
        float quantity;
        int activeIndex;

        cout << "Enter quantity: ";
        cin >> quantity;

        cout << "Choose the active to buy:\n";
        for (int i = 0; i < numActives; ++i) {
            cout << i + 1 << ". " << activeList[i]->getName() << " (Unit Value: " << activeList[i]->getUnitValue() << ")\n";
        }
        cin >> activeIndex;
        activeIndex--;  // Adjust for 0-based indexing

        double nominalValue = quantity * activeList[activeIndex]->getUnitValue();

        if (nominalValue > user.getWallet().getBalance()) {
            cout << "Insufficient balance. Order execution failed." << endl;
            return false;
        }

        Order order(quantity, nominalValue, activeList[activeIndex]->getName(), orderType);
        user.addOrder(order);
        user.getWallet().subtractFromBalance(nominalValue);
        return true;
    } else if (orderType == OrderType::Sell) {
        cout << "Sell order." << endl;
        float quantity;
        int activeIndex;

        cout << "Enter quantity: ";
        cin >> quantity;

        cout << "Choose the active to sell:\n";
        for (int i = 0; i < numActives; ++i) {
            cout << i + 1 << ". " << activeList[i]->getName() << " (Unit Value: " << activeList[i]->getUnitValue() << ")\n";
        }
        cin >> activeIndex;
        activeIndex--;  // Adjust for 0-based indexing

        double nominalValue = quantity * activeList[activeIndex]->getUnitValue();

        Order order(quantity, nominalValue, activeList[activeIndex]->getName(), orderType);
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
    Active* silver = new Active();
    Active* nasdaq100 = new Active();
    Active* usdinx = new Active();

    User user1, user2;

    user1.setName("User 1");
    user2.setName("User 2");

    user1.setOperate(true);
    user1.getWallet().setBalance(1000);

    user2.setOperate(false);
    user2.getWallet().setBalance(500);

    queue<User> userQueue;
    userQueue.push(user1);
    userQueue.push(user2);

    Active* activeList[] = { gold, silver, nasdaq100, usdinx };
    int numActives = sizeof(activeList) / sizeof(activeList[0]);

    while (!userQueue.empty()) {
        User currentUser = userQueue.front();
        userQueue.pop();
        ExecuteOrder(currentUser, activeList, numActives);
        currentUser.printOrders();
        currentUser.printWalletBalance();
    }

    delete gold;
    delete silver;
    delete nasdaq100;
    delete usdinx;

    return 0;
}

