// ELEC2645 Unit 2 Project Template
// Command Line Application Menu Handling Code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "funcs.h"  

//Basketball System Includes
#define MAX_PLAYERS 20
#define MAX_NAME_LEN 50

typedef struct {
    int number;
    char name[MAX_NAME_LEN];
    int games;
    int totalPoints;
    int totalRebounds;
    int totalAssists;
    int totalSteals;
    int totalBlocks;
} Player;

Player players[MAX_PLAYERS];
int playerCount = 0;


static void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

//Basketball function prototypes
int findIndexByNumber(const Player players[], int count, int number);
void addGameStats(Player players[], int *count);
void printAllPlayers(const Player players[], int count);
void findPlayer(const Player players[], int count);
void showTeamSeasonStats(const Player players[], int count);
void saveToFile(const Player players[], int count);
void loadFromFile(Player players[], int *count);

//Original menu prototypes
static void main_menu(void);
static void print_main_menu(void);
static int  get_user_input(void);
static void select_menu_item(int input);
static void go_back_to_main(void);
static int  is_integer(const char *s);

//Main 
int main(void)
{
    for(;;)
        main_menu();

    return 0;
}

//Main Menu Controller 
static void main_menu(void)
{
    print_main_menu();
    int input = get_user_input();
    select_menu_item(input);
}

//Menu Input
static int get_user_input(void)
{
    enum { MENU_ITEMS = 7 }; 
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS)
                valid_input = 1;
            else
                printf("Invalid menu item!\n");
        }
    } while (!valid_input);

    return value;
}

 //Menu Dispatcher 
static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            addGameStats(players, &playerCount);
            go_back_to_main(); 
            break;
        case 2:
            printAllPlayers(players, playerCount);
            go_back_to_main(); 
            break;
        case 3:
            findPlayer(players, playerCount);
            go_back_to_main(); 
            break;
        case 4:
            showTeamSeasonStats(players, playerCount);
            go_back_to_main(); 
            break;
        case 5:
            saveToFile(players, playerCount);
            go_back_to_main(); 
            break;
        case 6:
            loadFromFile(players, &playerCount);
            go_back_to_main(); 
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

//Menu Layout
static void print_main_menu(void)
{
    printf("\n----------- Basketball Stats System -----------\n");
    printf("\n"
           " 1. Add Player Game Stats\n"
           " 2. Show All Players\n"
           " 3. Find Player by Jersey Number\n"
           " 4. Show Team Season Stats\n"
           " 5. Save to File\n"
           " 6. Load from File\n"
           " 7. Exit\n");
    printf("---------------------------------------------\n");
}

//Back to Menu
static void go_back_to_main(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to main menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0';
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}

//Integer Check
static int is_integer(const char *s)
{
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-') s++;
    if (!isdigit((unsigned char)*s)) return 0;

    while (*s)
        if (!isdigit((unsigned char)*s++))
            return 0;

    return 1;
}

//Basketball Stats System
int findIndexByNumber(const Player players[], int count, int number) {
    for (int i = 0; i < count; i++)
        if (players[i].number == number)
            return i;
    return -1;
}

