#ifndef CREATURE_ORDER_H_INCLUDED
#define CREATURE_ORDER_H_INCLUDED


void initialise_order(creature_order_t *order);

void print_order(creature_order_t *order);

void add_to_rear(creature_order_t *order, square_t *square, type_t type);

void add_after_node(creature_order_t *order, creature_t *node, square_t *square, type_t type);

void delete_node(creature_order_t *order, creature_t *node);


#endif