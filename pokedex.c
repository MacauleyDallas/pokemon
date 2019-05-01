// Assignment 2 19T1 COMP1511: Pokedex
// pokedex.c
//
// This program was written by Macauley Dallas (z5232924)
// on 27th April 2019
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

// Add prototypes for any extra functions you create here.
static struct pokenode *new_pokenode(Pokemon pokemon, struct pokenode *node);
static void print_name_if_found(struct pokenode *node);
static struct pokenode *get_pokenode(Pokedex pokedex, int id);
static void print_id_if_found(struct pokenode *node);
static void print_type(struct pokenode *node, int failType);
static int compare_strings(const char *one, const char *two);
static int find_in_string(char *str, char *substr);
static void swap_pokenodes(struct pokenode *node1, struct pokenode *node2,
    Pokedex pokedex);
static Pokedex sort_pokedex_by_id(struct pokedex *pokedex);
static int lower_case(int character);
static int does_pokemon_exist(Pokedex pokedex, int pokemonID);
static int pokemon_in_range(Pokedex pokedex, int maxid);

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

// This function adds a new pokemon to the pokedex
void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    if (does_pokemon_exist(pokedex, pokemon_id(pokemon)) == 1) {
        fprintf(stderr, "Exiting because the pokemon already exists\n");
        exit(1);
    }
    struct pokenode *node;
    if (pokedex->head == NULL) {
        node = NULL;
    } else {
        node = pokedex->leadingNode;
    }
    struct pokenode *newPokenode = new_pokenode(pokemon, node);
    if (pokedex->numPokemon == 0) {
        pokedex->head = newPokenode;
        pokedex->selectedPokenode = newPokenode;
    }
    pokedex->numPokemon++;
    pokedex->leadingNode = newPokenode;
}

// This function prints the detailes of the selected pokenode
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
}

// This function returns the currently selected pokenode
Pokemon get_current_pokemon(Pokedex pokedex) {
    Pokemon pokemon = NULL;
    if (pokedex->selectedPokenode != NULL) {
        pokemon = pokedex->selectedPokenode->pokemon;
    } else {
        fprintf(stderr, "Exiting because the current pokedex is empty\n");
        exit(1);
    }
    return pokemon;
}

// This function marks the currently selected pokemon as found
void find_current_pokemon(Pokedex pokedex) {
    if (pokedex->selectedPokenode != NULL) {
        pokedex->selectedPokenode->found = 1;
    }
}

// This function prints out a list of all pokemon in the pokedex
// It also prints an indicator of which pokemon is currently selected
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

// This function changes the currently selected pokemon
// to the next pokemon in the linked list
void next_pokemon(Pokedex pokedex) {
    struct pokenode *selected = pokedex->selectedPokenode;
    if (selected != NULL) {
        if (selected->next != NULL && selected != NULL) {
            pokedex->selectedPokenode = pokedex->selectedPokenode->next;
        }
    }
}

// This function changes the currently selected pokemon
// to the previous pokemon in the linked list
void prev_pokemon(Pokedex pokedex) {
    struct pokenode *selected = pokedex->selectedPokenode;
    if (selected != NULL) {
        if (selected->prev != NULL) {
            pokedex->selectedPokenode = pokedex->selectedPokenode->prev;
        }
    }
}

// This function changes the currently selected pokemon to
// a pokemon with the given pokemon ID
void change_current_pokemon(Pokedex pokedex, int id) {
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == id) {
            pokedex->selectedPokenode = leadingNode;
        }
        leadingNode = leadingNode->next;
    }
}

// This function removes a pokemon from the pokedex
// this includes freeing the memory allocated to the pokemon
// and the pokenode
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

// This function destroys the entire pokedex, along will
// freeing all the memory allocated to pokemon, pokenodes
// and the pokedex itself
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

// This function marks a random pokemon as found within a specified range
// a given amount of times. It can mark the same pokemon multiple times
// and it can mark pokemon that are already found
void go_exploring(Pokedex pokedex, int seed, int factor, int how_many) {
    struct pokenode *randomNode = NULL;
    struct pokenode *leadingNode = pokedex->head;
    srand(seed);
    int num = 0;
    int encountered = 0;
    int maxid = (factor);

    if (pokemon_in_range(pokedex, maxid) == 0) {
        fprintf(stderr,
            "Exiting because there are no pokemon in the specified range\n");
        exit(1);
    }
    while (encountered < how_many) {
        num = rand() % maxid;
        leadingNode = pokedex->head;
        while (leadingNode != NULL) {
            if (pokemon_id(leadingNode->pokemon) == num) {
                randomNode = leadingNode;
            }
            leadingNode = leadingNode->next;
        }
        if (randomNode != NULL) {
            randomNode->found = 1;
            encountered++;
        }

    }
}

