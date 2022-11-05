#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "world.h"
#include "creatureHelper.h"

// Initialses the order for animals to act.
void initialise_order(creature_order_t *order) {
  assert(order);
  order->front = order->rear = NULL;
}

// Prints the order, used for testing.
void print_order(creature_order_t *order) {
  for (creature_t *current = order->front; current; current = current->next) {
    printf("%d, ", current->id);
  }
  printf("\n");
}

// Adds an elemement to the end of the order.
void add_to_rear(creature_order_t *order, square_t *square, type_t type) {
  creature_t *item = create_creature(square, type);

  // Empty list case
  if (!order->front) {
    order->front = order->rear = item;
    return;
  }

  // Default case
  (order->rear)->next = item;
  order->rear = item;
}

/* 
	Adds an element after a specific given node in the order.
	This is used for when 2 creatures reproduce, placing their child in between their place in the order.
*/
void add_after_node(creature_order_t *order, creature_t *node, square_t *square, type_t type) {
  if (node == order->rear) {
    add_to_rear(order, square, type);
  } else {
    creature_t *item = create_creature(square, type);
    item->next = node->next;
    node->next = item;
  }
}

// This deletes a node from the order, freeing the memory for that node.
void delete_node(creature_order_t *order, creature_t *node) {
  node->current_pos->creature = NULL;
  creature_t *current;
  creature_t *next;
  if (order->rear == order->front) {
    order->front = order->rear = NULL;
  } else {
    current = order->front;
    next = current->next;
    if (node == current) {
      order->front = next;
      
    } else {
      while (next != node) {
        current = next;
        next = current->next;
      }
      current->next = next->next;
      if (!current->next) {
        order->rear = current;
      }
      
    }
  }
  
  free(node);
}