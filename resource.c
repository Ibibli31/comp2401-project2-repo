#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Resource functions */

/**
 * Creates a new `Resource` object.
 *
 * Allocates memory for a new `Resource` and initializes its fields.
 * The `name` is dynamically allocated.
 *
 * @param[out] resource      Pointer to the `Resource*` to be allocated and initialized.
 * @param[in]  name          Name of the resource (the string is copied).
 * @param[in]  amount        Initial amount of the resource.
 * @param[in]  max_capacity  Maximum capacity of the resource.
 */
void resource_create(Resource **resource, const char *name, int amount, int max_capacity) {
    // Allocate memory for the resource structure
    *resource = (Resource*)malloc(sizeof(Resource));
    if (*resource == NULL) {
        return;
    }

    // Allocate and copy the name
    (*resource)->name = (char*)malloc(strlen(name) + 1);
    if ((*resource)->name == NULL) {
        free(*resource);
        *resource = NULL;
        return;
    }
    strcpy((*resource)->name, name);

    // Initialize the fields
    (*resource)->amount = amount;
    (*resource)->max_capacity = max_capacity;
}

/**
 * Destroys a `Resource` object.
 *
 * Frees all memory associated with the `Resource`.
 *
 * @param[in,out] resource  Pointer to the `Resource` to be destroyed.
 */
void resource_destroy(Resource *resource) {
  if (resource != NULL) {
    free(resource->name);  // Free the dynamically allocated name
    free(resource);        // Free the resource struct itself
  }
}

/* ResourceAmount functions */

/**
 * Initializes a `ResourceAmount` structure.
 *
 * Associates a `Resource` with a specific `amount`.
 *
 * @param[out] resource_amount  Pointer to the `ResourceAmount` to initialize.
 * @param[in]  resource         Pointer to the `Resource`.
 * @param[in]  amount           The amount associated with the `Resource`.
 */
void resource_amount_init(ResourceAmount *resource_amount, Resource *resource, int amount) {
    resource_amount->resource = resource;
    resource_amount->amount = amount;
}

/**
 * Initializes the `ResourceArray`.
 *
 * Allocates memory for the array of `Resource*` pointers and sets initial values.
 *
 * @param[out] array  Pointer to the `ResourceArray` to initialize.
 */
void resource_array_init(ResourceArray *array) {
  array->resources = (Resource**)malloc(sizeof(Resource*));
  if (array->resources == NULL) {
      return;
  }
  array->size = 0;
  array->capacity = 1;
}

/**
 * Cleans up the `ResourceArray` by destroying all resources and freeing memory.
 *
 * Iterates through the array, calls `resource_destroy` on each `Resource`,
 * and frees the array memory.
 *
 * @param[in,out] array  Pointer to the `ResourceArray` to clean.
 */
void resource_array_clean(ResourceArray *array) {
  if (array != NULL) {
    // Destroy all resources in the array
    for (int i = 0; i < array->size; i++) {
        resource_destroy(array->resources[i]);
    }
    // Free the array itself
    free(array->resources);
    array->resources = NULL;
    array->size = 0;
    array->capacity = 0;
  }
}

/**
 * Adds a `Resource` to the `ResourceArray`, resizing if necessary (doubling the size).
 *
 * Resizes the array when the capacity is reached and adds the new `Resource`.
 * Use of realloc is NOT permitted.
 * 
 * @param[in,out] array     Pointer to the `ResourceArray`.
 * @param[in]     resource  Pointer to the `Resource` to add.
 */
void resource_array_add(ResourceArray *array, Resource *resource) {
      // Check if we need to resize
      if (array->size >= array->capacity) {
        // Double the capacity
        int new_capacity = array->capacity * 2;
        Resource **new_array = (Resource**)malloc(sizeof(Resource*) * new_capacity);
        if (new_array == NULL) {
            return;
        }
        
        // Copy existing elements
        for (int i = 0; i < array->size; i++) {
            new_array[i] = array->resources[i];
        }
        
        // Free old array and update pointer
        free(array->resources);
        array->resources = new_array;
        array->capacity = new_capacity;
    }
    
    // Add the new resource
    array->resources[array->size] = resource;
    array->size++;
}
