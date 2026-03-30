import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

// ============================================================
//        RESORT MANAGEMENT SYSTEM - Java Project
//        IDE: Visual Studio Code (Windows)
// ============================================================

// ---------- Room Class ----------
class Room {
    int number;
    String type;
    double pricePerNight;
    int capacity;
    boolean isOccupied;

    public Room(int number, String type, double pricePerNight, int capacity) {
        this.number        = number;
        this.type          = type;
        this.pricePerNight = pricePerNight;
        this.capacity      = capacity;
        this.isOccupied    = false;
    }

    public void display() {
        System.out.printf("  %-8d %-12s %-18.2f %-10d %s%n",
            number, type, pricePerNight, capacity,
            isOccupied ? "OCCUPIED" : "AVAILABLE");
    }
}

// ---------- Guest Class ----------
class Guest {
    String guestId;
    String name;
    String phone;
    int    roomNumber;
    int    nights;
    double totalBill;
    boolean active;

    public Guest(String guestId, String name, String phone,
                 int roomNumber, int nights, double totalBill) {
        this.guestId    = guestId;
        this.name       = name;
        this.phone      = phone;
        this.roomNumber = roomNumber;
        this.nights     = nights;
        this.totalBill  = totalBill;
        this.active     = true;
    }

    public void display() {
        System.out.printf("  %-10s %-20s %-14s %-7d %-8d %.2f%n",
            guestId, name, phone, roomNumber, nights, totalBill);
    }
}

// ---------- Resort Manager ----------
class ResortManager {
    private static final String RESORT_NAME = "Azure Paradise Resort";
    private List<Room>  rooms  = new ArrayList<>();
    private List<Guest> guests = new ArrayList<>();
    private Scanner     sc     = new Scanner(System.in);

    public ResortManager() {
        for (int i = 0; i < 5; i++) rooms.add(new Room(101 + i, "Deluxe", 3500.0, 2));
        for (int i = 0; i < 3; i++) rooms.add(new Room(201 + i, "Suite",  7000.0, 4));
        for (int i = 0; i < 2; i++) rooms.add(new Room(301 + i, "Villa", 15000.0, 6));
        System.out.println("  [INFO] " + rooms.size() + " rooms initialized.");
    }

    // ---- Helpers ----
    private void printLine(char c, int n) {
        System.out.println("  " + String.valueOf(c).repeat(n));
    }

    private void printLine() { printLine('-', 58); }

    private void printHeader() {
        System.out.println();
        printLine('=', 58);
        System.out.println("         *** " + RESORT_NAME + " ***");
        printLine('=', 58);
    }

    private void pressEnter() {
        System.out.print("\n  Press ENTER to continue...");
        sc.nextLine();
    }

    private Room findRoom(int number) {
        return rooms.stream().filter(r -> r.number == number).findFirst().orElse(null);
    }

    private Guest findGuest(String id) {
        return guests.stream().filter(g -> g.guestId.equals(id) && g.active).findFirst().orElse(null);
    }

    private void showAvailableRooms() {
        printHeader();
        System.out.println("  AVAILABLE ROOMS");
        printLine();
        boolean found = false;
        for (Room r : rooms) {
            if (!r.isOccupied) {
                System.out.printf("  Room %-4d | %-8s | Rs %.2f/night | Capacity: %d%n",
                    r.number, r.type, r.pricePerNight, r.capacity);
                found = true;
            }
        }
        if (!found) System.out.println("  No rooms available.");
        printLine();
    }

    // ---- Check-In ----
    public void checkIn() {
        printHeader();
        System.out.println("  CHECK-IN");
        printLine();
        showAvailableRooms();

        System.out.print("  Enter Guest ID   : ");
        String id = sc.nextLine().trim();

        if (findGuest(id) != null) {
            System.out.println("  [ERROR] Guest ID already checked in.");
            pressEnter(); return;
        }

        System.out.print("  Enter Full Name  : ");
        String name = sc.nextLine().trim();
        System.out.print("  Enter Phone      : ");
        String phone = sc.nextLine().trim();
        System.out.print("  Enter Room Number: ");
        int roomNo = Integer.parseInt(sc.nextLine().trim());

        Room room = findRoom(roomNo);
        if (room == null)        { System.out.println("  [ERROR] Room not found.");        pressEnter(); return; }
        if (room.isOccupied)     { System.out.println("  [ERROR] Room already occupied."); pressEnter(); return; }

        System.out.print("  Enter Nights     : ");
        int nights = Integer.parseInt(sc.nextLine().trim());

        double bill = room.pricePerNight * nights;
        guests.add(new Guest(id, name, phone, roomNo, nights, bill));
        room.isOccupied = true;

        System.out.println();
        printLine('*', 58);
        System.out.println("  CHECK-IN SUCCESSFUL!");
        System.out.printf("  Guest : %s  |  Room : %d  |  Nights : %d%n", name, roomNo, nights);
        System.out.printf("  Estimated Bill: Rs %.2f%n", bill);
        printLine('*', 58);
        pressEnter();
    }

