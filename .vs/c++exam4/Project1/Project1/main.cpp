#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <algorithm>
#include "Bank.h"
using namespace std;

void main() {


    FinanceManager financeManager;

   

    while (true) {
 
        clearConsole();
        showMenu();

        int choice;
        cin >> choice;

            

        switch (choice) {
        case 1: // Реєстрація
        {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;

            financeManager.registerUser(username, password);
            std::cout << "User registered.\n";
        }
        break;

        case 2: // Логін
        {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;

            if (financeManager.loginUser(username, password)) {
                std::cout << "Login successful.\n";
            }
            else {
                std::cout << "Login failed. Invalid credentials.\n";
            }
        }
        break;

        case 3: // Додавання гаманця
        {
            if (!financeManager.isLoggedIn()) {
                std::cout << "You must log in first.\n";
                break;
            }

            financeManager.addWallet();  // Виклик без аргументів, оскільки всередині методу відбувається запит назви гаманця
            std::cout << "Wallet added.\n";
        }
        break;

        case 4: // Додавання карти до гаманця
        {
            if (!financeManager.isLoggedIn()) {
                std::cout << "You must log in first.\n";
                break;
            }

            if (!financeManager.hasWallet()) {
                std::cout << "You need to create a wallet first!\n";
                break;
            }

            int cardNumber;
            std::cout << "Enter four-digit card number: ";
            std::cin >> cardNumber;

            int cardType;
            std::cout << "Select card type:\n1. Debit Card\n2. Credit Card\n";
            std::cin >> cardType;

            if (cardType == 1) {
                financeManager.addDebitCard(cardNumber);
                std::cout << "Debit Card added.\n";
            }
            else if (cardType == 2) {
                financeManager.addCreditCard(cardNumber);
                std::cout << "Credit Card added.\n";
            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        break;

        case 5: // Поповнення карти
        {
            int cardNumber;
            double amount;
            std::cout << "Enter card number: ";
            std::cin >> cardNumber;
            std::cout << "Enter deposit amount: ";
            std::cin >> amount;

            financeManager.depositToCard(cardNumber, amount);
        }
        break;

        case 6: // Витрати
        {
            int cardNumber;
            double amount;
            std::string category;

            std::cout << "Enter card number: ";
            std::cin >> cardNumber;
            std::cout << "Enter expense amount: ";
            std::cin >> amount;
            std::cout << "Enter category: ";
            std::cin >> category;

            financeManager.addExpense(category, amount, cardNumber);
        }
        break;

        case 7: // Генерація звітів
        {
            financeManager.showCards();
            financeManager.showWallets();
        }
        break;

        case 8: // Показ гаманців та карток користувача
        {
            financeManager.showWallets();
            financeManager.showCards();
        }
        break;

        case 9: // Вихід
            std::cout << "Exiting Finance Manager.\n";
            return;
        }
    }
}