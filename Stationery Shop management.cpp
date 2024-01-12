#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>
#include <iomanip> 

using namespace std;

enum UserType {
    CUSTOMER,
    SHOPKEEPER,
    EXIT_PROGRAM
};

enum CustomerOption {
    BUY_ITEM = 1,
    VIEW_ITEMS,
    EXIT_CUSTOMER
};

enum ShopkeeperOption {
    ADD_ITEM = 1,
    DISPLAY_ITEMS,
    CHECK_ITEM,
    UPDATE_ITEM,
    DELETE_ITEM,
    SAVE_TO_FILE,
    LOAD_FROM_FILE,
    EXIT_SHOPKEEPER
};

class Transaction {
public:
    int itemCode;
    string itemName;
    int quantity;
    double itemCost;
    time_t transactionTime;

    Transaction(int code, const string& name, int qty, double cost)
        : itemCode(code), itemName(name), quantity(qty), itemCost(cost) {
        transactionTime = time(0);
    }
};

class Item {
public:
    int Item_Id;
    string item_name;
    int stock;

    Item(int id, const string& name, int stock) : Item_Id(id), item_name(name), stock(stock) {}

    virtual void display() {
        cout << " Item Code: " << Item_Id << "\n";
        cout << " Item Name: " << item_name << "\n";
        cout << " Stock: " << stock << "\n";
    }
};

class StationaryItem : public Item {
public:
    string brand;
    double cost;

    StationaryItem(int id, const string& name, const string& brand, int stock, double cost)
        : Item(id, name, stock), brand(brand), cost(cost) {}

    void display() override {
        Item::display();
        cout << " Brand: " << brand << "\n";
        cout << " Cost: $" << cost << "\n";
    }
};

class StationaryStore {
private:
    vector<Item*> items;
    vector<Transaction> transactions;
    static int totalItemsAdded;
    double totalCost;

public:
    StationaryStore() : totalCost(0) {}

    void control_panel();
    void add_item();
    void display_items();
    void check_item();
    void update_item();
    void delete_item();
    void buy_item();
    void saveTransactionHistory();
    ~StationaryStore();
    friend void saveToFile(const StationaryStore& store);
    friend void loadFromFile(StationaryStore& store);
};
void StationaryStore::control_panel() {
    system("Color 0A");
    system("cls");
    cout << setw(50) << setfill('*') << "Stationary Store Management System" << setw(50) << setfill('*');
    cout << "\n 1. Add New Stationary Item";
    cout << "\n 2. Display Stationary Items";
    cout << "\n 3. Check Specific Stationary Item";
    cout << "\n 4. Update Stationary Item";
    cout << "\n 5. Delete Stationary Item";
    cout << "\n 6. Save to File";
    cout << "\n 7. Load from File";
    cout << "\n 8. Exit";
}
int StationaryStore::totalItemsAdded = 0;

void StationaryStore::add_item() {
    system("cls");
    system("Color 0A");
    int id, stock;
    string name, brand;
    double cost;
    cout << "\n Add New Stationary Item: \n";
    cout << setw(80) << setfill('*') << "" << endl;
    cout << " Item Code : ";
    cin >> id;
    cout << setw(80) << setfill('*') << "" << endl;

   
    for (Item* item : items) {
        if (item->Item_Id == id) {
            throw runtime_error("Item with the same ID already exists. Please choose a unique ID.");
        }
    }

    cout << "\n Item Name: ";
    cin >> name;
    cout << "\n Brand Name: ";
    cin >> brand;
    cout << "\n Cost: $";
    cin >> cost;
    cout << "\n Stock: ";
    cin >> stock;
    items.push_back(new StationaryItem(id, name, brand, stock, cost));
    totalItemsAdded++;
    cout << setw(80) << setfill('*') << "" << endl;
}

void StationaryStore::display_items() {
    system("cls");
    system("Color 0A");
    cout << setw(60) << setfill('*') << " Stationary Items" << setw(60) << setfill('*') << "" << endl;
    for (Item* item : items) {
        item->display();
        cout << setw(80) << setfill('-') << "" << endl;
    }
    cout << "Total Items Added: " << totalItemsAdded << endl;
}

