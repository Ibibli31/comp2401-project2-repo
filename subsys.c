#include "subsystem.h"
#include <string.h>

/* initializes the memory pointed to by the subsystem with name and status.
 
 in/out subsystem:  Pointer to the Subsystem structure to initialize
 in name:           Name to assign to the subsystem
 in status:         Initial status of the subsystem
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_SUCCESS otherwise */
int subsys_init(Subsystem *subsystem, const char *name, char status){
  // checks pointers
  if (subsystem == NULL || name == NULL) {
    return ERR_NULL_POINTER; /* or some other error code */
  }

  //Set name to subsystem
  strncpy(subsystem->name, name, sizeof(subsystem->name) - 1);

  // null terminator
  subsystem->name[sizeof(subsystem->name) - 1] = '\0';

  // assigns a default status 
  subsystem->status = status;

  // initializes data
  subsystem->data = 0;

  printf("'%s' has been added to the subsystem collection.\n", name);
  
  return ERR_SUCCESS;
}

/* prints the data of a single subsystem.
 
 in subsystem: Pointer to the Subsystem structure to print
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_SUCCESS otherwise */
int subsys_print(Subsystem *subsystem) {
  // checks if the subsystem is null
  if (subsystem == NULL) {
    return ERR_NULL_POINTER;
  }

  // prints name with fixed spacing
  printf("Name: %-16s; Status: ", subsystem->name);

  // prints status
  subsys_status_print(subsystem);
  
  // collects subsystem's data
  int subsystemData = (subsystem->status >> STATUS_DATA) & 1;
  unsigned int data;

  // prints data if the subsystem has some
  if (subsystemData == 1) {
    subsys_data_get(subsystem, &data);
    printf("Data: %08X\n", data);
  } else {
    printf("Data: 0\n");
  }

  return ERR_SUCCESS;
}

/* sets the status value of the subsystem.
 
 in/out subsystem: Pointer to the Subsystem structure
 in status:        Status bit to set
 in value:         Value to assign to the status bit

 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_INVALID_STATUS if the provided status or value is out of range
 - ERR_SUCCESS otherwise */
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value) {
  // checks if the subsystem exists
  if (subsystem == NULL){
    return ERR_NULL_POINTER;
  }

  // determines if the status is valid and the range of value lines up
  if ((status == STATUS_POWER || status == STATUS_DATA || status == STATUS_ACTIVITY || status == STATUS_ERROR) && value > 1) {
    printf("The value is invalid for the type of status.\n");
    return ERR_INVALID_STATUS;
  }

  if ((status == STATUS_PERFORMANCE || status == STATUS_RESOURCE) && value > 3) {
    printf("The value is invalid for the type of status.\n");
    return ERR_INVALID_STATUS;
  }

  // Modify bits
  switch(status){
    case STATUS_POWER:
    case STATUS_DATA:
    case STATUS_ACTIVITY:
    case STATUS_ERROR:
      //Clear bit
      subsystem->status &= ~(1 << status);
      //Set new value
      subsystem-> status |= (value << status);
      break;
    case STATUS_PERFORMANCE:
      subsystem ->status &= ~(3 << STATUS_PERFORMANCE); //Clear bits 3-2
      subsystem->status |= (value << STATUS_PERFORMANCE); //Set new value
      break;
    case STATUS_RESOURCE:
      subsystem->status &= ~(3 << STATUS_RESOURCE); //Clear bits 1-0
      subsystem->status |= (value << STATUS_RESOURCE); //Set new value
      break;

    default:
      printf("The status number is invalid.\n");
      return ERR_INVALID_STATUS;
  }
  printf("'%s' status was successfully updated.\n", subsystem->name);
  return ERR_SUCCESS;
}

/* prints the status of the subsystem to the screen.
 
 in subsystem: Pointer to the Subsystem structure
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_SUCCESS otherwise */
int subsys_status_print(const Subsystem *subsystem){
  //Make sure subsystem exisits
  if (subsystem == NULL) {
    return ERR_NULL_POINTER;
  }

  int power = (subsystem->status >> STATUS_POWER) & 1;
  int data = (subsystem->status >> STATUS_DATA) & 1;
  int activity = (subsystem->status >> STATUS_ACTIVITY) & 1;
  int error = (subsystem->status >> STATUS_ERROR) & 1;
  int performance = (subsystem->status >> STATUS_PERFORMANCE) & 3; //2 bit 
  int resource = (subsystem->status >> STATUS_RESOURCE) & 3; //2 bit

  printf("[PWR: %d | DATA: %d | ACT: %d | ERR: %d | PERF: %d | RES: %d ]; ",
         power, data, activity, error, performance, resource);

  return ERR_SUCCESS;
}

/* sets the data parameter for the subsystem and saves old data before overwriting it.
 
 in/out subsystem: Pointer to the Subsystem structure
 in new_data:      New data to set for the subsystem
 in/out old_data:  Pointer to store the old data if it's not NULL
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_NO_DATA if the new data is zero
 - ERR_SUCCESS otherwise */
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data){
  // check for null values
  if (subsystem == NULL) {
    return ERR_NULL_POINTER;
  }

  // checks if the new data is null
  if (new_data == 0) {
    printf("The data has a value of 0.\n");
    return ERR_NO_DATA;
  }

  // checks if there's old data to be saved
  if (old_data != NULL) {
    *old_data = subsystem->data;
  }

  // sets the new data to the subsystem data
  subsystem->data = new_data;

  // updates the data status to true
  subsys_status_set(subsystem, 6, 1);

  printf("'%s' data has successfully been set.\n", subsystem->name);
  return ERR_SUCCESS;
}

/* gets the data of the subsystem.
 
 in subsystem: Pointer to the Subsystem structure
 out data:     Pointer to store the retrieved data
 Returns:
 - ERR_NULL_POINTER if a null pointer is passed
 - ERR_NO_DATA if there is no data queued
 - ERR_SUCCESS otherwise */
int subsys_data_get(Subsystem *subsystem, unsigned int *data) {
  // check for null values
  if (subsystem == NULL ) {
    return ERR_NULL_POINTER;
  }

  // check if there's any data queued
  int subsystemData = (subsystem->status >> STATUS_DATA) & 1;
  if (subsystemData == 0) {
    *data = 0;
    return ERR_NO_DATA;
  }

  // copies subsystem data to data
  *data = subsystem->data;
  // set data field to 0
  subsystem->data = 0;
  //Clear bit
  subsystem->status &= ~(1 << STATUS_DATA);

  return ERR_SUCCESS;
}

