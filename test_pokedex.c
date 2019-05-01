// Assignment 2 19T1 COMP1511: Pokedex
// test_pokedex.c
//
// This file allows you to automatically test the functions you
// implement in pokedex.c.
//
// This program was written by Macauley Dallas (z5232924)
// on 27/04/19
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Added pointer check for the provided tests.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pokedex.h"

// Add your own #defines here.


// Sample data on Bulbasaur, the Pokemon with pokemon_id 1.
#define BULBASAUR_ID 1
#define BULBASAUR_NAME "Bulbasaur"
#define BULBASAUR_HEIGHT 0.7
#define BULBASAUR_WEIGHT 6.9
#define BULBASAUR_FIRST_TYPE GRASS_TYPE
#define BULBASAUR_SECOND_TYPE POISON_TYPE

// Sample data on Ivysaur, the Pokemon with pokemon_id 2.
#define IVYSAUR_ID 2
#define IVYSAUR_NAME "Ivysaur"
#define IVYSAUR_HEIGHT 1.0
#define IVYSAUR_WEIGHT 13.0
#define IVYSAUR_FIRST_TYPE GRASS_TYPE
#define IVYSAUR_SECOND_TYPE POISON_TYPE

// Sample data on Venusaur, the Pokemon with pokemon_id 3.
#define VENUSAUR_ID 3
#define VENUSAUR_NAME "Venusaur"
#define VENUSAUR_HEIGHT 2.0
#define VENUSAUR_WEIGHT 100.0
#define VENUSAUR_FIRST_TYPE GRASS_TYPE
#define VENUSAUR_SECOND_TYPE POISON_TYPE

// Sample data on Charmander, the Pokemon with pokemon_id 4.
#define CHARMANDER_ID 4
#define CHARMANDER_NAME "Charmander"
#define CHARMANDER_HEIGHT 0.6
#define CHARMANDER_WEIGHT 8.5
#define CHARMANDER_FIRST_TYPE FIRE_TYPE
#define CHARMANDER_SECOND_TYPE NONE_TYPE

// Sample data on Charmeleon, the Pokemon with pokemon_id 5.
#define CHARMELEON_ID 5
#define CHARMELEON_NAME "Charmeleon"
#define CHARMELEON_HEIGHT 1.1
#define CHARMELEON_WEIGHT 19.0
#define CHARMELEON_FIRST_TYPE FIRE_TYPE
#define CHARMELEON_SECOND_TYPE NONE_TYPE



// Add your own prototypes here.
static void get_current(void);
static Pokemon create_venusaur(void);
static Pokemon create_charmander(void);
static Pokemon create_charmeleon(void);
static void add_five_pokemon(Pokedex pokedex, Pokemon bulbasaur,
    Pokemon ivysaur, Pokemon venusaur, Pokemon charmeleon,
    Pokemon charmander);
static int compare_strings(const char *one, const char *two);
static void change_pokemon(Pokedex pokedex, int type, int amount);
static void test_go_exploring(void);
static void test_search_pokemon(void);
static void find_pokemon(Pokedex pokedex, int amount);


// Tests for Pokedex functions from pokedex.c.
static void test_new_pokedex(void);
static void test_add_pokemon(void);
static void test_get_found_pokemon(void);
static void test_next_pokemon(void);

// Helper functions for creating/comparing Pokemon.
static Pokemon create_bulbasaur(void);
static Pokemon create_ivysaur(void);
static int is_same_pokemon(Pokemon first, Pokemon second);
static int is_copied_pokemon(Pokemon first, Pokemon second);



int main(int argc, char *argv[]) {
    printf("Welcome to the COMP1511 Pokedex Tests!\n");

    printf("\n==================== Pokedex Tests ====================\n");
    test_new_pokedex();
    test_add_pokemon();
    test_next_pokemon();
    test_get_found_pokemon();
    get_current();
    test_go_exploring();
    test_search_pokemon();
    printf("\nAll Pokedex tests passed, you are Awesome!\n");
}