void StationaryStore::check_item() {
    system("cls");
    system("Color 0A");
    int itemCode;
    cout << "\n Check Specific Stationary Item\n";
    cout << setw(80) << setfill('*') << "" << endl;
    cout << "\n Item Code: ";
    cin >> itemCode;
    cout << setw(80) << setfill('*') << "" << endl;
    bool found = false;
    for (Item* item : items) {
        if (item->Item_Id == itemCode) {
            item->display();
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Stationary Item not found.\n";
    }
}

void StationaryStore::update_item() {
    system("cls");
    system("Color 0A");
    int itemCode;
    cout << "\n Update Stationary Item Record\n";
    cout << setw(80) << setfill('*') << "" << endl;
    cout << "\n Item Code : ";
    cin >> itemCode;
    cout << setw(80) << setfill('*') << "" << endl;
    for (Item* item : items) {
        if (item->Item_Id == itemCode) {
            item->display();
            cout << "Enter new information:\n";
            int stock;
            string name, brand;
            double cost;
            cout << "\n Item Name: ";
            cin >> name;
            cout << "\n Brand Name: ";
            cin >> brand;
            cout << "\n Cost: $";
            cin >> cost;
            cout << "\n Stock: ";
            cin >> stock;
            item->item_name = name;
            if (StationaryItem* sItem = dynamic_cast<StationaryItem*>(item)) {
                sItem->brand = brand;
                sItem->cost = cost;
            }
            item->stock = stock;
            cout << "Item updated successfully.\n";
            return;
        }
    }
    cout << "Stationary Item not found.\n";
}

void StationaryStore::delete_item() {
    system("cls");
    system("Color 0A");
    int itemCode;
    cout << "\n Delete Stationary Item Record";
    cout << setw(80) << setfill('*') << "" << endl;
    cout << "\n Item Code : ";
    cin >> itemCode;
    cout << setw(80) << setfill('*') << "" << endl;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->Item_Id == itemCode) {
            items[i]->display();
            cout << "Item deleted successfully.\n";
            delete items[i];
            items.erase(items.begin() + i);
            return;
        }
    }
    cout << "Stationary Item not found.\n";
}



