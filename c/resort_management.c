/*
 * ============================================================
 *         RESORT MANAGEMENT SYSTEM - C Mini Project
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS      50
#define MAX_GUESTS     100
#define MAX_NAME       50
#define MAX_ID         20
#define MAX_PHONE      15
#define RESORT_NAME    "Azure Paradise Resort"

/* -------------------- Structures -------------------- */

typedef struct {
    int    room_number;
    char   type[20];       /* Deluxe, Suite, Villa */
    float  price_per_night;
    int    is_occupied;
    int    capacity;
} Room;

typedef struct {
    char  guest_id[MAX_ID];
    char  name[MAX_NAME];
    char  phone[MAX_PHONE];
    int   room_number;
    int   nights;
    float total_bill;
    int   active;          /* 1 = checked-in, 0 = checked-out */
} Guest;

/* -------------------- Global Data ------------------- */

Room  rooms[MAX_ROOMS];
Guest guests[MAX_GUESTS];
int   room_count  = 0;
int   guest_count = 0;

/* ==================== Utilities ==================== */

void clear_screen() {
    printf("\033[2J\033[H");   /* ANSI clear */
}

void print_line(char c, int n) {
    for (int i = 0; i < n; i++) putchar(c);
    putchar('\n');
}

void print_header() {
    printf("\n");
    print_line('=', 60);
    printf("         *** %s ***\n", RESORT_NAME);
    print_line('=', 60);
}

void press_enter() {
    printf("\n  Press ENTER to continue...");
    getchar(); getchar();
}

/* ==================== Init Rooms ==================== */

void init_rooms() {
    /* 5 Deluxe Rooms */
    for (int i = 0; i < 5; i++) {
        rooms[i].room_number     = 101 + i;
        strcpy(rooms[i].type,    "Deluxe");
        rooms[i].price_per_night = 3500.0f;
        rooms[i].is_occupied     = 0;
        rooms[i].capacity        = 2;
    }
    /* 3 Suite Rooms */
    for (int i = 5; i < 8; i++) {
        rooms[i].room_number     = 201 + (i - 5);
        strcpy(rooms[i].type,    "Suite");
        rooms[i].price_per_night = 7000.0f;
        rooms[i].is_occupied     = 0;
        rooms[i].capacity        = 4;
    }
    /* 2 Villas */
    for (int i = 8; i < 10; i++) {
        rooms[i].room_number     = 301 + (i - 8);
        strcpy(rooms[i].type,    "Villa");
        rooms[i].price_per_night = 15000.0f;
        rooms[i].is_occupied     = 0;
        rooms[i].capacity        = 6;
    }
    room_count = 10;
    printf("  [INFO] %d rooms initialized.\n", room_count);
}

/* ==================== Room Functions ==================== */

void display_all_rooms() {
    print_header();
    printf("  %-8s %-12s %-18s %-10s %-10s\n",
           "Room No", "Type", "Price/Night(INR)", "Capacity", "Status");
    print_line('-', 60);
    for (int i = 0; i < room_count; i++) {
        printf("  %-8d %-12s %-18.2f %-10d %-10s\n",
               rooms[i].room_number,
               rooms[i].type,
               rooms[i].price_per_night,
               rooms[i].capacity,
               rooms[i].is_occupied ? "OCCUPIED" : "AVAILABLE");
    }
    print_line('-', 60);
}

int find_room(int number) {
    for (int i = 0; i < room_count; i++)
        if (rooms[i].room_number == number) return i;
    return -1;
}

void display_available_rooms() {
    print_header();
    printf("  AVAILABLE ROOMS\n");
    print_line('-', 60);
    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (!rooms[i].is_occupied) {
            printf("  Room %d | %-8s | Rs %.2f/night | Capacity: %d\n",
                   rooms[i].room_number,
                   rooms[i].type,
                   rooms[i].price_per_night,
                   rooms[i].capacity);
            found++;
        }
    }
    if (!found) printf("  No rooms available at the moment.\n");
    print_line('-', 60);
}

/* ==================== Guest Functions ==================== */

int find_guest_by_id(const char *id) {
    for (int i = 0; i < guest_count; i++)
        if (strcmp(guests[i].guest_id, id) == 0 && guests[i].active)
            return i;
    return -1;
}

void check_in() {
    print_header();
    printf("  CHECK-IN\n");
    print_line('-', 60);

    display_available_rooms();

    if (guest_count >= MAX_GUESTS) {
        printf("  [ERROR] Guest list full!\n");
        press_enter(); return;
    }

    Guest g;
    int room_no;

    printf("  Enter Guest ID   : "); scanf("%s", g.guest_id);
    if (find_guest_by_id(g.guest_id) != -1) {
        printf("  [ERROR] Guest ID already checked in.\n");
        press_enter(); return;
    }

    printf("  Enter Full Name  : "); scanf(" %[^\n]", g.name);
    printf("  Enter Phone      : "); scanf("%s", g.phone);
    printf("  Enter Room Number: "); scanf("%d", &room_no);

    int ri = find_room(room_no);
    if (ri == -1) { printf("  [ERROR] Room not found.\n"); press_enter(); return; }
    if (rooms[ri].is_occupied) { printf("  [ERROR] Room already occupied.\n"); press_enter(); return; }

    printf("  Enter Nights     : "); scanf("%d", &g.nights);

    g.room_number = room_no;
    g.total_bill  = rooms[ri].price_per_night * g.nights;
    g.active      = 1;

    guests[guest_count++] = g;
    rooms[ri].is_occupied = 1;

    printf("\n");
    print_line('*', 60);
    printf("  CHECK-IN SUCCESSFUL!\n");
    printf("  Guest : %s  |  Room : %d  |  Nights : %d\n", g.name, room_no, g.nights);
    printf("  Estimated Bill: Rs %.2f\n", g.total_bill);
    print_line('*', 60);
    press_enter();
}

