#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <limits>
#include <algorithm>
#include <fstream>  // Added for file handling

using namespace std;

// Car class to store car information
class Car {
private:
    string carID;
    string brand;
    string model;
    double rentalPrice;
    bool isAvailable;

public:
    Car(string id, string b, string m, double price) 
        : carID(id), brand(b), model(m), rentalPrice(price), isAvailable(true) {}

    // Getters
    string getCarID() const { return carID; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    double getRentalPrice() const { return rentalPrice; }
    bool getAvailability() const { return isAvailable; }

    // Setters
    void setAvailability(bool status) { isAvailable = status; }

    // Display car details
    void displayDetails() const {
        cout << "\nCar ID: " << carID
            << "\nBrand: " << brand
            << "\nModel: " << model
            << "\nRental Price per Day: $" << fixed << setprecision(2) << rentalPrice
            << "\nAvailability: " << (isAvailable ? "Available" : "Rented")
            << "\n------------------------" << endl;
    }
};

// Customer class to store customer information
class Customer {
private:
    string customerID;
    string name;
    string contactNumber;

public:
    Customer(string id, string n, string contact) 
        : customerID(id), name(n), contactNumber(contact) {}

    // Getters
    string getCustomerID() const { return customerID; }
    string getName() const { return name; }
    string getContact() const { return contactNumber; }

    // Display customer details
    void displayDetails() const {
        cout << "\nCustomer ID: " << customerID
            << "\nName: " << name
            << "\nContact Number: " << contactNumber
            << "\n------------------------" << endl;
    }
};

// Rental class to manage rental transactions
class Rental {
private:
    string rentalID;
    string customerID;
    string carID;
    time_t startDate;
    int durationDays;
    double totalCost;

public:
    Rental(string rid, string cust, string car, int days, double cost) 
        : rentalID(rid), customerID(cust), carID(car), durationDays(days), totalCost(cost) {
        startDate = time(0);
    }

    // Getters
    string getRentalID() const { return rentalID; }
    string getCustomerID() const { return customerID; }
    string getCarID() const { return carID; }
    int getDuration() const { return durationDays; }
    double getTotalCost() const { return totalCost; }

    // Display rental details
    void displayDetails() const {
        cout << "\nRental ID: " << rentalID
            << "\nCustomer ID: " << customerID
            << "\nCar ID: " << carID
            << "\nDuration (days): " << durationDays
            << "\nTotal Cost: $" << fixed << setprecision(2) << totalCost
            << "\nStart Date: " << ctime(&startDate)
            << "------------------------" << endl;
    }
};

// RentalSystem class to manage the entire system
class RentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Rental> rentals;
    int rentalCounter;

    // Add these new private methods for file handling
    void saveDataToFiles() {
        // Save cars data
        ofstream carFile("cars.txt");
        if (carFile.is_open()) {
            for (const auto& car : cars) {
                carFile << car.getCarID() << ","
                       << car.getBrand() << ","
                       << car.getModel() << ","
                       << car.getRentalPrice() << ","
                       << car.getAvailability() << endl;
            }
            carFile.close();
        }

        // Save customers data
        ofstream customerFile("customers.txt");
        if (customerFile.is_open()) {
            for (const auto& customer : customers) {
                customerFile << customer.getCustomerID() << ","
                           << customer.getName() << ","
                           << customer.getContact() << endl;
            }
            customerFile.close();
        }

        // Save rentals data
        ofstream rentalFile("rentals.txt");
        if (rentalFile.is_open()) {
            for (const auto& rental : rentals) {
                rentalFile << rental.getRentalID() << ","
                          << rental.getCustomerID() << ","
                          << rental.getCarID() << ","
                          << rental.getDuration() << ","
                          << rental.getTotalCost() << endl;
            }
            rentalFile.close();
        }
    }

    void loadDataFromFiles() {
        // Load cars data
        ifstream carFile("cars.txt");
        if (carFile.is_open()) {
            string line;
            while (getline(carFile, line)) {
                string id, brand, model;
                double price;
                bool available;
                size_t pos = 0;
                string token;

                // Parse carID
                pos = line.find(",");
                id = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse brand
                pos = line.find(",");
                brand = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse model
                pos = line.find(",");
                model = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse price
                pos = line.find(",");
                price = stod(line.substr(0, pos));
                line.erase(0, pos + 1);

                // Parse availability
                available = (line == "1");

                cars.push_back(Car(id, brand, model, price));
                if (!available) {
                    cars.back().setAvailability(false);
                }
            }
            carFile.close();
        }

        // Load customers data
        ifstream customerFile("customers.txt");
        if (customerFile.is_open()) {
            string line;
            while (getline(customerFile, line)) {
                string id, name, contact;
                size_t pos = 0;

                // Parse customerID
                pos = line.find(",");
                id = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse name
                pos = line.find(",");
                name = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse contact
                contact = line;

                customers.push_back(Customer(id, name, contact));
            }
            customerFile.close();
        }

        // Load rentals data
        ifstream rentalFile("rentals.txt");
        if (rentalFile.is_open()) {
            string line;
            while (getline(rentalFile, line)) {
                string rentalID, customerID, carID;
                int duration;
                double cost;
                size_t pos = 0;

                // Parse rentalID
                pos = line.find(",");
                rentalID = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse customerID
                pos = line.find(",");
                customerID = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse carID
                pos = line.find(",");
                carID = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Parse duration
                pos = line.find(",");
                duration = stoi(line.substr(0, pos));
                line.erase(0, pos + 1);

                // Parse cost
                cost = stod(line);

                rentals.push_back(Rental(rentalID, customerID, carID, duration, cost));
                
                // Update rental counter
                int currentNumber = stoi(rentalID.substr(1));
                if (currentNumber >= rentalCounter) {
                    rentalCounter = currentNumber + 1;
                }
            }
            rentalFile.close();
        }
    }

public:
    RentalSystem() : rentalCounter(1) {
        loadDataFromFiles();  // Load data when system starts
    }

