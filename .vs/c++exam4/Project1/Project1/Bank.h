#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

using namespace std;

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showMenu() {
    cout << "1. Register User\n";
    cout << "2. Login\n";
    cout << "3. Add Wallet\n";
    cout << "4. Add Card\n";
    cout << "5. Deposit to Card\n";
    cout << "6. Add Expense\n";
    cout << "7. Generate Reports\n";
    cout << "8. Save Reports to File\n";
    cout << "9. Show\n";
    cout << "10. Exit\n";
    cout << "Enter your choice: ";
}

class Card {
private:
    double balance;
    int cardNumber;
    int cardName;

public:
    Card() : balance(0), cardNumber(0), cardName(0) {}

    double getBalance() const { return balance; }

    int getCardNumber() const { return cardNumber; }

    int getCardName() const { return cardName; }

    void setCardNumber(int number) {
        cardNumber = number;
    }

    void setCardName(int name) {
        if (name >= 1000 && name <= 9999) {
            cardName = name;
        }
        else {
            cout << "Invalid card name. Please enter a four-digit number.\n";
        }
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Card #" << cardNumber << " (" << setfill('0') << setw(4) << cardName << ") has been successfully topped up. Current balance: $" << balance << "\n";
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
        }
        else {
            cout << "Not enough funds for the concept.\n";
        }
    }
};


class Wallet {
private:
    double balance;
    int walletNumber;
    string walletName;
    vector<Card> cards; 

public:
    Wallet(const string& name) : balance(0), walletNumber(0), walletName(name) {}

    double getBalance() const { return balance; }

    int getWalletNumber() const { return walletNumber; }

    string getWalletName() const { return walletName; }

    vector<Card>& getCards() { return cards; }  // для неконстантних об'єктів
    const vector<Card>& getCards() const { return cards; }  // для константних об'єктів

    void setWalletName(const string& name) {
        walletName = name;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Wallet #" << walletNumber << " (" << walletName << ") has been successfully topped up. Current balance: $" << balance << "\n";
    }

    void addCard(const Card& card) {
        cards.push_back(card);
    }
};

// Представляє користувача з іменем користувача та паролем.
// Додано Wallet в клас користувача
class User {
private:
    string username;
    string password;
    vector<Wallet> wallets; 

public:
    User(const string& username, const string& password)
        : username(username), password(password) {}

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }

    vector<Wallet>& getWallets() {
        return wallets;
    }

    void addWallet(const Wallet& wallet) {
        wallets.push_back(wallet);
    }

    bool hasWallet() const {
        return !wallets.empty();
    }

    Wallet& getWalletByIndex(int index) {
        return wallets.at(index);
    }
};


// Представляє дебетову картку, успадковану від класу Card.
class DebitCard : public Card {
public:
    DebitCard() {}
};

// Представляє кредитну картку з класу Card з додатковим кредитним лімітом.
class CreditCard : public Card {
private:
    double creditLimit;

public:
    CreditCard() : creditLimit(10000) {}

    double getCreditLimit() const {
        return creditLimit;
    }

    void setCreditLimit(double limit) {
        creditLimit = limit;
    }
};
// Представляє витрати з категорією та сумою.
class Expense {
private:
    string category;
    double amount;

public:
    Expense(const string& category, double amount) : category(category), amount(amount) {}

    string getCategory() const { return category; }

    double getAmount() const { return amount; }
};
// Керує фінансовими операціями, обліковими записами користувачів і створює звіти.
class FinanceManager {
private:
    vector<User> users;
    vector<Wallet> wallets;
    vector<Card> cards;
    vector<CreditCard> creditCards;
    vector<DebitCard> debitCards;
    vector<Expense> expenses;
    User* currentUser;

public:
    FinanceManager() : currentUser(nullptr) {}
    // Реєструє нового користувача з іменем користувача та паролем.
    void registerUser(const string& username, const string& password) {
        users.push_back(User(username, password));
    }  
    // Спроба увійти користувача за допомогою імені користувача та пароля.
    bool loginUser(const string& username, const string& password) {
        for (auto& user : users) {
            if (user.getUsername() == username && user.getPassword() == password) {
                currentUser = &user;
                return true;
            }
        }
        return false;
    }
    // Перевіряє, чи користувач увійшов у систему.
    bool isLoggedIn() const {
        return currentUser != nullptr;
    }
    // Додає гаманець за умовчанням.
  /*  void addWallet() {
        wallets.push_back(Wallet());
    }*/
    // Додає гаманець із вказаною назвою.
    void addWallet() {
        if (!isLoggedIn()) {
            cout << "You must log in first.\n";
            return;
        }

        string walletName;
        cout << "Enter wallet name: ";
        cin.ignore();
        getline(cin, walletName);

        Wallet newWallet(walletName);
        currentUser->addWallet(newWallet); // Додаємо гаманець до поточного користувача
        cout << "Wallet added for user " << currentUser->getUsername() << ".\n";
    }