////////////////////////////////////////////////////////////////////////
//                     Pokedex Test Functions                         //
////////////////////////////////////////////////////////////////////////

// `test_new_pokedex` checks whether the new_pokedex and destroy_pokedex
// functions work correctly, to the extent that it can.
//
// It does this by creating a new Pokedex, checking that it's not NULL,
// then calling destroy_pokedex.
//
// Note that it isn't possible to check whether destroy_pokedex has
// successfully destroyed/freed the Pokedex, so the best we can do is to
// call the function and make sure that it doesn't crash..

static void test_new_pokedex(void) {
    printf("\n>> Testing new_pokedex\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("       --> Checking that the returned Pokedex is not NULL\n");
    assert(pokedex != NULL);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed new_pokedex tests!\n");
}

// `test_add_pokemon` checks whether the add_pokemon function works
// correctly.
//
// It does this by creating the Pokemon Bulbasaur (using the helper
// functions in this file and the provided code in pokemon.c), and
// calling add_pokemon to add it to the Pokedex.
//
// Some of the ways that you could extend these test would include:
//   - adding additional Pokemon other than just Bulbasaur,
//   - checking whether the currently selected Pokemon is correctly set,
//   - checking that functions such as `count_total_pokemon` return the
//     correct result after more Pokemon are added,
//   - ... and more!
static void test_add_pokemon(void) {
    printf("\n>> Testing add_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    // Creating five pokemon
    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();
    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();
    printf("    ... Creating Charmander\n");
    Pokemon charmander = create_charmander();
    printf("    ... Creating Charmeleon\n");
    Pokemon charmeleon = create_charmeleon();

    // Calling function to add the five pokemon to pokedex
    add_five_pokemon(pokedex, bulbasaur, ivysaur, venusaur,
        charmeleon, charmander);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("       --> Checking that the total Pokemon in the Pokedex is 5\n");
    assert(count_total_pokemon(pokedex) == 5);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed add_pokemon tests!\n");
}

// `test_next_pokemon` checks whether the next_pokemon function works
// correctly.
//
// It does this by creating two Pokemon: Bulbasaur and Ivysaur (using
// the helper functions in this file and the provided code in pokemon.c).
//
// It then adds these to the Pokedex, then checks that calling the
// next_pokemon function changes the currently selected Pokemon from
// Bulbasaur to Ivysaur.
//
// Some of the ways that you could extend these tests would include:
//   - adding even more Pokemon to the Pokedex,
//   - calling the next_pokemon function when there is no "next" Pokemon,
//   - calling the next_pokemon function when there are no Pokemon in
//     the Pokedex,
//   - ... and more!
static void test_next_pokemon(void) {
    printf("\n>> Testing next_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    // Creating five pokemon
    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();
    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();
    printf("    ... Creating Charmander\n");
    Pokemon charmander = create_charmander();
    printf("    ... Creating Charmeleon\n");
    Pokemon charmeleon = create_charmeleon();

    // Calling function to add the five pokemon to pokedex
    add_five_pokemon(pokedex, bulbasaur, ivysaur, venusaur,
        charmeleon, charmander);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Moving to the next pokemon\n");
    change_pokemon(pokedex, 1, 1);

    printf("       --> Checking that the current Pokemon is Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));

    printf("    ... Moving two pokemon more down\n");
    change_pokemon(pokedex, 1, 2);

    printf("       --> Checking that the current Pokemon is Charmander\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), charmander));

    printf("    ... Attempting to move beyond the end of the pokedex\n");
    change_pokemon(pokedex, 1, 5);

    printf("       --> Checking that the current Pokemon is Charmeleon\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), charmeleon));

    printf("    ... Moving Back to the First pokemon\n");
    change_pokemon(pokedex, 0, 5);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf("    ... Creating another new Pokedex\n");
    Pokedex newPokedex = new_pokedex();

    printf("    ... Attempting to change pokemon when there are no pokemon\n");
    change_pokemon(newPokedex, 1, 2);
    change_pokemon(newPokedex, 0, 2);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(newPokedex);

    printf(">> Passed next_pokemon tests!\n");
}

// `test_get_found_pokemon` checks whether the get_found_pokemon
// function works correctly.
//
// It does this by creating two Pokemon: Bulbasaur and Ivysaur (using
// the helper functions in this file and the provided code in pokemon.c).
//
// It then adds these to the Pokedex, sets Bulbasaur to be found, and
// then calls the get_found_pokemon function to get all of the Pokemon
// which have been found (which should be just the one, Bulbasaur).
//
// Some of the ways that you could extend these tests would include:
//   - calling the get_found_pokemon function on an empty Pokedex,
//   - calling the get_found_pokemon function on a Pokedex where none of
//     the Pokemon have been found,
//   - checking that the Pokemon in the new Pokedex are in ascending
//     order of pokemon_id (regardless of the order that they appeared
//     in the original Pokedex),
//   - checking that the currently selected Pokemon in the returned
//     Pokedex has been set correctly,
//   - checking that the original Pokedex has not been modified,
//   - ... and more!
static void test_get_found_pokemon(void) {
    printf("\n>> Testing get_found_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Getting all found Pokemon on an empty Pokedex\n");
    Pokedex found_pokedex = get_found_pokemon(pokedex);

    printf("       --> Checking that the pokedex is empty\n");
    assert(count_total_pokemon(found_pokedex) == 0);

    // Creating five pokemon
    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();
    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();
    printf("    ... Creating Charmander\n");
    Pokemon charmander = create_charmander();
    printf("    ... Creating Charmeleon\n");
    Pokemon charmeleon = create_charmeleon();

    // Calling function to add the five pokemon to pokedex
    add_five_pokemon(pokedex, bulbasaur, ivysaur, venusaur,
        charmeleon, charmander);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(get_current_pokemon(pokedex) == bulbasaur);

    printf("    ... Getting all found Pokemon in the Pokedex\n");
    found_pokedex = get_found_pokemon(pokedex);

    printf("       --> Checking that the pokedex is empty\n");
    assert(count_total_pokemon(found_pokedex) == 0);

    printf("    ... Setting Bulbasaur to be found\n");
    find_current_pokemon(pokedex);

    printf("    ... Getting all found Pokemon\n");
    found_pokedex = get_found_pokemon(pokedex);

    printf("       --> Checking the correct Pokemon were copied and returned\n");
    assert(count_total_pokemon(found_pokedex) == 1);
    assert(count_found_pokemon(found_pokedex) == 1);
    assert(is_copied_pokemon(get_current_pokemon(found_pokedex), bulbasaur));

    printf("    ... Destroying both Pokedexes\n");
    destroy_pokedex(pokedex);
    destroy_pokedex(found_pokedex);

    printf(">> Passed get_found_pokemon tests!\n");
}


// Write your own Pokedex tests here!
static void get_current(void) {
    printf("\n>> Testing get current function\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Getting the currently selected when there are none\n");
    Pokemon current = get_current_pokemon(pokedex);

    printf("       --> Checking the the correct pokemon was retreived\n");
    assert(get_current_pokemon(pokedex) == NULL);

    // Creating five pokemon
    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();
    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();
    printf("    ... Creating Charmander\n");
    Pokemon charmander = create_charmander();
    printf("    ... Creating Charmeleon\n");
    Pokemon charmeleon = create_charmeleon();

    // Calling function to add the five pokemon to pokedex
    add_five_pokemon(pokedex, bulbasaur, ivysaur, venusaur,
        charmeleon, charmander);

    printf("    ... Getting the currently selected pokemon\n");
    current = get_current_pokemon(pokedex);

    printf("       --> Checking the the correct pokemon was retreived\n");
    assert(get_current_pokemon(pokedex) == bulbasaur);

    printf("    ... Changing the currently selected pokemon\n");
    change_pokemon(pokedex, 1, 1);

    printf("    ... Getting the currently selected pokemon\n");
    current = get_current_pokemon(pokedex);

    printf("       --> Checking the the correct pokemon was retreived\n");
    assert(get_current_pokemon(pokedex) == ivysaur);

    printf("    ... Destroying the pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed get_current tests!\n");

}

static void test_go_exploring(void) {
    printf("\n>> Testing go exploring function\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Getting the currently selected when there are none\n");
    Pokemon current = get_current_pokemon(pokedex);

    printf("       --> Checking the the correct pokemon was retreived\n");
    assert(get_current_pokemon(pokedex) == NULL);

    // Creating five pokemon
    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();
    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();
    printf("    ... Creating Charmander\n");
    Pokemon charmander = create_charmander();
    printf("    ... Creating Charmeleon\n");
    Pokemon charmeleon = create_charmeleon();

    // Calling function to add the five pokemon to pokedex
    add_five_pokemon(pokedex, bulbasaur, ivysaur, venusaur,
        charmeleon, charmander);

    int seed = 429;
    printf("    ... Going Exploring\n");
    go_exploring(pokedex, seed, 3, 1);

    printf("    ... Getting all found Pokemon\n");
    Pokedex found_pokedex = get_found_pokemon(pokedex);

    printf("       --> Checking that the pokedex only has one pokemon\n");
    assert(count_total_pokemon(found_pokedex) == 1);

    printf("       --> Destroying the found pokemon pokedex\n");
    destroy_pokedex(found_pokedex);

    printf("       --> Destroying original pokedex\n");
    destroy_pokedex(pokedex);
}

static void test_search_pokemon(void) {
    printf("\n>> Testing search pokemon function\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Getting the currently selected when there are none\n");
    Pokemon current = get_current_pokemon(pokedex);

    printf("       --> Checking the the correct pokemon was retreived\n");
    assert(get_current_pokemon(pokedex) == NULL);

    // Creating five pokemon
    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();
    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();
    printf("    ... Creating Charmander\n");
    Pokemon charmander = create_charmander();
    printf("    ... Creating Charmeleon\n");
    Pokemon charmeleon = create_charmeleon();

    // Calling function to add the five pokemon to pokedex
    add_five_pokemon(pokedex, bulbasaur, ivysaur, venusaur,
        charmeleon, charmander);

    printf("    ... Finding all pokemon in the pokedex\n");
    find_pokemon(pokedex, 5);
    Pokedex newPokedex;

    printf("    ... Searching the pokedex for the string 'ur'\n");
    newPokedex = search_pokemon(pokedex, "ur");

    printf("       --> Testing to see if the resulting pokedex has three entries\n");
    assert(count_total_pokemon(newPokedex) == 3);


    printf("    ... Searching the pokedex for pokemon with an empty search string\n");
    newPokedex = search_pokemon(pokedex, "");

    printf("       --> Testing to see if the resulting pokedex has zero entries\n");
    assert(count_total_pokemon(newPokedex) == 0);

}


////////////////////////////////////////////////////////////////////////
//                     Helper Functions                               //
////////////////////////////////////////////////////////////////////////

// Helper function to create Bulbasaur for testing purposes.
static Pokemon create_bulbasaur(void) {
    Pokemon pokemon = new_pokemon(
            BULBASAUR_ID, BULBASAUR_NAME,
            BULBASAUR_HEIGHT, BULBASAUR_WEIGHT,
            BULBASAUR_FIRST_TYPE,
            BULBASAUR_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Ivysaur for testing purposes.
static Pokemon create_ivysaur(void) {
    Pokemon pokemon = new_pokemon(
            IVYSAUR_ID, IVYSAUR_NAME,
            IVYSAUR_HEIGHT, IVYSAUR_WEIGHT,
            IVYSAUR_FIRST_TYPE,
            IVYSAUR_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to compare whether two Pokemon are the same.
// This checks that the two pointers contain the same address, i.e.
// they are both pointing to the same pokemon struct in memory.
//
// Pokemon ivysaur = new_pokemon(0, 'ivysaur', 1.0, 13.0, GRASS_TYPE, POISON_TYPE)
// Pokemon also_ivysaur = ivysaur
// is_same_pokemon(ivysaur, also_ivysaur) == TRUE
static int is_same_pokemon(Pokemon first, Pokemon second) {
    return first == second;
}

// Helper function to compare whether one Pokemon is a *copy* of
// another, based on whether their attributes match (e.g. pokemon_id,
// height, weight, etc).
//
// It also checks that the pointers do *not* match -- i.e. that the
// pointers aren't both pointing to the same pokemon struct in memory.
// If the pointers both contain the same address, then the second
// Pokemon is not a *copy* of the first Pokemon.
//
// This function doesn't (yet) check that the Pokemon's names match
// (but perhaps you could add that check yourself...).
static int is_copied_pokemon(Pokemon first, Pokemon second) {
    return (pokemon_id(first) == pokemon_id(second))
    &&  (first != second)
    &&  (pokemon_height(first) == pokemon_height(second))
    &&  (pokemon_weight(first) == pokemon_weight(second))
    &&  (pokemon_first_type(first) == pokemon_first_type(second))
    &&  (compare_strings(pokemon_name(first), pokemon_name(second)) == 1)
    &&  (pokemon_second_type(first) == pokemon_second_type(second));
}


// Write your own helper functions here!
// Creating a few more pokemon to use
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
static Pokemon create_venusaur(void) {
    Pokemon pokemon = new_pokemon(
            VENUSAUR_ID, VENUSAUR_NAME,
            VENUSAUR_HEIGHT, VENUSAUR_WEIGHT,
            VENUSAUR_FIRST_TYPE,
            VENUSAUR_SECOND_TYPE
    );
    return pokemon;
}

static Pokemon create_charmander(void) {
    Pokemon pokemon = new_pokemon(
            CHARMANDER_ID, CHARMANDER_NAME,
            CHARMANDER_HEIGHT, CHARMANDER_WEIGHT,
            CHARMANDER_FIRST_TYPE,
            CHARMANDER_SECOND_TYPE
    );
    return pokemon;
}

static Pokemon create_charmeleon(void) {
    Pokemon pokemon = new_pokemon(
            CHARMELEON_ID, CHARMELEON_NAME,
            CHARMELEON_HEIGHT, CHARMELEON_WEIGHT,
            CHARMELEON_FIRST_TYPE,
            CHARMELEON_SECOND_TYPE
    );
    return pokemon;
}

static void add_five_pokemon(Pokedex pokedex, Pokemon bulbasaur,
    Pokemon ivysaur, Pokemon venusaur, Pokemon charmeleon,
    Pokemon charmander) {
    printf("    ... Adding Bulbasaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);

    printf("    ... Adding Ivysaur to the Pokedex\n");
    add_pokemon(pokedex, ivysaur);

    printf("    ... Adding Venusaur to the Pokedex\n");
    add_pokemon(pokedex, venusaur);

    printf("    ... Adding Charmander to the Pokedex\n");
    add_pokemon(pokedex, charmander);

    printf("    ... Adding Charmeleon to the Pokedex\n");
    add_pokemon(pokedex, charmeleon);
}

static void find_pokemon(Pokedex pokedex, int amount) {
    int i = 0;
    while (i < amount) {
        find_current_pokemon(pokedex);
        next_pokemon(pokedex);
        i++;
    }
}
static void change_pokemon(Pokedex pokedex, int type, int amount) {
    int count = 0;
    while (count < amount) {
        if (type == 1) {
            next_pokemon(pokedex);
        } else {
            prev_pokemon(pokedex);
        }
        count++;
    }
}
