/*
 * ============================================================
 *       RESORT MANAGEMENT SYSTEM - C++ Mini Project
 * ============================================================
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

const string RESORT_NAME = "Azure Paradise Resort";

// ==================== Classes ====================

class Room {
public:
    int    number;
    string type;
    double pricePerNight;
    int    capacity;
    bool   isOccupied;

    Room(int num, string t, double price, int cap)
        : number(num), type(t), pricePerNight(price),
          capacity(cap), isOccupied(false) {}

    void display() const {
        cout << "  " << left
             << setw(8)  << number
             << setw(12) << type
             << setw(18) << fixed << setprecision(2) << pricePerNight
             << setw(10) << capacity
             << (isOccupied ? "OCCUPIED" : "AVAILABLE")
             << "\n";
    }
};

class Guest {
public:
    string guestId;
    string name;
    string phone;
    int    roomNumber;
    int    nights;
    double totalBill;
    bool   active;

    Guest(string id, string n, string ph, int room, int nts, double bill)
        : guestId(id), name(n), phone(ph), roomNumber(room),
          nights(nts), totalBill(bill), active(true) {}

    void display() const {
        cout << "  " << left
             << setw(10) << guestId
             << setw(20) << name
             << setw(14) << phone
             << setw(7)  << roomNumber
             << setw(8)  << nights
             << fixed << setprecision(2) << totalBill
             << "\n";
    }
};

// ==================== Resort Manager ====================

class ResortManager {
    vector<Room>  rooms;
    vector<Guest> guests;

    void printLine(char c = '-', int n = 60) const {
        cout << string(n, c) << "\n";
    }

    void printHeader() const {
        cout << "\n";
        printLine('=');
        cout << "         *** " << RESORT_NAME << " ***\n";
        printLine('=');
    }

    void pressEnter() const {
        cout << "\n  Press ENTER to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    Room* findRoom(int number) {
        for (auto& r : rooms)
            if (r.number == number) return &r;
        return nullptr;
    }

    Guest* findGuest(const string& id) {
        for (auto& g : guests)
            if (g.guestId == id && g.active) return &g;
        return nullptr;
    }

    void showAvailableRooms() const {
        printHeader();
        cout << "  AVAILABLE ROOMS\n";
        printLine();
        bool found = false;
        for (const auto& r : rooms) {
            if (!r.isOccupied) {
                cout << "  Room " << r.number
                     << " | " << setw(8) << left << r.type
                     << " | Rs " << fixed << setprecision(2) << r.pricePerNight
                     << "/night | Capacity: " << r.capacity << "\n";
                found = true;
            }
        }
        if (!found) cout << "  No rooms available.\n";
        printLine();
    }

public:
    ResortManager() {
        // 5 Deluxe
        for (int i = 0; i < 5; i++)
            rooms.emplace_back(101 + i, "Deluxe", 3500.0, 2);
        // 3 Suites
        for (int i = 0; i < 3; i++)
            rooms.emplace_back(201 + i, "Suite", 7000.0, 4);
        // 2 Villas
        for (int i = 0; i < 2; i++)
            rooms.emplace_back(301 + i, "Villa", 15000.0, 6);

        cout << "  [INFO] " << rooms.size() << " rooms initialized.\n";
    }

    // ---------- Check-In ----------
    void checkIn() {
        printHeader();
        cout << "  CHECK-IN\n";
        printLine();
        showAvailableRooms();

        string id, name, phone;
        int roomNo, nights;

        cout << "  Enter Guest ID   : "; cin >> id;
        if (findGuest(id)) {
            cout << "  [ERROR] Guest ID already checked in.\n";
            pressEnter(); return;
        }

        cin.ignore();
        cout << "  Enter Full Name  : "; getline(cin, name);
        cout << "  Enter Phone      : "; cin >> phone;
        cout << "  Enter Room Number: "; cin >> roomNo;

        Room* room = findRoom(roomNo);
        if (!room) { cout << "  [ERROR] Room not found.\n"; pressEnter(); return; }
        if (room->isOccupied) { cout << "  [ERROR] Room already occupied.\n"; pressEnter(); return; }

        cout << "  Enter Nights     : "; cin >> nights;

        double bill = room->pricePerNight * nights;
        guests.emplace_back(id, name, phone, roomNo, nights, bill);
        room->isOccupied = true;

        cout << "\n";
        printLine('*');
        cout << "  CHECK-IN SUCCESSFUL!\n";
        cout << "  Guest : " << name << "  |  Room : " << roomNo << "  |  Nights : " << nights << "\n";
        cout << "  Estimated Bill: Rs " << fixed << setprecision(2) << bill << "\n";
        printLine('*');
        pressEnter();
    }

    // ---------- Check-Out ----------
    void checkOut() {
        printHeader();
        cout << "  CHECK-OUT\n";
        printLine();

        string id;
        cout << "  Enter Guest ID: "; cin >> id;

        Guest* g = findGuest(id);
        if (!g) { cout << "  [ERROR] Active guest not found.\n"; pressEnter(); return; }

        Room* r = findRoom(g->roomNumber);

        cout << "\n";
        printLine('*');
        cout << "  CHECKOUT SUMMARY\n";
        printLine();
        cout << "  Guest ID   : " << g->guestId  << "\n";
        cout << "  Name       : " << g->name      << "\n";
        cout << "  Room No    : " << g->roomNumber << "\n";
        cout << "  Nights     : " << g->nights    << "\n";
        cout << "  --------------------------------\n";
        cout << "  TOTAL BILL : Rs " << fixed << setprecision(2) << g->totalBill << "\n";
        printLine('*');

        g->active = false;
        if (r) r->isOccupied = false;

        cout << "\n  Thank you for staying at " << RESORT_NAME << "!\n";
        pressEnter();
    }

    // ---------- View All Guests ----------
    void viewGuests() const {
        printHeader();
        cout << "  CURRENT GUESTS\n";
        printLine();
        cout << "  " << left
             << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(14) << "Phone"
             << setw(7)  << "Room"
             << setw(8)  << "Nights"
             << "Bill(Rs)\n";
        printLine();

        bool found = false;
        for (const auto& g : guests) {
            if (g.active) { g.display(); found = true; }
        }
        if (!found) cout << "  No guests currently checked in.\n";
        printLine();
        pressEnter();
    }

    // ---------- Search Guest ----------
    void searchGuest() {
        printHeader();
        cout << "  SEARCH GUEST\n";
        printLine();

        string id;
        cout << "  Enter Guest ID: "; cin >> id;

        Guest* g = findGuest(id);
        if (!g) { cout << "  [ERROR] Guest not found.\n"; pressEnter(); return; }

        Room* r = findRoom(g->roomNumber);
        cout << "\n  ----- Guest Details -----\n";
        cout << "  ID        : " << g->guestId    << "\n";
        cout << "  Name      : " << g->name        << "\n";
        cout << "  Phone     : " << g->phone       << "\n";
        cout << "  Room      : " << g->roomNumber  << " (" << (r ? r->type : "?") << ")\n";
        cout << "  Nights    : " << g->nights      << "\n";
        cout << "  Total Bill: Rs " << fixed << setprecision(2) << g->totalBill << "\n";
        pressEnter();
    }

    // ---------- View All Rooms ----------
    void viewAllRooms() const {
        printHeader();
        cout << "  " << left
             << setw(8)  << "Room No"
             << setw(12) << "Type"
             << setw(18) << "Price/Night(INR)"
             << setw(10) << "Capacity"
             << "Status\n";
        printLine();
        for (const auto& r : rooms) r.display();
        printLine();
        pressEnter();
    }

    // ---------- Occupancy Report ----------
    void occupancyReport() const {
        printHeader();
        cout << "  OCCUPANCY REPORT\n";
        printLine();

        int occupied = 0;
        double revenue = 0.0;
        for (const auto& r : rooms) if (r.isOccupied) occupied++;
        for (const auto& g : guests) if (g.active) revenue += g.totalBill;

        int total = rooms.size();
        cout << "  Total Rooms   : " << total << "\n";
        cout << "  Occupied      : " << occupied << "\n";
        cout << "  Available     : " << (total - occupied) << "\n";
        cout << "  Occupancy %   : " << fixed << setprecision(1)
             << (100.0 * occupied / total) << "%\n";
        cout << "  Expected Rev  : Rs " << fixed << setprecision(2) << revenue << "\n";
        printLine();
        pressEnter();
    }

    // ---------- Main Menu ----------
    void run() {
        int choice;
        do {
            printHeader();
            cout << "\n";
            cout << "  [1]  Check-In Guest\n";
            cout << "  [2]  Check-Out Guest\n";
            cout << "  [3]  View All Guests\n";
            cout << "  [4]  Search Guest\n";
            cout << "  [5]  View All Rooms\n";
            cout << "  [6]  View Available Rooms\n";
            cout << "  [7]  Occupancy Report\n";
            cout << "  [0]  Exit\n";
            printLine();
            cout << "  Enter Choice: ";
            cin >> choice;

            switch (choice) {
                case 1: checkIn();           break;
                case 2: checkOut();          break;
                case 3: viewGuests();        break;
                case 4: searchGuest();       break;
                case 5: viewAllRooms();      break;
                case 6:
                    showAvailableRooms();
                    pressEnter();
                    break;
                case 7: occupancyReport();   break;
                case 0:
                    cout << "\n  Thank you! Visit " << RESORT_NAME << " again!\n\n";
                    break;
                default:
                    cout << "  Invalid choice.\n";
                    pressEnter();
            }
        } while (choice != 0);
    }
};

// ==================== Entry Point ====================

int main() {
    cout << "\n  Initializing " << RESORT_NAME << "...\n";
    ResortManager manager;
    manager.run();
    return 0;
}
