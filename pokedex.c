// Assignment 2 19T1 COMP1511: Pokedex
// pokedex.c
//
// This program was written by Macauley Dallas (z5232924)
// on 27/04/19
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Minor clarifications about `struct pokenode`.
// Version 1.1.0: Moved destroy_pokedex function to correct location.
// Version 1.1.1: Renamed "pokemon_id" to "id" in change_current_pokemon.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.

// But note you are not permitted to use functions from string.h
// so do not #include <string.h>

#include "pokedex.h"

// Add your own #defines here.

// Note you are not permitted to use arrays in struct pokedex,
// you must use a linked list.
//
// You are also not permitted to use arrays in any of the functions
// below.
//
// The only exception is that char arrays are permitted for
// search_pokemon and functions it may call, as well as the string
// returned by pokemon_name (from pokemon.h).
//
// You will need to add fields to struct pokedex.
// You may change or delete the head field.

struct pokedex {
    struct pokenode *head;
    struct pokenode *selectedPokenode;
    struct pokenode *leadingNode;
    int numPokemon;
};


// You don't have to use the provided struct pokenode, you are free to
// make your own struct instead.
// If you use the provided struct pokenode, you will need to add fields
// to it to store other information.

struct pokenode {
    struct pokenode *prev;
    struct pokenode *next;
    int found;
    Pokemon pokemon;
    int evolveInto;
};

// Add any other structs you define here.


// Add prototypes for any extra functions you create here.
static struct pokenode *new_pokenode(Pokemon pokemon, struct pokenode *node);
static void print_name_if_found(struct pokenode *node);
static struct pokenode *get_pokenode(Pokedex pokedex, int id);
static void print_id_if_found(struct pokenode *node);
static void print_type(struct pokenode *node, int failType);
static int compare_strings(const char *one, const char *two);
static int find_in_string(char *str, char *substr);
static void poketails(struct pokenode *node1, struct pokenode *node2);
static void swap_pokenodes(struct pokenode *node1, struct pokenode *node2,
    Pokedex pokedex);
static Pokedex sort_pokedex_by_id(struct pokedex *pokedex);
static int upper_case(int character);
static int does_pokemon_exist(Pokedex pokedex, Pokemon pokemon);

// You need to implement the following 20 functions.
// In other words, replace the lines calling fprintf & exit with your code.
// You can find descriptions of what each function should do in pokedex.h

Pokedex new_pokedex(void) {
    Pokedex new_pokedex = malloc(sizeof (struct pokedex));
    assert(new_pokedex != NULL);
    new_pokedex->numPokemon = 0;
    new_pokedex->head = NULL;
    new_pokedex->selectedPokenode = NULL;
    return new_pokedex;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    if (does_pokemon_exist(pokedex, pokemon) == 1) {
        fprintf(stderr, "Exiting because the pokemon already exists\n");
        exit(1);
    }
    struct pokenode *node;
    if (pokedex->head == NULL) {
        node = NULL;
    } else {
        node = pokedex->leadingNode;
    }
    //Make new pokenode
    struct pokenode *newPokenode = new_pokenode(pokemon, node);
    if (pokedex->numPokemon == 0) {
        pokedex->head = newPokenode;
        pokedex->selectedPokenode = newPokenode;
    }
    pokedex->numPokemon++;
    pokedex->leadingNode = newPokenode;
}

void detail_pokemon(Pokedex pokedex) {
    struct pokenode *selectedPokenode = pokedex->selectedPokenode;
    if (selectedPokenode != NULL) {
        if (selectedPokenode->found == 0) {
            printf("Id: %003d\n", pokemon_id(selectedPokenode->pokemon));
            printf("Name: ");
            print_name_if_found(selectedPokenode);
            printf("\n");
            printf("Height: --\n");
            printf("Weight: --\n");
            printf("Type: --\n");
        } else {
            printf("Id: %003d\n", pokemon_id(selectedPokenode->pokemon));
            printf("Name: %s\n", pokemon_name(selectedPokenode->pokemon));
            printf("Height: %.1lfm\n", pokemon_height(selectedPokenode->pokemon));
            printf("Weight: %.1lfkg\n", pokemon_weight(selectedPokenode->pokemon));
            print_type(selectedPokenode, 1);
        }
    }

    // printf("Type %s \n", pokemon_first_type(pokedex->selectedPokenode->pokemon));

    // printf("Type %s %s \n", pokemon_first_type(pokedex->selectedPokenode->pokemon), pokemon_second_type(pokedex->selectedPokenode->pokemon));

}

