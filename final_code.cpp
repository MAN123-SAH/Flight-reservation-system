#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cstring>

using namespace std;

void loadingEffect(string message) {
    cout << message;
    for (int i = 0; i < 5; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << " ✅ Done!" << endl;
}

class Passenger {
public:
    string name;
    string passportNumber;

    Passenger(string n, string p) {
        name=n;
        passportNumber=p;
        }
};

class Flight {
public:
    string flightNumber;
    string origin;
    string destination;
    int totalSeats;
    int availableSeats;
    double price;
    vector<Passenger> passengers;

    Flight(string fNum, string ori, string dest, int seats, double p){
         flightNumber=fNum;
         origin=ori;
         destination=dest;
          totalSeats=seats;
          availableSeats=seats; 
          price=p ;
        
    }

    bool bookSeat(Passenger p) {
        for (const auto &existingPassenger : passengers) {
            if (existingPassenger.passportNumber == p.passportNumber) {
                cout << "🚫 Passenger already booked on this flight!" << endl;
                return false;
            }
        }

        if (availableSeats > 0) {
            passengers.push_back(p);
            availableSeats--;
            loadingEffect("Processing your ticket");
            cout << "✅ Seat booked for " << p.name << " on Flight " << flightNumber << "! ✈" << endl;
            return true;
        } else {
            cout << "❌ Sorry, no seats available on Flight " << flightNumber << "." << endl;
            return false;
        }
    }

    bool cancelSeat(Passenger p) {
        auto it = find_if(passengers.begin(), passengers.end(), [&](const Passenger &passenger) {
            return passenger.passportNumber == p.passportNumber;
        });

        if (it != passengers.end()) {
            passengers.erase(it);
            availableSeats++;
            loadingEffect("Processing cancellation");
            cout << "❌ Booking canceled for " << p.name << " on Flight " << flightNumber << "." << endl;
            return true;
        } else {
            cout << "🚫 Passenger not found on Flight " << flightNumber << "." << endl;
            return false;
        }
    }

    void displayFlightDetails() const {
        cout << left << setw(15) << flightNumber
             << left << setw(15) << origin
             << left << setw(15) << destination
             << left << setw(7) << availableSeats << "/" << setw(4) << totalSeats
             << " $" << fixed << setprecision(2) << price << endl;
    }
};

class Airline {
public:
    string name;
    vector<Flight> flights;

    Airline(string n) {
        name=n ;
        
    }

    void addFlight(const Flight &f) {
        flights.push_back(f);
    }

    void displayFlights() const {
        cout << "\n🌍 Available Flights in " << name << ":\n";
        cout << "-------------------------------------------------------------------\n";
        cout << left << setw(15) << "Flight"
             << setw(15) << "Origin"
             << setw(15) << "Destination"
             << setw(12) << "Seats"
             << "Price" << endl;
        cout << "-------------------------------------------------------------------\n";
        for (const auto &flight : flights) {
            flight.displayFlightDetails();
        }
        cout << "-------------------------------------------------------------------\n";
    }

    Flight* getFlightByNumber(const string &flightNumber) {
        for (auto &flight : flights) {
            if (flight.flightNumber == flightNumber)
                return &flight;
        }
        return nullptr;
    }
};

class Reservation {
public:
    void makeReservation(Passenger p, Flight &f) {
        f.bookSeat(p);
    }

    void cancelReservation(Passenger p, Flight &f) {
        f.cancelSeat(p);
    }
};

void showMenu() {
    cout << "\n✈  SKY RESERVATION SYSTEM MENU ✈\n";
    cout << "1️⃣  Book a Flight\n";
    cout << "2️⃣  Cancel a Booking\n";
    cout << "3️⃣  View Available Flights\n";
    cout << "4️⃣  Exit\n";
    cout << "👉 Enter your choice: ";
}

int main() {
    Airline airline("Sky Airlines");
    Reservation reservationSystem;

    airline.addFlight(Flight("SA101", "New York", "London", 5, 899.90));
    airline.addFlight(Flight("SA102", "Los Angeles", "Tokyo", 3, 1000.90));

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        if (choice == 1) {
            string name, passport, flightNumber;

            cin.ignore(); 
            cout << "Enter your name: ";
            getline(cin, name);
            cout << "Enter your passport number: ";
            cin >> passport;

            airline.displayFlights();
            cout << "Select a flight by entering the Flight Number: ";
            cin >> flightNumber;

            Flight* flight = airline.getFlightByNumber(flightNumber);
            if (flight) {
                cout << "💲 Price per ticket: $" << fixed << setprecision(2) << flight->price << endl;
                cout << "Confirm booking? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    Passenger p(name, passport);
                    reservationSystem.makeReservation(p, *flight);
                } else {
                    cout << "❌ Booking cancelled by user." << endl;
                }
            } else {
                cout << "🚫 Flight not found. Please enter a valid flight number.\n";
            }

        } else if (choice == 2) {
            string passport, flightNumber;
            cout << "Enter your passport number: ";
            cin >> passport;
            airline.displayFlights();
            cout << "Select a flight by entering the Flight Number: ";
            cin >> flightNumber;

            Flight* flight = airline.getFlightByNumber(flightNumber);
            if (flight) {
                Passenger p("", passport);
                reservationSystem.cancelReservation(p, *flight);
            } else {
                cout << "🚫 Flight not found. Please enter a valid flight number.\n";
            }

        } else if (choice == 3) {
            airline.displayFlights();
        } else if (choice == 4) {
            cout << "✈  Thank you for using Sky Airlines Reservation System! 🌍\n";
            break;
        } else {
            cout << "❌ Invalid choice. Please try again!\n";
        }
    }
    return 0;
}