    ~RentalSystem() {
        saveDataToFiles();  // Save data when system closes
    }

    // Add a new car
    void addCar(string id, string brand, string model, double price) {
        cars.push_back(Car(id, brand, model, price));
        cout << "\nCar added successfully!" << endl;
        saveDataToFiles();
    }

    // Add a new customer
    void addCustomer(string id, string name, string contact) {
        customers.push_back(Customer(id, name, contact));
        cout << "\nCustomer added successfully!" << endl;
        saveDataToFiles();
    }

    // Display all cars
    void displayAllCars() const {
        if (cars.empty()) {
            cout << "\nNo cars available in the system." << endl;
            return;
        }
        cout << "\n=== Available Cars ===" << endl;
        for (const auto& car : cars) {
            car.displayDetails();
        }
    }

    // Display all customers
    void displayAllCustomers() const {
        if (customers.empty()) {
            cout << "\nNo customers registered in the system." << endl;
            return;
        }
        cout << "\n=== Registered Customers ===" << endl;
        for (const auto& customer : customers) {
            customer.displayDetails();
        }
    }

    // Display all rentals
    void displayAllRentals() const {
        if (rentals.empty()) {
            cout << "\nNo rental records found." << endl;
            return;
        }
        cout << "\n=== Rental Records ===" << endl;
        for (const auto& rental : rentals) {
            rental.displayDetails();
        }
    }

    // Rent a car
    void rentCar(string customerID, string carID, int days) {
        // Find customer
        auto customerIt = find_if(customers.begin(), customers.end(),
            [customerID](const Customer& c) { return c.getCustomerID() == customerID; });
        
        if (customerIt == customers.end()) {
            cout << "\nCustomer not found!" << endl;
            return;
        }

        // Find car
        auto carIt = find_if(cars.begin(), cars.end(),
            [carID](const Car& c) { return c.getCarID() == carID; });
        
        if (carIt == cars.end()) {
            cout << "\nCar not found!" << endl;
            return;
        }

        if (!carIt->getAvailability()) {
            cout << "\nCar is not available for rent!" << endl;
            return;
        }

        // Calculate total cost
        double totalCost = carIt->getRentalPrice() * days;

        // Create rental record
        string rentalID = "R" + to_string(rentalCounter++);
        rentals.push_back(Rental(rentalID, customerID, carID, days, totalCost));

        // Update car availability
        carIt->setAvailability(false);

        cout << "\nCar rented successfully!" << endl;
        cout << "Total cost: $" << fixed << setprecision(2) << totalCost << endl;
        
        saveDataToFiles();  // Save after rental
    }

    // Return a car
    void returnCar(string carID) {
        auto carIt = find_if(cars.begin(), cars.end(),
            [carID](const Car& c) { return c.getCarID() == carID; });
        
        if (carIt == cars.end()) {
            cout << "\nCar not found!" << endl;
            return;
        }

        if (carIt->getAvailability()) {
            cout << "\nThis car is not rented!" << endl;
            return;
        }

        carIt->setAvailability(true);
        cout << "\nCar returned successfully!" << endl;
        
        saveDataToFiles();  // Save after return
    }
};

// Main function with menu-driven interface
int main() {
    RentalSystem rentalSystem;
    int choice;

    // Add some sample data
    rentalSystem.addCar("C001", "Toyota", "Camry", 50.0);
    rentalSystem.addCar("C002", "Honda", "Civic", 45.0);
    rentalSystem.addCar("C003", "BMW", "X5", 85.0);
    rentalSystem.addCustomer("CUST001", "John Doe", "1234567890");
    rentalSystem.addCustomer("CUST002", "Jane Smith", "0987654321");

    while (true) {
        cout << "\n=== Car Rental System ===" << endl;
        cout << "1. Add New Car" << endl;
        cout << "2. Add New Customer" << endl;
        cout << "3. Display All Cars" << endl;
        cout << "4. Display All Customers" << endl;
        cout << "5. Rent a Car" << endl;
        cout << "6. Return a Car" << endl;
        cout << "7. Display All Rentals" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string id, brand, model;
                double price;
                cout << "Enter Car ID: ";
                getline(cin, id);
                cout << "Enter Brand: ";
                getline(cin, brand);
                cout << "Enter Model: ";
                getline(cin, model);
                cout << "Enter Rental Price per Day: ";
                cin >> price;
                rentalSystem.addCar(id, brand, model, price);
                break;
            }
            case 2: {
                string id, name, contact;
                cout << "Enter Customer ID: ";
                getline(cin, id);
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Contact Number: ";
                getline(cin, contact);
                rentalSystem.addCustomer(id, name, contact);
                break;
            }
            case 3:
                rentalSystem.displayAllCars();
                break;
            case 4:
                rentalSystem.displayAllCustomers();
                break;
            case 5: {
                string customerID, carID;
                int days;
                cout << "Enter Customer ID: ";
                getline(cin, customerID);
                cout << "Enter Car ID: ";
                getline(cin, carID);
                cout << "Enter Rental Duration (days): ";
                cin >> days;
                rentalSystem.rentCar(customerID, carID, days);
                break;
            }
            case 6: {
                string carID;
                cout << "Enter Car ID: ";
                getline(cin, carID);
                rentalSystem.returnCar(carID);
                break;
            }
            case 7:
                rentalSystem.displayAllRentals();
                break;
            case 8:
                cout << "\nThank you for using the Car Rental System!" << endl;
                return 0;
            default:
                cout << "\nInvalid choice! Please try again." << endl;
        }
    }

    return 0;
} 