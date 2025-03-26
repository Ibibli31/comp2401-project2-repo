#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

#define DEFAULT_CHOICE -1

int print_menu(int *choice);

int main(void) {
    // Use a while loop to print the menu for the user and call the appropriate functions.
    // The definitions above are provided to assist with this.
    int choice = DEFAULT_CHOICE;

    // Create and initalize Subsystem collection
    SubsystemCollection collection, filteredCollection;
    subsys_collection_init(&collection);

    Subsystem subsystem;
    char name[MAX_STR];
    unsigned char status;
    unsigned char value;
    unsigned int currentData;
    unsigned int savedData;
    unsigned char filterStr;
    int subsysId;

    while(choice != 0){
      // gets user's choice
      print_menu(&choice);

      switch(choice){
        case 1:
          // asks user for a subsystem name
          printf("\nEnter a name: ");
          scanf("%31s", name);
          while (getchar() != '\n');

          // We dont need to check if user used spaces, because it states in the doc not too !
          
          // initialize the subsystem
          subsys_init(&subsystem, name, 0);

          // adds the subsystem to the collection, if there's space
          subsys_append(&collection, &subsystem);
          break;
        
        case 2:
          // asks user for a subsystem name
          printf("\nEnter subsystem name to print: ");
          scanf("%31s", name);
          while (getchar() != '\n');

          // verifies that the subsystem exists
          subsysId = verify_subsystem_exists(&collection, name);

          // prints subsytem if it exists
          if (subsysId != ERR_NO_DATA) {
            subsys_print(&collection.subsystems[subsysId]);
          }
          break;

        case 3:
          // prints all the subsystems
          subsys_collection_print(&collection);
          break;

        case 4:
          // asks user for a subsystem name, status and value
          printf("\nEnter <Subsystem Name> <Status ID; 7,6,5,4,2,0> <New Value (0-3)>: ");
          scanf("%31s %hhu %hhu", name, &status, &value);
          while (getchar() != '\n');

          // verifies that the subsystem exists
          subsysId = verify_subsystem_exists(&collection, name);

          // updates the subsystem status
          if (subsysId != ERR_NO_DATA) {
            subsys_status_set(&collection.subsystems[subsysId], status, value);
          }

          break;
        
        case 5:
          // asks user for a subsystem name
          printf("\nEnter subsystem name to remove: ");
          scanf("%31s", name);
          while (getchar() != '\n');

          // verifies that the subsystem exists
          subsysId = verify_subsystem_exists(&collection, name);

          // removes the subsystem from the collection
          if (subsysId != ERR_NO_DATA) {
            subsys_remove(&collection, subsysId);
          }

          break;
        
        case 6:
          // asks user for a subsystem name
          printf("\nEnter filter string (8 characters of 1, 0, *): ");
          scanf("%s", &filterStr);
          while (getchar() != '\n');

          // initializes the filtered collection
          subsys_collection_init(&filteredCollection);

          // filters the collection
          subsys_filter(&collection, &filteredCollection, &filterStr);
          break;

        case 7:
          // asks user for a subsystem name
          printf("\nEnter <Subsystem Name> <Data, uppercase hex without 0x>: ");
          scanf("%31s %x", name, &currentData);
          while (getchar() != '\n');

          // verifies that the subsystem exists
          subsysId = verify_subsystem_exists(&collection, name);

          // sets the data in the subsytem
          if (subsysId != ERR_NO_DATA) {
            subsys_data_set(&collection.subsystems[subsysId], currentData, &savedData);
          }

          break;
        case 0:
          printf("Exiting the program.\n");
          break;
        default:
          printf("Enter a valid menu option.\n");
      }
    }

    return 0;
}

/* 
    Prints a menu to the user and prompts them until they enter a valid menu option.

    out: *choice, an integer corresponding to a MENU_ definition defining the choice made by the user.

    Returns: 
        - ERR_SUCCESS if there is no error
        - ERR_NULL_POINTER if `choice` is NULL

*/
int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
        
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}