    bool hasWallet() const {
        if (currentUser != nullptr && !currentUser->getWallets().empty()) {
            return true;
        }
        return false;
    }

   
    void addCard(int cardNumber, bool isCreditCard) {
        if (!currentUser || currentUser->getWallets().empty()) {
            cout << "You need to create a wallet first!\n";
            return;
        }

        Card newCard;
        newCard.setCardNumber(cardNumber);

        if (isCreditCard) {
            newCard = CreditCard();  // Створюємо кредитну картку
        }
        else {
            newCard = DebitCard();   // Створюємо дебетову картку
        }

        // Додаємо картку до останнього створеного гаманця
        currentUser->getWallets().back().addCard(newCard);
        cout << (isCreditCard ? "Credit" : "Debit") << " Card added to wallet.\n";
    }

    // Додає кредитну картку.
    void addCreditCard(int cardNumber) {
        CreditCard newCreditCard;
        newCreditCard.setCardNumber(cardNumber);
        creditCards.push_back(newCreditCard);
    }
    // Вносить суму на картку з указаним номером картки.
    void depositToCard(int cardNumber, double amount) {
        Card* foundCard = findCardByNumber(cardNumber);

        if (foundCard) {
            foundCard->deposit(amount);
            cout << "Deposit to card successful.\n";
        }
        else {
            cout << "Card #" << cardNumber << " not found.\n";
        }
    }
    // Додає дебетову картку.
    void addDebitCard(int cardNumber) {
        DebitCard newDebitCard;
        newDebitCard.setCardNumber(cardNumber);
        debitCards.push_back(newDebitCard);
    }
    // Показує список дебетових карток.
    void showDebitCards() const {
        if (!isLoggedIn()) {
            cout << "Please log in first.\n";
            return;
        }

        cout << "Debit Cards:\n";
        for (const DebitCard& debitCard : debitCards) {
            cout << "Card #" << debitCard.getCardNumber() << " (" << setfill('0') << setw(4) << debitCard.getCardName() << "): $" << debitCard.getBalance() << "\n";
        }
    }
    // Знаходить картку за її номером.
    Card* findCardByNumber(int cardNumber) {
        for (auto& debitCard : debitCards) {
            if (debitCard.getCardNumber() == cardNumber) {
                return &debitCard;
            }
        }
        for (auto& creditCard : creditCards) {
            if (creditCard.getCardNumber() == cardNumber) {
                return &creditCard;
            }
        }
        return nullptr;
    }
    // Додає витрати на картку з заданою категорією та сумою.
    void addExpense(const string& category, double amount, int cardNumber) {
        if (!isLoggedIn()) {
            cout << "Please login first.\n";
            return;
        }

        Card* card = findCardByNumber(cardNumber);

        if (!card) {
            cout << "Card not found.\n";
            return;
        }

        if (card->getBalance() < amount) {
            cout << "There are insufficient funds on the card to make the expenditure.\n";
            return;
        }

        card->withdraw(amount);
        expenses.push_back(Expense(category, amount));

        cout << "Spending on $" << amount << " added to the card #" << cardNumber << " (" << card->getCardName() << ").\n";
    }

    // Показує список гаманців.
    void showWallets() const {
        if (!isLoggedIn()) {
            cout << "Please log in first.\n";
            return;
        }

        cout << "Wallets:\n";
        for (const Wallet& wallet : currentUser->getWallets()) {
            cout << "Wallet #" << wallet.getWalletNumber() << " (" << wallet.getWalletName() << "): $" << wallet.getBalance() << "\n";
        }
    }



    void showCards() const {
        if (!isLoggedIn()) {
            cout << "Please log in first.\n";
            return;
        }

        cout << "Cards:\n";
        for (const Wallet& wallet : currentUser->getWallets()) {
            cout << "Wallet: " << wallet.getWalletName() << "\n";
            for (const Card& card : wallet.getCards()) {
                cout << "  Card #" << card.getCardNumber() << " (" << setfill('0') << setw(4) << card.getCardName() << "): $" << card.getBalance() << "\n";
            }
        }
    }

    // Показує список усіх зареєстрованих користувачів.
    void showUsers() const {
        cout << "Users:\n";
        for (const User& user : users) {
            cout << "Username: " << user.getUsername() << "\n";
        }
    }
    // Знаходить користувача за іменем користувача.
    User* findUser(const string& username) {
        for (auto& user : users) {
            if (user.getUsername() == username) {
                return &user;
            }
        }
        return nullptr;
    }

