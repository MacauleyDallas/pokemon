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
//
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
    int numPokemon;
    struct pokenode *selectedPokemon;
};


// You don't have to use the provided struct pokenode, you are free to
// make your own struct instead.
// If you use the provided struct pokenode, you will need to add fields
// to it to store other information.

struct pokenode {
    struct pokenode *next;
    int found;
    Pokemon         pokemon;
};

// Add any other structs you define here.


// Add prototypes for any extra functions you create here.
struct pokenode *new_pokenode(Pokemon pokemon);


// You need to implement the following 20 functions.
// In other words, replace the lines calling fprintf & exit with your code.
// You can find descriptions of what each function should do in pokedex.h


Pokedex new_pokedex(void) {
    Pokedex new_pokedex = malloc(sizeof (struct pokedex));
    assert(new_pokedex != NULL);
    // add your own code here

    // k
    new_pokedex->head = NULL;
    return new_pokedex;
}


////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    //Make new pokenode
    int i = 0;
    int exit = 0;
    int firstPoke = 0;
    struct pokenode *newPokenode = new_pokenode(pokemon);
    struct pokenode *end = pokedex->head;

    printf("%s\n", end);
    if (end == NULL) {
        pokedex->head = newPokenode;
        pokedex->selectedPokemon = newPokenode;
    } else {
        while (exit != 1) {
            if (end->next != NULL){
                end = end->next;
            } else {
                exit = 1;
            }
            exit = 1;
            printf("%d\n",i++);
        }
    }

    end->next = newPokenode;
}

void detail_pokemon(Pokedex pokedex) {

    struct pokenode *selectedPokemon = pokedex->selectedPokemon;

    printf("ID: %003d\n", pokemon_id(selectedPokemon->pokemon));
    if (selectedPokemon->found == 0) {
        printf("Name: ********\n");
    } else if (selectedPokemon->found == 1) {
        printf("Name: %s\n", pokemon_name(selectedPokemon->pokemon));
    }
    printf("Height: %.2lfm\n", pokemon_height(selectedPokemon->pokemon));
    printf("Weight: %.2lfkg\n", pokemon_weight(selectedPokemon->pokemon));

    // printf("Type %s \n", pokemon_first_type(pokedex->selectedPokemon->pokemon));

    // printf("Type %s %s \n", pokemon_first_type(pokedex->selectedPokemon->pokemon), pokemon_second_type(pokedex->selectedPokemon->pokemon));

}

Pokemon get_current_pokemon(Pokedex pokedex) {
    Pokemon pokemon = pokedex->selectedPokemon->pokemon;
    return pokemon;
}

void find_current_pokemon(Pokedex pokedex) {
    pokedex->selectedPokemon->found = 1;
}

void print_pokemon(Pokedex pokedex) {
    struct pokenode *currentHead = pokedex->head;
    struct pokenode *currentlySelected = pokedex->selectedPokemon;
    while (currentHead != NULL) {
        printf("%s\n",pokemon_name(currentHead->pokemon));
        currentHead = currentHead->next;
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

void next_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the next_pokemon function in pokedex.c\n");
    exit(1);
}

void prev_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the prev_pokemon function in pokedex.c\n");
    exit(1);
}

void change_current_pokemon(Pokedex pokedex, int id) {
    fprintf(stderr, "exiting because you have not implemented the change_current_pokemon function in pokedex.c\n");
    exit(1);
}

void remove_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the remove_pokemon function in pokedex.c\n");
    exit(1);
}

void destroy_pokedex(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the destroy_pokedex function in pokedex.c\n");
    exit(1);
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

void go_exploring(Pokedex pokedex, int seed, int factor, int how_many) {
    fprintf(stderr, "exiting because you have not implemented the go_exploring function in pokedex.c\n");
    exit(1);
}

int count_found_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the count_found_pokemon function in pokedex.c\n");
    exit(1);
}

int count_total_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the count_total_pokemon function in pokedex.c\n");
    exit(1);
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {
    fprintf(stderr, "exiting because you have not implemented the add_pokemon_evolution function in pokedex.c\n");
    exit(1);
}

void show_evolutions(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the show_evolutions function in pokedex.c\n");
    exit(1);
}

int get_next_evolution(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the get_next_evolution function in pokedex.c\n");
    exit(1);
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    fprintf(stderr, "exiting because you have not implemented the get_pokemon_of_type function in pokedex.c\n");
    exit(1);
}

Pokedex get_found_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the get_found_pokemon function in pokedex.c\n");
    exit(1);
}

Pokedex search_pokemon(Pokedex pokedex, char *text) {
    fprintf(stderr, "exiting because you have not implemented the search_pokemon function in pokedex.c\n");
    exit(1);
}

// Add definitions for your own functions below.
// Make them static to limit their scope to this file.

struct pokenode *new_pokenode(Pokemon pokemon) {
    struct pokenode *n;
    n = malloc(sizeof(struct pokenode));
    assert(n != NULL);
    n->pokemon = pokemon;
    n->next = NULL;
    n->found = 0;
    return n;
}