Pokemon get_current_pokemon(Pokedex pokedex) {

    Pokemon pokemon = NULL;
    if (pokedex->selectedPokenode != NULL) {
        pokemon = pokedex->selectedPokenode->pokemon;
    }
    // printf("%d\n", pokemon_id(pokedex->selectedPokenode->pokemon));
    return pokemon;
}

void find_current_pokemon(Pokedex pokedex) {
    if (pokedex->selectedPokenode != NULL) {
        pokedex->selectedPokenode->found = 1;
    }
}

void print_pokemon(Pokedex pokedex) {
    struct pokenode *currentNode = pokedex->head;
    while (currentNode != NULL) {
        if (currentNode->pokemon == pokedex->selectedPokenode->pokemon) {
            printf("--> ");
        } else {
            printf("    ");
        }
        printf("#%03d: ", pokemon_id(currentNode->pokemon));
        print_name_if_found(currentNode);
        printf("\n");
        currentNode = currentNode->next;
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

void next_pokemon(Pokedex pokedex) {
    struct pokenode *selected = pokedex->selectedPokenode;
    if (selected != NULL) {
        if (selected->next != NULL && selected != NULL) {
            pokedex->selectedPokenode = pokedex->selectedPokenode->next;
        }
    }
}
void prev_pokemon(Pokedex pokedex) {
    struct pokenode *selected = pokedex->selectedPokenode;
    if (selected != NULL) {
        if (selected->prev != NULL) {
            pokedex->selectedPokenode = pokedex->selectedPokenode->prev;
        }
    }
    // print_pokemon(pokedex);
}

void change_current_pokemon(Pokedex pokedex, int id) {
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == id) {
            pokedex->selectedPokenode = leadingNode;
        }
        leadingNode = leadingNode->next;
    }
}

void remove_pokemon(Pokedex pokedex) {
    struct pokenode *node = pokedex->selectedPokenode;
    struct pokenode *next = node->next;
    struct pokenode *prev = node->prev;

    if (node != NULL && node != pokedex->head) {
        destroy_pokemon(node->pokemon);
        if (next == NULL) {
            pokedex->selectedPokenode = prev;
        } else {
            pokedex->selectedPokenode = next;
            node->next->prev = prev;
        }
        node->prev->next = next;
    }

    if (node == pokedex->head) {
        pokedex->selectedPokenode = next;
        destroy_pokemon(node->pokemon);

        if (pokedex->head->next != NULL) {
            pokedex->head = pokedex->head->next;
        } else {
            pokedex->head = NULL;
        }
    }

    free(node);
}

void destroy_pokedex(Pokedex pokedex) {
    struct pokenode *node = pokedex->head;
    struct pokenode *nextNode;
    while (node != NULL) {
        destroy_pokemon(node->pokemon);
        nextNode =  node->next;
        free(node);
        node = nextNode;
    }
    free(pokedex);
    pokedex = NULL;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

void go_exploring(Pokedex pokedex, int seed, int factor, int how_many) {
    // Pokemon currentlySelected = get_current_pokemon(pokedex);
    struct pokenode *randomNode = NULL;
    struct pokenode *leadingNode = pokedex->head;
    srand(seed);
    int num = 0;
    int encountered = 0;

    while (encountered < how_many) {
        num = rand() % (factor - 1);
        leadingNode = pokedex->head;
        while (leadingNode != NULL) {
            if (pokemon_id(leadingNode->pokemon) == num) {
                randomNode = leadingNode;
            }
            leadingNode = leadingNode->next;
        }

        if (randomNode != NULL) {
            randomNode->found = 1;
        }
        encountered++;
    }
}

int count_found_pokemon(Pokedex pokedex) {
    int count = 0;
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (leadingNode->found == 1) {
            count++;
        }
        leadingNode = leadingNode->next;
    }
    return count;
}