void check_out() {
    print_header();
    printf("  CHECK-OUT\n");
    print_line('-', 60);

    char id[MAX_ID];
    printf("  Enter Guest ID: "); scanf("%s", id);

    int gi = find_guest_by_id(id);
    if (gi == -1) { printf("  [ERROR] Active guest not found.\n"); press_enter(); return; }

    Guest *g = &guests[gi];
    int ri   = find_room(g->room_number);

    printf("\n");
    print_line('*', 60);
    printf("  CHECKOUT SUMMARY\n");
    print_line('-', 60);
    printf("  Guest ID   : %s\n",      g->guest_id);
    printf("  Name       : %s\n",      g->name);
    printf("  Room No    : %d\n",      g->room_number);
    printf("  Nights     : %d\n",      g->nights);
    printf("  --------------------------------\n");
    printf("  TOTAL BILL : Rs %.2f\n", g->total_bill);
    print_line('*', 60);

    g->active = 0;
    if (ri != -1) rooms[ri].is_occupied = 0;

    printf("\n  Thank you for staying at %s!\n", RESORT_NAME);
    press_enter();
}

void view_all_guests() {
    print_header();
    printf("  CURRENT GUESTS\n");
    print_line('-', 60);
    printf("  %-10s %-20s %-12s %-6s %-12s %-10s\n",
           "ID", "Name", "Phone", "Room", "Nights", "Bill(Rs)");
    print_line('-', 60);

    int found = 0;
    for (int i = 0; i < guest_count; i++) {
        if (guests[i].active) {
            printf("  %-10s %-20s %-12s %-6d %-12d %-10.2f\n",
                   guests[i].guest_id,
                   guests[i].name,
                   guests[i].phone,
                   guests[i].room_number,
                   guests[i].nights,
                   guests[i].total_bill);
            found++;
        }
    }
    if (!found) printf("  No guests currently checked in.\n");
    print_line('-', 60);
    press_enter();
}

void search_guest() {
    print_header();
    printf("  SEARCH GUEST\n");
    print_line('-', 60);

    char id[MAX_ID];
    printf("  Enter Guest ID: "); scanf("%s", id);

    int gi = find_guest_by_id(id);
    if (gi == -1) { printf("  [ERROR] Guest not found or not active.\n"); press_enter(); return; }

    Guest *g = &guests[gi];
    int ri   = find_room(g->room_number);

    printf("\n  ----- Guest Details -----\n");
    printf("  ID        : %s\n",      g->guest_id);
    printf("  Name      : %s\n",      g->name);
    printf("  Phone     : %s\n",      g->phone);
    printf("  Room      : %d (%s)\n", g->room_number, (ri != -1) ? rooms[ri].type : "?");
    printf("  Nights    : %d\n",      g->nights);
    printf("  Total Bill: Rs %.2f\n", g->total_bill);
    press_enter();
}

/* ==================== Reports ==================== */

void occupancy_report() {
    print_header();
    printf("  OCCUPANCY REPORT\n");
    print_line('-', 60);

    int occupied = 0, available = 0;
    float revenue = 0.0f;

    for (int i = 0; i < room_count; i++) {
        if (rooms[i].is_occupied) occupied++;
        else available++;
    }
    for (int i = 0; i < guest_count; i++)
        if (guests[i].active) revenue += guests[i].total_bill;

    printf("  Total Rooms   : %d\n", room_count);
    printf("  Occupied      : %d\n", occupied);
    printf("  Available     : %d\n", available);
    printf("  Occupancy %%   : %.1f%%\n", (float)occupied / room_count * 100);
    printf("  Expected Rev  : Rs %.2f\n", revenue);
    print_line('-', 60);
    press_enter();
}

/* ==================== Main Menu ==================== */

void main_menu() {
    int choice;
    do {
        clear_screen();
        print_header();
        printf("\n");
        printf("  [1]  Check-In Guest\n");
        printf("  [2]  Check-Out Guest\n");
        printf("  [3]  View All Guests\n");
        printf("  [4]  Search Guest\n");
        printf("  [5]  View All Rooms\n");
        printf("  [6]  View Available Rooms\n");
        printf("  [7]  Occupancy Report\n");
        printf("  [0]  Exit\n");
        print_line('-', 60);
        printf("  Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: clear_screen(); check_in();           break;
            case 2: clear_screen(); check_out();          break;
            case 3: clear_screen(); view_all_guests();    break;
            case 4: clear_screen(); search_guest();       break;
            case 5: clear_screen(); display_all_rooms();  press_enter(); break;
            case 6: clear_screen(); display_available_rooms(); press_enter(); break;
            case 7: clear_screen(); occupancy_report();   break;
            case 0:
                printf("\n  Thank you! Visit %s again!\n\n", RESORT_NAME);
                break;
            default:
                printf("  Invalid choice.\n");
                press_enter();
        }
    } while (choice != 0);
}

/* ==================== Entry Point ==================== */

int main() {
    printf("\n  Initializing %s...\n", RESORT_NAME);
    init_rooms();
    main_menu();
    return 0;
}