    // Показує гаманці та картки конкретного користувача.
    void showUserWalletsAndCards(const string& username) {
        User* user = findUser(username);
        if (user) {
            cout << "Wallets and Cards for User '" << username << "':\n";

            // Show Wallets
            for (const Wallet& wallet : wallets) {
                cout << "Wallet #" << wallet.getWalletNumber() << " (" << wallet.getWalletName() << "): $" << wallet.getBalance() << "\n";
            }

            // Show Debit Cards
            for (const DebitCard& debitCard : debitCards) {
                cout << "Debit Card #" << debitCard.getCardNumber() << " (" << setfill('0') << setw(4) << debitCard.getCardName() << "): $" << debitCard.getBalance() << "\n";
            }

            // Show Credit Cards
            for (const CreditCard& creditCard : creditCards) {
                cout << "Credit Card #" << creditCard.getCardNumber() << " (" << setfill('0') << setw(4) << creditCard.getCardName() << "): $" << creditCard.getBalance() << " | Credit Limit: $" << creditCard.getCreditLimit() << "\n";
            }
        }
        else {
            cout << "User '" << username << "' not found.\n";
        }
    }
    // Відображення фінансового звіту з категоріями та сумами.
    void displayReport(const map<string, double>& report) const {
        
        cout << "Report:\n";

        // Виведення кожного запису у звіті
        for (const auto& entry : report) {
            cout << entry.first << ": $" << entry.second << "\n";
        }
    }
    // Створює щоденний звіт про витрати.
    map<string, double> generateDailyReport() {
        map<string, double> dailyReport;

        for (const Expense& expense : expenses) {
            time_t now = time(nullptr);
            tm timeInfo;

            #ifdef _WIN32
            localtime_s(&timeInfo, &now);
            #else
            localtime_r(&now, &timeInfo);
            #endif

            char dateBuffer[11];
            strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);
            string currentDate(dateBuffer);

            dailyReport[currentDate] += expense.getAmount();
        }

        return dailyReport;
    }
    // Створює щотижневий звіт про витрати.
    map<string, double> generateWeeklyReport() {
        map<string, double> weeklyReport;

        for (const Expense& expense : expenses) {
            time_t now = time(nullptr);
            tm timeInfo;

            #ifdef _WIN32
            localtime_s(&timeInfo, &now);
            #else
            localtime_r(&now, &timeInfo);
            #endif

            int weekNumber = timeInfo.tm_yday / 7 + 1;
            int year = timeInfo.tm_year + 1900;
            string currentWeek = to_string(year) + "-W" + to_string(weekNumber);

            weeklyReport[currentWeek] += expense.getAmount();
        }

        return weeklyReport;
    }
    // Формує місячний звіт про витрати.
    map<string, double> generateMonthlyReport() {
        map<string, double> monthlyReport;

        for (const Expense& expense : expenses) {
            time_t now = time(nullptr);
            tm timeInfo;

            #ifdef _WIN32
            localtime_s(&timeInfo, &now);
            #else
            localtime_r(&now, &timeInfo);
            #endif

            char monthBuffer[8];
            strftime(monthBuffer, sizeof(monthBuffer), "%Y-%m", &timeInfo);
            string currentMonth(monthBuffer);

            monthlyReport[currentMonth] += expense.getAmount();
        }

        return monthlyReport;
    }
    //Порівнює пари string і double на основі значення double(для сортування).
    static bool compareByValue(const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    }
    // На основі звіту створює список із 3 найбільших витрат.
    vector<pair<string, double>> generateTop3Expenses(const map<string, double>& report) {
        
        vector<pair<string, double>> expensesVector(report.begin(), report.end());

        
        sort(expensesVector.begin(), expensesVector.end(), &FinanceManager::compareByValue);

        
        expensesVector.resize(min(expensesVector.size(), static_cast<size_t>(3)));

        return expensesVector;
    }
    
    vector<pair<string, double>> generateTop3Categories(const map<string, double>& report) {
       
        vector<pair<string, double>> categoriesVector(report.begin(), report.end());

        
        sort(categoriesVector.begin(), categoriesVector.end(), compareByValue);

       
        categoriesVector.resize(min(categoriesVector.size(), static_cast<size_t>(3)));

        return categoriesVector;
    }
    //збереження у файл
    void saveToFile(const string& filename) {
        ofstream file(filename, ios::binary);

        if (!file.is_open()) {
            throw runtime_error("Unable to open file for writing.");
        }

        // Записуємо дані в файл
        file.write(reinterpret_cast<const char*>(this), sizeof(FinanceManager));

        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);

        if (!file.is_open()) {
            throw runtime_error("Unable to open file for reading.");
        }

        // Зчитуємо дані з файлу
        file.read(reinterpret_cast<char*>(this), sizeof(FinanceManager));

        file.close();
    }

};