    // ---- Check-Out ----
    public void checkOut() {
        printHeader();
        System.out.println("  CHECK-OUT");
        printLine();

        System.out.print("  Enter Guest ID: ");
        String id = sc.nextLine().trim();

        Guest g = findGuest(id);
        if (g == null) { System.out.println("  [ERROR] Active guest not found."); pressEnter(); return; }

        Room r = findRoom(g.roomNumber);

        System.out.println();
        printLine('*', 58);
        System.out.println("  CHECKOUT SUMMARY");
        printLine();
        System.out.println("  Guest ID   : " + g.guestId);
        System.out.println("  Name       : " + g.name);
        System.out.println("  Room No    : " + g.roomNumber);
        System.out.println("  Nights     : " + g.nights);
        System.out.println("  --------------------------------");
        System.out.printf("  TOTAL BILL : Rs %.2f%n", g.totalBill);
        printLine('*', 58);

        g.active = false;
        if (r != null) r.isOccupied = false;

        System.out.println("\n  Thank you for staying at " + RESORT_NAME + "!");
        pressEnter();
    }

    // ---- View Guests ----
    public void viewGuests() {
        printHeader();
        System.out.println("  CURRENT GUESTS");
        printLine();
        System.out.printf("  %-10s %-20s %-14s %-7s %-8s %s%n",
            "ID", "Name", "Phone", "Room", "Nights", "Bill(Rs)");
        printLine();
        boolean found = false;
        for (Guest g : guests) {
            if (g.active) { g.display(); found = true; }
        }
        if (!found) System.out.println("  No guests currently checked in.");
        printLine();
        pressEnter();
    }

    // ---- Search Guest ----
    public void searchGuest() {
        printHeader();
        System.out.println("  SEARCH GUEST");
        printLine();

        System.out.print("  Enter Guest ID: ");
        String id = sc.nextLine().trim();

        Guest g = findGuest(id);
        if (g == null) { System.out.println("  [ERROR] Guest not found."); pressEnter(); return; }

        Room r = findRoom(g.roomNumber);
        System.out.println("\n  ----- Guest Details -----");
        System.out.println("  ID        : " + g.guestId);
        System.out.println("  Name      : " + g.name);
        System.out.println("  Phone     : " + g.phone);
        System.out.println("  Room      : " + g.roomNumber + " (" + (r != null ? r.type : "?") + ")");
        System.out.println("  Nights    : " + g.nights);
        System.out.printf("  Total Bill: Rs %.2f%n", g.totalBill);
        pressEnter();
    }

    // ---- View All Rooms ----
    public void viewAllRooms() {
        printHeader();
        System.out.printf("  %-8s %-12s %-18s %-10s %s%n",
            "Room No", "Type", "Price/Night(INR)", "Capacity", "Status");
        printLine();
        for (Room r : rooms) r.display();
        printLine();
        pressEnter();
    }

    // ---- Occupancy Report ----
    public void occupancyReport() {
        printHeader();
        System.out.println("  OCCUPANCY REPORT");
        printLine();

        long occupied = rooms.stream().filter(r -> r.isOccupied).count();
        double revenue = guests.stream().filter(g -> g.active).mapToDouble(g -> g.totalBill).sum();

        System.out.println("  Total Rooms   : " + rooms.size());
        System.out.println("  Occupied      : " + occupied);
        System.out.println("  Available     : " + (rooms.size() - occupied));
        System.out.printf( "  Occupancy %%   : %.1f%%%n", 100.0 * occupied / rooms.size());
        System.out.printf( "  Expected Rev  : Rs %.2f%n", revenue);
        printLine();
        pressEnter();
    }

    // ---- Main Menu ----
    public void run() {
        int choice;
        do {
            printHeader();
            System.out.println();
            System.out.println("  [1]  Check-In Guest");
            System.out.println("  [2]  Check-Out Guest");
            System.out.println("  [3]  View All Guests");
            System.out.println("  [4]  Search Guest");
            System.out.println("  [5]  View All Rooms");
            System.out.println("  [6]  View Available Rooms");
            System.out.println("  [7]  Occupancy Report");
            System.out.println("  [0]  Exit");
            printLine();
            System.out.print("  Enter Choice: ");
            choice = Integer.parseInt(sc.nextLine().trim());

            switch (choice) {
                case 1 -> checkIn();
                case 2 -> checkOut();
                case 3 -> viewGuests();
                case 4 -> searchGuest();
                case 5 -> viewAllRooms();
                case 6 -> { showAvailableRooms(); pressEnter(); }
                case 7 -> occupancyReport();
                case 0 -> System.out.println("\n  Thank you! Visit " + RESORT_NAME + " again!\n");
                default -> { System.out.println("  Invalid choice."); pressEnter(); }
            }
        } while (choice != 0);
    }
}

// ---------- Main Class ----------
public class ResortManagement {
    public static void main(String[] args) {
        System.out.println("\n  Initializing Azure Paradise Resort...");
        ResortManager manager = new ResortManager();
        manager.run();
    }
}