void addGameStats(Player players[], int *count)
{
    int number;
    printf("Enter jersey number: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }

    int index = findIndexByNumber(players, *count, number);

    if (index == -1) {
        if (*count >= MAX_PLAYERS) {
            printf("Player limit reached.\n");
            clear_input_buffer();
            return;
        }

        Player p;
        p.number = number;
        p.games = 0;
        p.totalPoints = p.totalRebounds = p.totalAssists = 0;
        p.totalSteals = p.totalBlocks = 0;

        printf("Enter name: ");
        if (scanf("%49s", p.name) != 1) {
            printf("Invalid name.\n");
            clear_input_buffer();
            return;
        }

        int pts, reb, ast, stl, blk;
        printf("Enter PTS REB AST STL BLK: ");
        if (scanf("%d %d %d %d %d", &pts, &reb, &ast, &stl, &blk) != 5) {
            printf("Invalid stats.\n");
            clear_input_buffer();
            return;
        }

        p.games = 1;
        p.totalPoints = pts;
        p.totalRebounds = reb;
        p.totalAssists = ast;
        p.totalSteals = stl;
        p.totalBlocks = blk;

        players[*count] = p;
        (*count)++;

        printf("Player added.\n");
    }
    else {
        Player *p = &players[index];
        int pts, reb, ast, stl, blk;

        printf("Enter PTS REB AST STL BLK: ");
        if (scanf("%d %d %d %d %d", &pts, &reb, &ast, &stl, &blk) != 5) {
            printf("Invalid stats.\n");
            clear_input_buffer();
            return;
        }

        p->games++;
        p->totalPoints += pts;
        p->totalRebounds += reb;
        p->totalAssists += ast;
        p->totalSteals += stl;
        p->totalBlocks += blk;

        printf("Stats updated.\n");
    }

    clear_input_buffer();
}

void printAllPlayers(const Player players[], int count)
{
    if (count == 0) {
        printf("No players.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        const Player *p = &players[i];
        printf("\n#%d %s - Games: %d\n", p->number, p->name, p->games);
        printf("Totals  PTS:%d REB:%d AST:%d STL:%d BLK:%d\n",
               p->totalPoints, p->totalRebounds, p->totalAssists,
               p->totalSteals, p->totalBlocks);
    }
}

void findPlayer(const Player players[], int count)
{
    int number;
    printf("Enter jersey number: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int index = findIndexByNumber(players, count, number);
    if (index == -1) {
        printf("Player not found.\n");
        return;
    }

    const Player *p = &players[index];
    printf("\n#%d %s - Games: %d\n", p->number, p->name, p->games);
    printf("Totals  PTS:%d REB:%d AST:%d STL:%d BLK:%d\n",
           p->totalPoints, p->totalRebounds, p->totalAssists,
           p->totalSteals, p->totalBlocks);
}

void showTeamSeasonStats(const Player players[], int count)
{
    if (count == 0) {
        printf("No players.\n");
        return;
    }

    int gp=0, pts=0, reb=0, ast=0, stl=0, blk=0;

    for (int i = 0; i < count; i++) {
        gp  += players[i].games;
        pts += players[i].totalPoints;
        reb += players[i].totalRebounds;
        ast += players[i].totalAssists;
        stl += players[i].totalSteals;
        blk += players[i].totalBlocks;
    }

    printf("\n===== Team Stats =====\n");
    printf("Players: %d\n", count);
    printf("Total games (sum of player games): %d\n", gp);
    printf("Totals PTS:%d REB:%d AST:%d STL:%d BLK:%d\n",
            pts, reb, ast, stl, blk);
}

void saveToFile(const Player players[], int count)
{
    char filename[100];
    printf("Filename: ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        const Player *p = &players[i];
        fprintf(fp, "%d %s %d %d %d %d %d %d\n",
                p->number, p->name, p->games,
                p->totalPoints, p->totalRebounds, p->totalAssists,
                p->totalSteals, p->totalBlocks);
    }
    fclose(fp);
    printf("Saved.\n");
}

void loadFromFile(Player players[], int *count)
{
    char filename[100];
    printf("Filename: ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening.\n");
        return;
    }

    int n;
    if (fscanf(fp, "%d", &n) != 1) {
        printf("File format error.\n");
        fclose(fp);
        return;
    }

    if (n > MAX_PLAYERS) {
        printf("File has %d players, but max supported is %d. Truncating.\n", n, MAX_PLAYERS);
        n = MAX_PLAYERS;
    }

    for (int i = 0; i < n; i++) {
        Player p;
        if (fscanf(fp, "%d %49s %d %d %d %d %d %d",
               &p.number, p.name, &p.games,
               &p.totalPoints, &p.totalRebounds, &p.totalAssists,
               &p.totalSteals, &p.totalBlocks) != 8) {
            printf("File format error while reading player %d.\n", i + 1);
            fclose(fp);
            return;
        }

        players[i] = p;
    }

    *count = n;
    fclose(fp);

    printf("Loaded %d players.\n", *count);
}