// This function counts the amout of pokemon in the pokedex
// that have been found and returns that count as an integer
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

// This function counts the amout of pokemon in the pokedex
// and returns that count as an integer
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

// This function will set the evolveID of a given pokemon
// to another given pokemon as long as the given ID's
// exist and are not identical
void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {

    if (does_pokemon_exist(pokedex, to_id) == 0
        || does_pokemon_exist(pokedex, to_id) == 0
        || from_id == to_id) {
        fprintf(stderr, "Exiting due to invalid input\n");
        exit(1);

    }
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == from_id) {
            leadingNode->evolveInto = to_id;
        }
        leadingNode = leadingNode->next;
    }

}

// This function prints out the evolution process of
// the currently selected pokemon
void show_evolutions(Pokedex pokedex) {
    struct pokenode *leadingNode = pokedex->selectedPokenode;
    int evolves = leadingNode->evolveInto;

    printf("#%003d ", pokemon_id(leadingNode->pokemon));
    if (leadingNode->found == 1) {
        print_name_if_found(leadingNode);
    } else {
        printf("????");
    }
    print_type(leadingNode, 2);

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

// This function returns the evolveID of a the currently
// selected pokemon
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

// This function gets all pokemon of specified type
// and places them in a seperate pokedex
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

// This function gets all found pokemon and places them
// in a seperate pokedex
Pokedex get_found_pokemon(Pokedex pokedex) {
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
    Pokedex returnPoke = sort_pokedex_by_id(newPokedex);
    return returnPoke;
}

// This function gets all pokemon who's name contains
// the spesific given characters in the given order
// regardless of capitilization. It then returns a new
// pokedex filled with these pokemon.
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

// This function will print out the type of a pokeon inside
// a given pokenode. If the pokemon is not found it will
// instead print 'hidden' characters, which depend on what failType
// given to the function when it was called.
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

// This function prints out the name of a pokemon if it has been found
// The function will instead pring a
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

// This allocates allocates memory to and creates a new pokenode
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

// This function gets a pokenode containing a pokemon with a given id
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

// This function compares two strings and returns an integer 1 if they match
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

// This function searches a given string for a given
static int find_in_string(char *str, char *substr) {
    char *fullName = str;
    char fullName2[30] = {0};
    char searchTerm2[30] = {0};
    char *searchTerm = substr;
    int i = 0;
    int x = 0;
    int y = 0;
    int loop = 0;

    while (*(searchTerm + y) != '\0') {
        searchTerm2[y] = lower_case(*(searchTerm + y));
        y++;
    }

    int searchCount = y;
    y = 0;

    while (*(fullName + y) != '\0') {
        fullName2[y] = lower_case(*(fullName + y));
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

// This function sorts a given pokedex by pokemon ID
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

// This function swaps two given pokenodes
static void swap_pokenodes(struct pokenode *node1, struct pokenode *node2,
    Pokedex pokedex) {
    struct pokenode *prev1 = NULL;
    struct pokenode *next2 = NULL;
    if (node1->prev != NULL && node2->next != NULL) {
        node1->prev->next = node2;
    } else if (node1->prev == NULL && node2->next != NULL) {
        pokedex->head = node2;
        next2 = node2->next;
    } else if (node1->prev == NULL && node2->next == NULL) {
        pokedex->head = node2;
        next2 = node2->next;
    } else if (node1->prev != NULL && node2->next == NULL) {
        node1->prev->next = node2;
        next2 = node2->next;
    }
    node2->prev = prev1;
    node2->next = node1;
    node1->next = next2;
    node1->prev = node2;
}

// This functions changes the case of a letter from upper to lower.
static int lower_case(int character) {
    if (character >= 'A' && character <= 'Z') {
        int alphabetPosition = character - 'A';
        return 'a' + alphabetPosition;
    } else {
        return character;
    }
}

// This functions checks if the given pokemonID
// already exists in a given pokedex
static int does_pokemon_exist(Pokedex pokedex, int pokemonID) {
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == pokemonID) {
            return 1;
        }
        leadingNode = leadingNode->next;
    }
    return 0;
}

// This function checks that there are pokemon that exist in a given
// pokedext that have id's above zero and below a given maximum
static int pokemon_in_range(Pokedex pokedex, int maxid) {
    int currentId = 0;struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        currentId = pokemon_id(leadingNode->pokemon);
        if (currentId > 0 && currentId < maxid) {
            return 1;
        }
        leadingNode = leadingNode->next;
    }
    return 1;
}
