// Assignment 2 19T1 COMP1511: Pokedex
// pokedex.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Minor clarifications about `struct pokenode`.
// Version 1.1.0: Moved destroy_pokedex function to correct location.
// Version 1.1.1: Renamed "pokemon_id" to "id" in change_current_pokemon.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.
#include <string.h>
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


// You need to implement the following 20 functions.
// In other words, replace the lines calling fprintf & exit with your code.
// You can find descriptions of what each function should do in pokedex.h


Pokedex new_pokedex(void) {
    Pokedex new_pokedex = malloc(sizeof (struct pokedex));
    assert(new_pokedex != NULL);
    // add your own code here
    new_pokedex->numPokemon = 0;
    // k
    new_pokedex->head = NULL;
    return new_pokedex;
}


////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    //Make new pokenode
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

void detail_pokemon(Pokedex pokedex) {
    struct pokenode *selectedPokenode = pokedex->selectedPokenode;

    if (selectedPokenode->found == 0) {
        printf("ID: %003d\n", pokemon_id(selectedPokenode->pokemon));
        printf("Name: *****\n");
        printf("Height: ---\n");
        printf("Weight: ---\n");
        printf("Type: ---\n");
    } else {
        printf("ID: %003d\n", pokemon_id(selectedPokenode->pokemon));
        printf("Name: %s\n", pokemon_name(selectedPokenode->pokemon));
        printf("Height: %.2lfm\n", pokemon_height(selectedPokenode->pokemon));
        printf("Weight: %.2lfkg\n", pokemon_weight(selectedPokenode->pokemon));
        print_type(selectedPokenode, 1);
    }

    // printf("Type %s \n", pokemon_first_type(pokedex->selectedPokenode->pokemon));

    // printf("Type %s %s \n", pokemon_first_type(pokedex->selectedPokenode->pokemon), pokemon_second_type(pokedex->selectedPokenode->pokemon));

}

Pokemon get_current_pokemon(Pokedex pokedex) {
    Pokemon pokemon = pokedex->selectedPokenode->pokemon;
    // printf("%d\n", pokemon_id(pokedex->selectedPokenode->pokemon));
    return pokemon;
}

void find_current_pokemon(Pokedex pokedex) {
    pokedex->selectedPokenode->found = 1;
}