int count_total_pokemon(Pokedex pokedex) {
    int count = 0;
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        count++;
        leadingNode = leadingNode->next;
    }

    return count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {
    //printf("Setting the evolve ID of %d to %d", from_id, to_id);
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == from_id) {
            leadingNode->evolveInto = to_id;
            //printf("Changing ID\n");
        }
        leadingNode = leadingNode->next;
    }

}

void show_evolutions(Pokedex pokedex) {
    struct pokenode *leadingNode = pokedex->selectedPokenode;
    // Pokemon pokemon = leadingNode->pokemon;
    int evolves = leadingNode->evolveInto;

    printf("#%003d ", pokemon_id(leadingNode->pokemon));
    if (leadingNode->found == 1) {
        print_name_if_found(leadingNode);
    } else {
        printf("????");
    }
    print_type(leadingNode, 2);


    // printf("\n->> ID: %d\n", evolves);
    int exit = 0;
    int evolveID = leadingNode->evolveInto;
    int selected = -1;

    if (evolves == -1) {
        exit = 1;
    }

    struct pokenode *node = pokedex->head;
    evolveID = leadingNode->evolveInto;
    while (exit == 0) {
        while (selected != evolveID) {
            selected = pokemon_id(node->pokemon);

            if (selected != evolveID) {
                node = node->next;
            }
        }

        printf(" --> #%003d ", selected);

        if (node->found == 1) {
            print_name_if_found(node);
        } else {
            printf("%s\n", pokemon_name(leadingNode->pokemon) );
            printf("????");
        }
        print_type(node, 2);

        if (node->evolveInto != -1) {
            evolveID = node->evolveInto;
            node = pokedex->head;
            selected = -1;
        } else {
            exit = 1;
        }
    }
    printf("\n");
}

