#include "subsystem.h"
#include <string.h>

/* verifies if the subsystem with the given name exists in the collection.
 
 in collection: Pointer to the SubsystemCollection to search
 in name:       Name of the subsystem to verify
 Returns:
 - The index of the found subsystem
 - ERR_SYS_NOT_FOUND if the subsystem is not present
 - ERR_NO_DATA if the subsystem is not found */
int verify_subsystem_exists(SubsystemCollection *collection, const char *name){
  int id = subsys_find(collection, name);

  // checks if subsystem_id is valid
  if (id == ERR_SYS_NOT_FOUND) {
    //Do proper verfication
    printf("'%s' is not in the subsystem collection.\n", name);
    return ERR_SYS_NOT_FOUND;
  }

  return id;
}

/* initializes an empty SubsystemCollection structure.
 
 in/out subsystems: Pointer to the SubsystemCollection to initialize
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_SUCCESS if the initialization is successful */
int subsys_collection_init(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER; /* or other error code */
    }

    /* Initialize the size of the collection to 0 */
    subsystems->size = 0;

    /* Return success */
    return ERR_SUCCESS;
}

/* appends a copy of the structure to the end of the collection.
 
 in/out subsystems: Pointer to the SubsystemCollection to append to
 in subsystem:      Pointer to the Subsystem to append
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed for either argument
 - ERR_MAX_CAPACITY if the collection is full
 - ERR_SUCCESS if the append is successful */
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem)
{
    // checks if pointers are null
    if (subsystems == NULL || subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // checks if there's capacity left
    if (subsystems->size >= MAX_ARR) {
        return ERR_MAX_CAPACITY;
    }

    // adds the provided subsystem to the collection
    subsystems->subsystems[subsystems->size] = *subsystem;

    // increases the size of the collection
    subsystems->size++;

    return ERR_SUCCESS;
}

/* searches for the first subsystem with the same name and returns its index.
 
 in subsystems: Pointer to the SubsystemCollection to search
 in name:       Name of the subsystem to find
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - The index of the found subsystem
 - ERR_SYS_NOT_FOUND if no subsystem with that name is found */
int subsys_find(const SubsystemCollection *subsystems, const char *name) {
  // validates the pointers
  if (subsystems == NULL || name == NULL) {
    return ERR_NULL_POINTER;
  }

  // loop through all the collections
  for (unsigned int i = 0; i < subsystems->size; i++) {
    // checks if subsystem name matches char name
    if (strcmp(subsystems->subsystems[i].name, name) == 0) {
      return i;
    }
  }
    // if none was found, return error
    return ERR_SYS_NOT_FOUND;
}

/* prints all subsystems in the collection.
 
 in subsystems: Pointer to the SubsystemCollection to print
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_NO_DATA if the collection is empty
 - ERR_SUCCESS if the printing is successful */
int subsys_collection_print(SubsystemCollection *subsystems) {
  // validates the pointers
  if (subsystems == NULL) {
    return ERR_NULL_POINTER;
  }

  // checks that subsystems collection isnt empty
  if (subsystems->size == 0) {
    printf("\nThere are no subsystems in the collection.\n");
    return ERR_NO_DATA;
  }

  // loops through the array and prints the every subsystem
  for (unsigned int i = 0; i < subsystems->size; i++) {
    subsys_print(&subsystems->subsystems[i]);
  }

  return ERR_SUCCESS;
}

/* removes the subsystem at the specified index if it exists.
 
 in/out subsystems: Pointer to the SubsystemCollection to modify
 in index: Index of the subsystem to remove
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_NO_DATA if the collection is empty
 - ERR_INVALID_INDEX if the index is out of range
 - ERR_SUCCESS if the removal is successful */
int subsys_remove(SubsystemCollection *subsystems, int index){
  
  // checks for any invalid or null values
  if (subsystems == NULL ) {
    return ERR_NULL_POINTER;
  }

  // checks if the collection is empty
  if (subsystems->size == 0) {
    return ERR_NO_DATA;
  }

  // checks if the index is valid
  if (index < 0 || index >= (int)subsystems->size){
    return ERR_INVALID_INDEX;
  }
 
  // lets user know which subsystem is getting removed
  printf("Subsystem '%s' was deleted successfully\n", subsystems->subsystems[index].name);

  // shifts all elements to left (after index)
  for (unsigned int i = index; i < subsystems->size - 1; i++){
    subsystems->subsystems[i] = subsystems->subsystems[i + 1];
  }

  // reduce size
  subsystems->size--;

  return ERR_SUCCESS;
}

/* filters subsystems according to user input.
 
 in src: Pointer to the source SubsystemCollection to filter
 out dest: Pointer to the destination SubsystemCollection to store the results
 in filter: Pointer to an array of filter criteria (must be 8 characters long)
 Returns:
 - ERR_NULL_POINTER if any null pointer is passed
 - ERR_NO_DATA if the filter string is not 8 characters long
 - ERR_SUCCESS if filtering is completed successfully */
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
  // checks if collection is null
  if(src == NULL){
    return ERR_NULL_POINTER;
  }

  // verifies that filter string is 8 characters, and only has 1,0, or *
  if (strlen((const char *)filter) != 8){
    printf("The string is not 8 characters long.\n");
    return ERR_NO_DATA;
  }

  // creates the filter and wildcard masks
  unsigned char filterMask = 0b00000000;
  unsigned char wildcardMask = 0b00000000;

  // goes through filter string and assign values to masks
  for (int i = 0; i < 8; i++){
    switch(filter[i]){
      case '1':
        filterMask |= (1 << (7 - i));
        break;
      case '*':
        wildcardMask |= (1 << (7 - i));
        break;
      case '0':
        break;
      default:
        // will return error if string contains an unknown character
        printf("The string contains a character other than 1, 0 or *.\n");
        return ERR_NO_DATA;
    }
  }

  // flips each bit of filter mask
  filterMask = ~filterMask;

  // resets size of destination array to 0
  dest->size = 0;

  // checks every subsystem of the collection and if it passes the filter
  for (unsigned int i = 0; i < src->size; i++){
    // checks the filter
    unsigned char result = (filterMask ^ src->subsystems[i].status) | wildcardMask;
    if(result == 0b11111111){
      // copies the Subsystems that Match the filter stuff 
      dest->subsystems[dest->size++] = src->subsystems[i]; 
    }
  }

  // prints the filtered subsystem
  if (dest->size > 0) {
    subsys_collection_print(dest);
  } else {
    printf("No subsystem was apart of the collection.\n");
  }

  return ERR_SUCCESS;
}
