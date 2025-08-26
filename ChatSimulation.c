#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SAVE_FILE "player_save.dat"

struct Player {
    char name[50];
    int hp;
    int max_hp;
    int attack;
    int monsters_defeated;
};

// --- Function Prototypes ---
void create_new_player(struct Player *p);
void save_player(const struct Player *p);
int load_player(struct Player *p);
void game_loop(struct Player *p);
void explore(struct Player *p);
void fight(struct Player *p);
void clear_input_buffer();

// --- Main Function ---
int main() {
    srand(time(NULL)); // Seed the random number generator
    struct Player player;

    if (load_player(&player)) {
        printf("Welcome back, %s!\n", player.name);
    } else {
        printf("Welcome, new adventurer!\n");
        create_new_player(&player);
        save_player(&player);
    }

    game_loop(&player);
    return 0;
}

// --- Game Loop and Actions ---

void game_loop(struct Player *p) {
    int choice;
    while (1) {
        printf("\n--- %s | HP: %d/%d | ATK: %d | Defeated: %d ---\n",
               p->name, p->hp, p->max_hp, p->attack, p->monsters_defeated);
        printf("What will you do?\n");
        printf("1. Explore\n");
        printf("2. Rest (Restore HP)\n");
        printf("3. View Stats\n");
        printf("4. Save and Exit\n");
        printf("--------------------------------------------------\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: explore(p); break;
            case 2:
                p->hp = p->max_hp;
                printf("You feel rested and fully restored.\n");
                save_player(p);
                break;
            case 3:
                printf("\nPlayer: %s\nHP: %d/%d\nAttack: %d\nMonsters Defeated: %d\n",
                       p->name, p->hp, p->max_hp, p->attack, p->monsters_defeated);
                break;
            case 4:
                printf("Your progress has been saved. Goodbye!\n");
                exit(0);
            default: printf("Invalid choice.\n");
        }

        if (p->hp <= 0) {
            printf("\nYou have been defeated. Game Over.\n");
            remove(SAVE_FILE); // Delete save file on death
            exit(0);
        }
    }
}

void explore(struct Player *p) {
    printf("\nYou venture into the unknown...\n");
    int event = rand() % 3; // 0: Monster, 1: Treasure, 2: Nothing

    switch (event) {
        case 0:
            printf("A wild monster appears!\n");
            fight(p);
            break;
        case 1:
            printf("You found a treasure chest! Your attack power increases!\n");
            p->attack += 2;
            save_player(p);
            break;
        case 2:
            printf("The area is quiet. You find nothing of interest.\n");
            break;
    }
}

void fight(struct Player *p) {
    int monster_hp = (rand() % 20) + 10; // Monster HP between 10 and 29
    int monster_atk = (rand() % 5) + 3;  // Monster ATK between 3 and 7
    printf("A monster with %d HP and %d ATK stands before you!\n", monster_hp, monster_atk);

    while (p->hp > 0 && monster_hp > 0) {
        printf("\nYour turn! (1. Attack, 2. Run): ");
        int choice;
        scanf("%d", &choice);
        clear_input_buffer();

        if (choice == 1) {
            int player_damage = (rand() % p->attack) + 1;
            monster_hp -= player_damage;
            printf("You hit the monster for %d damage! Monster HP: %d\n", player_damage, monster_hp);

            if (monster_hp > 0) {
                int monster_damage = (rand() % monster_atk) + 1;
                p->hp -= monster_damage;
                printf("The monster hits you for %d damage! Your HP: %d\n", monster_damage, p->hp);
            }
        } else if (choice == 2) {
            if ((rand() % 2) == 0) {
                printf("You successfully ran away!\n");
                return;
            } else {
                printf("You failed to run away!\n");
                int monster_damage = (rand() % monster_atk) + 1;
                p->hp -= monster_damage;
                printf("The monster hits you for %d damage! Your HP: %d\n", monster_damage, p->hp);
            }
        } else {
            printf("Invalid action!\n");
        }
    }

    if (p->hp > 0) {
        printf("\nYou defeated the monster!\n");
        p->monsters_defeated++;
        if (p->monsters_defeated % 3 == 0) {
            p->max_hp += 5;
            p->attack += 1;
            printf("You feel stronger! Max HP and Attack increased!\n");
        }
        save_player(p);
    }
}

// --- Player and File Management ---

void create_new_player(struct Player *p) {
    printf("Enter your hero's name: ");
    fgets(p->name, 50, stdin);
    p->name[strcspn(p->name, "\n")] = 0;
    p->max_hp = 50;
    p->hp = 50;
    p->attack = 5;
    p->monsters_defeated = 0;
}

void save_player(const struct Player *p) {
    FILE *fp = fopen(SAVE_FILE, "wb");
    if (fp) {
        fwrite(p, sizeof(struct Player), 1, fp);
        fclose(fp);
    }
}

int load_player(struct Player *p) {
    FILE *fp = fopen(SAVE_FILE, "rb");
    if (fp) {
        fread(p, sizeof(struct Player), 1, fp);
        fclose(fp);
        return 1;
    }
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