void print_pokemon(Pokedex pokedex) {
    struct pokenode *currentNode = pokedex->head;
    while (currentNode != NULL) {
        if (currentNode->pokemon == pokedex->selectedPokenode->pokemon) {
            printf("--> ");
        } else {
            printf("    ");
        }
        printf("#%03d ", pokemon_id(currentNode->pokemon));
        print_name_if_found(currentNode);
        printf("\n");
            currentNode = currentNode->next;
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

void next_pokemon(Pokedex pokedex) {
    if (pokedex->selectedPokenode->next != NULL) {
        pokedex->selectedPokenode = pokedex->selectedPokenode->next;
    }
    // print_pokemon(pokedex);
}

void prev_pokemon(Pokedex pokedex) {
    if (pokedex->selectedPokenode->prev != NULL) {
        pokedex->selectedPokenode = pokedex->selectedPokenode->prev;
    }
    // print_pokemon(pokedex);
}

void change_current_pokemon(Pokedex pokedex, int id) {
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == id ) {
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
        if (next == NULL) {
            pokedex->selectedPokenode = prev;
        } else {
            pokedex->selectedPokenode = next;
            node->next->prev = prev;
        }
        printf("Deleting\n");
        node->prev->next = next;
    }
    if (node == pokedex->head) {
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
    Pokemon currentlySelected = get_current_pokemon(pokedex);
    struct pokenode *randomNode = NULL;
    srand(seed);
    if (how_many-1 != 0) {

    } {
        printf("Error: Division by 0\n");
    }
    int num = rand()%(how_many-1);
    printf("Randomly chosen ID number %d\n", num);
    struct pokenode *leadingNode = pokedex->head;
    num = 20;

    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == num ) {
            randomNode = leadingNode;
        }

        leadingNode = leadingNode->next;
    }

    if (randomNode != NULL) {
        randomNode->found = 1;
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
    struct pokenode *leadingNode = pokedex->head;
    while (leadingNode != NULL) {
        if (pokemon_id(leadingNode->pokemon) == from_id) {
            leadingNode->evolveInto = to_id;
        }
        leadingNode = leadingNode->next;
    }

}

void show_evolutions(Pokedex pokedex) {
    char* yep = "????";
    int evolves = 0;
    struct pokenode *leadingNode = pokedex->selectedPokenode;
    Pokemon pokemon = leadingNode->pokemon;
    printf("#%02d", pokemon_id(leadingNode->pokemon));
    printf("%s\n", yep);
    if (leadingNode->found == 1) {
        print_name_if_found(leadingNode);
    } else {
        // printf("%s\n", pokemon_name(leadingNode->pokemon) );
        printf("?????");
    }
    print_type(leadingNode, 2);

    while (leadingNode->evolveInto != -1) {
        evolves = leadingNode->evolveInto;
        leadingNode = leadingNode->next;
        struct pokenode *node = get_pokenode(pokedex, evolves);
        pokemon = node->pokemon;
        printf(" --> #%02d ", pokemon_id(leadingNode->pokemon));
        if (leadingNode->found == 1) {
            print_name_if_found(leadingNode);
        } else {
            // printf("%s\n", pokemon_name(leadingNode->pokemon) );
            printf("?????");
        }
        print_type(leadingNode, 2);
    }
    printf("\n");
}

int get_next_evolution(Pokedex pokedex) {
    int evolveInto = pokedex->selectedPokenode->evolveInto;

    if (evolveInto == -1) {
        return DOES_NOT_EVOLVE;
    }
    return evolveInto;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    struct pokenode *node = pokedex->head;
    const char *matchType = pokemon_type_to_string(type);
    printf("%s\n", matchType);



    // Pokedex newPokedex = new_pokedex();
    // add_pokemon()
    // Pokemon pokemon;
    // new_pokenode(newPokedex, pokemon);
    // reurn newPokedex;
}

Pokedex get_found_pokemon(Pokedex pokedex) {
    struct pokenode *leadingNode = pokedex->head;
    int found;
    Pokedex newPokedex = new_pokedex();
    struct pokenode *leadingNewNode = newPokedex->head;
    Pokemon pokemon;

    while (leadingNode != NULL) {
        found = leadingNode->found;
        if (found == 1) {
            pokemon = leadingNode->pokemon;
            leadingNewNode = new_pokenode(pokemon, leadingNewNode);
        }

        leadingNode = leadingNode->next;
    }

    // const char *matchType = pokemon_type_to_string(type);
    // printf("%s\n", matchType);
}

Pokedex search_pokemon(Pokedex pokedex, char *text) {
    fprintf(stderr, "exiting because you have not implemented the search_pokemon function in pokedex.c\n");
    exit(1);
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
        if (strcmp(secondTypeString, none) == 0) {
            printf("Type: %s\n", firstTypeString);
        } else {
            printf("Type: %s %s\n", firstTypeString, secondTypeString);
        }
    } else if (failType == 2) {
        if (node->found == 1) {
            if (strcmp(secondTypeString, none) == 0) {
                printf(" [%s]", firstTypeString);
            } else {
                printf(" [%s, %s]", firstTypeString, secondTypeString);
            }
        } else {
            printf(" [???]");

        }
    }
}

static void print_name_if_found(struct pokenode *node) {
    Pokemon pokemon = node->pokemon;
    char *name = pokemon_name(pokemon);
    char hidden[100] = {0};
    int i = 0;

    if (node->found == 0) {
        while(name[i] != '\0') {
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