void StationaryStore::buy_item() {
    double totalBill = 0;

    while (true) {
        int itemCode;
        int freq;
        cout << "Enter the Item Code you want to buy (or -1 to finish): ";
        cin >> itemCode;

        if (itemCode == -1) {
            break;
        }

        bool found = false;
        for (Item* item : items) {
            if (item->Item_Id == itemCode) {
                cout << "Enter how many do you want to buy: ";
                cin >> freq;

                if (item->stock > 0) {
                    if (freq <= item->stock) {
                        cout << "You have successfully bought " << freq << " " << item->item_name << ".\n";
                        item->stock -= freq;
                        double itemCost = freq * dynamic_cast<StationaryItem*>(item)->cost;
                        totalBill += itemCost;

                        transactions.push_back(Transaction(itemCode, item->item_name, freq, itemCost));

                        cout << "Total cost: $" << totalBill << endl;
                    } else {
                        cout << "Sorry, this item is out of stock.\n";
                    }
                } else {
                    cout << "Sorry, this item is out of stock.\n";
                }
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Item not found.\n";
        }
    }

    saveTransactionHistory();
    cout << "Total bill: $" << totalBill << endl;
}

void StationaryStore::saveTransactionHistory() {
    ofstream file("transaction_history.txt", ios::app);
    if (file.is_open()) {
        file << "Transaction History:\n";
        for (const Transaction& transaction : transactions) {
            file << "Item Code: " << transaction.itemCode << "\n";
            file << "Item Name: " << transaction.itemName << "\n";
            file << "Quantity: " << transaction.quantity << "\n";
            file << "Item Cost: $" << transaction.itemCost << "\n";
            file << "Transaction Time: " << ctime(&transaction.transactionTime);
            file << "------------------------------------------\n";
        }
        file.close();
        cout << "Transaction history saved to 'transaction_history.txt'.\n";
    } else {
        cout << "Error opening the file for saving transaction history.\n";
    }
}

StationaryStore::~StationaryStore() {
    for (Item* item : items) {
        delete item;
    }
    items.clear();
}

void saveToFile(const StationaryStore& store) {
    ofstream file("stationary_items.txt");
    if (file.is_open()) {
        for (Item* item : store.items) {
            file << item->Item_Id << " "
                 << item->item_name << " "
                 << item->stock;
            if (StationaryItem* sItem = dynamic_cast<StationaryItem*>(item)) {
                file << " " << sItem->brand << " " << sItem->cost;
            }
            file << "\n";
        }
        file.close();
        cout << "Data saved to 'stationary_items.txt'.\n";
    } else {
        cout << "Error opening the file for saving.\n";
    }
}

void loadFromFile(StationaryStore& store) {
    ifstream file("stationary_items.txt");
    if (file.is_open()) {
        store.items.clear();
        int id, stock;
        string name, brand;
        double cost;
        while (file >> id >> name >> stock) {
            if (file.peek() == ' ') {
                file.get();
                file >> brand >> cost;
                store.items.push_back(new StationaryItem(id, name, brand, stock, cost));
            } else {
                store.items.push_back(new Item(id, name, stock));
            }
        }
        file.close();
        cout << "Data loaded from 'stationary_items.txt'.\n";
    } else {
        cout << "Error opening the file for loading.\n";
    }
}

int main() {
    StationaryStore store;
    UserType userTypeChoice;
    CustomerOption customerChoice;
    ShopkeeperOption shopkeeperChoice;

    do {
        cout << "Choose User Type:\n";
        cout << "1. Customer\n";
        cout << "2. Shopkeeper\n";
        cout << "3. Exit Program\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                userTypeChoice = CUSTOMER;
                break;
            case 2:
                userTypeChoice = SHOPKEEPER;
                break;
            case 3:
                userTypeChoice = EXIT_PROGRAM;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                continue;
        }

        switch (userTypeChoice) {
            case CUSTOMER:
                do {
                    cout << "Customer Menu:\n";
                    cout << "1. Buy Item\n";
                    cout << "2. View Items\n";
                    cout << "3. Exit Customer\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    switch (choice) {
                        case 1:
                            customerChoice = BUY_ITEM;
                            store.buy_item();
                            break;
                        case 2:
                            customerChoice = VIEW_ITEMS;
                            store.display_items();
                            break;
                        case 3:
                            customerChoice = EXIT_CUSTOMER;
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            continue;
                    }
                } while (customerChoice != EXIT_CUSTOMER);
                break;
            case SHOPKEEPER:
                do {
                    store.control_panel();

                    cout << "Your Choice: ";
                    cin >> choice;

                    switch (choice) {
                        case 1:
                            shopkeeperChoice = ADD_ITEM;
                            try {
                                store.add_item();
                            } catch (const runtime_error& e) {
                                cout << "Error: " << e.what() << endl;
                            }
                            break;
                        case 2:
                            shopkeeperChoice = DISPLAY_ITEMS;
                            store.display_items();
                            break;
                        case 3:
                            shopkeeperChoice = CHECK_ITEM;
                            store.check_item();
                            break;
                        case 4:
                            shopkeeperChoice = UPDATE_ITEM;
                            store.update_item();
                            break;
                        case 5:
                            shopkeeperChoice = DELETE_ITEM;
                            store.delete_item();
                            break;
                        case 6:
                            shopkeeperChoice = SAVE_TO_FILE;
                            saveToFile(store);
                            break;
                        case 7:
                            shopkeeperChoice = LOAD_FROM_FILE;
                            loadFromFile(store);
                            break;
                        case 8:
                            shopkeeperChoice = EXIT_SHOPKEEPER;
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            continue;
                    }
                    cout << "\n";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cin.get(); 
                } while (shopkeeperChoice != EXIT_SHOPKEEPER);
                break;
        }
    } while (userTypeChoice != EXIT_PROGRAM);

    return 0;
}