int get_next_evolution(Pokedex pokedex) {
    if (pokedex->selectedPokenode != NULL) {
        int evolveInto = pokedex->selectedPokenode->evolveInto;
        if (evolveInto == -1) {
            return DOES_NOT_EVOLVE;
        }
        return evolveInto;
    } else {
        fprintf(stderr, "Exiting because the current pokedex is empty\n");
        exit(1);
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    const char *matchType = pokemon_type_to_string(type);
    Pokedex newPokedex = new_pokedex();
    Pokemon pokemon;
    const char *firstType;
    const char *secondType;


    if (type == INVALID_TYPE || type == NONE_TYPE || type == MAX_TYPE) {
        fprintf(stderr, "Exiting because you entered an invalid search type.\n");
        exit(1);
    }


    struct pokenode *node = pokedex->head;
    while (node != NULL) {
        pokemon = node->pokemon;
        firstType = pokemon_type_to_string(pokemon_first_type(pokemon));
        secondType = pokemon_type_to_string(pokemon_second_type(pokemon));

        if (compare_strings(matchType, firstType) == 1 ||
            compare_strings(matchType, secondType) == 1) {
            if (node->found == 1) {
                add_pokemon(newPokedex, clone_pokemon(pokemon));
            }
        }

        node = node->next;
    }

    struct pokenode *newPokenode = newPokedex->head;
    while (newPokenode != NULL) {
        newPokenode->found = 1;
        newPokenode = newPokenode->next;
    }

    return newPokedex;
}

Pokedex get_found_pokemon(Pokedex pokedex) {
    //struct pokenode *node1 = pokedex->head->next;
    //struct pokenode *node2 = pokedex->head->next->next;

    Pokedex newPokedex = new_pokedex();
    Pokemon pokemon;
    struct pokenode *node = pokedex->head;
    while (node != NULL) {
        pokemon = node->pokemon;
        if (node->found == 1) {
            add_pokemon(newPokedex, clone_pokemon(pokemon));
        }
        node = node->next;
    }


    struct pokenode *newPokenode = newPokedex->head;
    while (newPokenode != NULL) {
        newPokenode->found = 1;
        newPokenode = newPokenode->next;
    }
    // return newPokedex;
    Pokedex returnPoke = sort_pokedex_by_id(newPokedex);
    return returnPoke;
}

Pokedex search_pokemon(Pokedex pokedex, char *text) {
    if (text == NULL) {
        return pokedex;
    }
    Pokedex newPokedex = new_pokedex();
    Pokemon pokemon;
    char *name;

    struct pokenode *node = pokedex->head;
    while (node != NULL) {
        pokemon = node->pokemon;
        name = pokemon_name(pokemon);
        int inString = find_in_string(name, text);
        // printf("In String = %d\n", inString);
        if (node->found == 1) {
            if (inString != 0) {
                add_pokemon(newPokedex, clone_pokemon(pokemon));
            }
        }
        node = node->next;
    }

    struct pokenode *newPokenode = newPokedex->head;
    while (newPokenode != NULL) {
        newPokenode->found = 1;
        newPokenode = newPokenode->next;
    }

    return newPokedex;
}

// Add definitions for your own functions below.
// Make them static to limit their scope to this file.

static void print_type(struct pokenode *node, int failType) {
    Pokemon pokemon = node->pokemon;
    pokemon_type firstType = pokemon_first_type(pokemon);
    pokemon_type secondType = pokemon_second_type(pokemon);

    const char *firstTypeString = pokemon_type_to_string(firstType);
    const char *secondTypeString = pokemon_type_to_string(secondType);
    const char *none = "None";

    if (failType == 1) {
        if (compare_strings(secondTypeString, none) == 1) {
            printf("Type: %s\n", firstTypeString);
        } else {
            printf("Type: %s %s\n", firstTypeString, secondTypeString);
        }
    } else if (failType == 2) {
        if (node->found == 1) {
            if (compare_strings(secondTypeString, none) == 1) {
                printf(" [%s]", firstTypeString);
            } else {
                printf(" [%s, %s]", firstTypeString, secondTypeString);
            }
        } else {
            printf(" [????]");

        }
    }
}

static void print_name_if_found(struct pokenode *node) {
    Pokemon pokemon = node->pokemon;
    char *name = pokemon_name(pokemon);
    char hidden[100] = {0};
    int i = 0;

    if (node->found == 0) {
        while (name[i] != '\0') {
            hidden[i] = 42;
            i++;
        }
    }

    if (node->found == 0) {
        printf("%s",hidden);
    } else {
        printf("%s",name);
    }
}

static struct pokenode *new_pokenode(Pokemon pokemon, struct pokenode *node) {
    struct pokenode *n;
    n = malloc(sizeof(struct pokenode));
    assert(n != NULL);
    n->next = NULL;
    n->pokemon = pokemon;
    n->found = 0;
    n->prev = NULL;
    n->evolveInto = -1;
    if (node != NULL) {
        node->next = n;
        n->prev = node;
    }
    return n;
}

static struct pokenode *get_pokenode(Pokedex pokedex, int id) {
    struct pokenode *leadingNode = pokedex->head;
    struct pokenode *found = NULL;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == id) {
            found = leadingNode;
        }
        leadingNode = leadingNode->next;
    }
    return found;
}

static int compare_strings(const char *one, const char *two) {
    int i;
    i = 0;
    int twoChar = 0;
    int oneChar = 0;
    int matchingChar = 0;

    while (one[i] == two[i] && one[i] != '\0') {
        i++;
    }
    matchingChar = i;

    i = 0;
    while (one[i] != '\0') {
        i++;
    }
    oneChar = i;

    i = 0;
    while (two[i] != '\0') {
        i++;
    }
    twoChar = i;

    if (oneChar == 0) {
        return 0;
    }
    if (oneChar == twoChar && twoChar == matchingChar) {
        return 1;
    } else {
        return 0;
    }
    return 0;
}

