#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

/* Event functions */

/**
 * Initializes an `Event` structure.
 *
 * Sets up an `Event` with the provided system, resource, status, priority, and amount.
 *
 * @param[out] event     Pointer to the `Event` to initialize.
 * @param[in]  system    Pointer to the `System` that generated the event.
 * @param[in]  resource  Pointer to the `Resource` associated with the event.
 * @param[in]  status    Status code representing the event type.
 * @param[in]  priority  Priority level of the event.
 * @param[in]  amount    Amount related to the event (e.g., resource amount).
 */
void event_init(Event *event, System *system, Resource *resource, int status, int priority, int amount) {
    event->system = system;
    event->resource = resource;
    event->status = status;
    event->priority = priority;
    event->amount = amount;
}

/* EventQueue functions */

/**
 * Initializes the `EventQueue`.
 *
 * Sets up the queue for use, initializing any necessary data (e.g., semaphores when threading).
 *
 * @param[out] queue  Pointer to the `EventQueue` to initialize.
 */
void event_queue_init(EventQueue *queue) {
  queue->head = NULL;
}

/**
 * Cleans up the `EventQueue`.
 *
 * Frees any memory and resources associated with the `EventQueue`.
 * 
 * @param[in,out] queue  Pointer to the `EventQueue` to clean.
 */
void event_queue_clean(EventQueue *queue) {
  EventNode *current = queue->head;
  while (current != NULL) {
      EventNode *next = current->next;
      free(current);
      current = next;
  }
  queue->head = NULL;
}

/**
 * Pushes an `Event` onto the `EventQueue`.
 *
 * Adds the event to the queue in a thread-safe manner, maintaining priority order (highest first).
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[in]     event  Pointer to the `Event` to push onto the queue.
 */
void event_queue_push(EventQueue *queue, const Event *event) {
  EventNode *new_node = malloc(sizeof(EventNode));
  new_node->event = *event;
  new_node->next = NULL;

  // If queue is empty or new event has higher priority than head
  if (queue->head == NULL || queue->head->event.priority < event->priority) {
      new_node->next = queue->head;
      queue->head = new_node;
      return;
  }

  // Find the insertion point
  EventNode *current = queue->head;
  while (current->next != NULL && 
         current->next->event.priority >= event->priority) {
      current = current->next;
  }

  // Insert after current
  new_node->next = current->next;
  current->next = new_node;
}

/**
 * Pops an `Event` from the `EventQueue`.
 *
 * Removes the highest priority event from the queue in a thread-safe manner.
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[out]    event  Pointer to the `Event` structure to store the popped event.
 * @return               Non-zero if an event was successfully popped; zero otherwise.
 */
int event_queue_pop(EventQueue *queue, Event *event) {
  if (queue->head == NULL) {
    return STATUS_EMPTY;  // Queue is empty
  }

  // Copy the event data
  *event = queue->head->event;

  // Remove the head node
  EventNode *temp = queue->head;
  queue->head = queue->head->next;
  free(temp);

  return STATUS_OK;  // Successfully popped event
}