static int find_in_string(char *str, char *substr) {
    char *fullName = str;
    char fullName2[30] = {0};
    char searchTerm2[30] = {0};
    char *searchTerm = substr;
    int i = 0;
    int x = 0;
    int y = 0;
    int loop = 0;

    // printf("-------------------\n");
    while (*(searchTerm + y) != '\0') {
        searchTerm2[y] = upper_case(*(searchTerm + y));
        y++;
    }

    int searchCount = y;
    y = 0;

    while (*(fullName + y) != '\0') {
        fullName2[y] = upper_case(*(fullName + y));
        y++;
    }
    y = 0;

    while (fullName2[i] != '\0') {
        y = 0;
        x = 0;
        loop = 0;
        if (fullName2[i] == searchTerm2[x]) {
            y = i;
            while (loop <= searchCount) {
                if (fullName2[y] == searchTerm2[x]) {
                    // printf("%d and %d\n", y, x);
                    // printf("%c and %c\n", fullName2[y], searchTerm2[x]);
                    y++;
                    x++;
                    if (searchTerm2[x] == '\0') {
                        // printf("For narnia\n\n");
                        return 1;
                    }
                } else {
                    loop = searchCount + 1;
                }
                loop++;
            }
        }
        i++;
    }
    return 0;
}


static Pokedex sort_pokedex_by_id(struct pokedex *pokedex) {
    struct pokenode *leadingNode = NULL;
    int swap = 1;
    int sorted = 0;
    int exit = 0;
    leadingNode = pokedex->head;
    Pokemon currentPokemon;
    Pokemon nextPokemon;
    if (leadingNode != NULL) {
        while (exit < 30) {
            swap = 0;
            sorted = 0;
            leadingNode = pokedex->head;
            if (leadingNode == NULL) {
                sorted = 1;
            }
            while (sorted == 0) {
                if (leadingNode != NULL && leadingNode->next != NULL) {
                    currentPokemon = leadingNode->pokemon;
                    nextPokemon = leadingNode->next->pokemon;
                    if (pokemon_id(currentPokemon) > pokemon_id(nextPokemon)) {
                        swap_pokenodes(leadingNode, leadingNode->next, pokedex);
                        swap++;
                    }
                    leadingNode = leadingNode->next;
                } else {
                    sorted = 1;
                }
            }
            exit++;
        }
    }
    pokedex->selectedPokenode = pokedex->head;
    return pokedex;
}

static void swap_pokenodes(struct pokenode *node1, struct pokenode *node2,
    Pokedex pokedex) {
    struct pokenode *prev1 = NULL;
    struct pokenode *next2 = NULL;

    if (node1->prev != NULL && node2->next != NULL) {
        node1->prev->next = node2;
        node2->prev = prev1;
        node1->prev = node2;
        node2->next = node1;
        node1->next = next2;
    } else if (node1->prev == NULL && node2->next != NULL) {
        pokedex->head = node2;
        next2 = node2->next;
        node2->prev = prev1;
        node1->prev = node2;
        node2->next = node1;
        node1->next = next2;
    } else if (node1->prev == NULL && node2->next == NULL) {
        pokedex->head = node2;
        next2 = node2->next;
        node2->prev = prev1;
        node1->prev = node2;
        node2->next = node1;
        node1->next = next2;
    } else if (node1->prev != NULL && node2->next == NULL) {
        node1->prev->next = node2;
        next2 = node2->next;
        node2->prev = prev1;
        node1->prev = node2;
        node2->next = node1;
        node1->next = next2;
    }
}

static void poketails(struct pokenode *node1, struct pokenode *node2) {
    printf("%d\n", pokemon_id(node2->pokemon));
    printf("%d\n", pokemon_id(node1->next->next->next->pokemon));
    node1->next->next->next = node2;
    printf("%d\n", pokemon_id(node1->next->next->next->pokemon));
}


static int upper_case(int character) {
    if (character >= 'A' && character <= 'Z') {
        int alphabetPosition = character - 'A';
        return 'a' + alphabetPosition;
    } else {
        return character;
    }
}

static int does_pokemon_exist(Pokedex pokedex, Pokemon pokemon) {
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == pokemon_id(pokemon)) {
            return 1;
        }
        leadingNode = leadingNode->next;
    }
    return 0;
}
