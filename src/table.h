
#include "player.h"
<<<<<<< HEAD
#include "card.h"
=======
>>>>>>> 7b4db5f0b185c64c2686b826939adea25b7b8b1b
#include "stack.h"

#ifndef TABLE_H
#define TABLE_H

enum TABLE_STATE
{
	WAITING_FOR_PLAYERS,
	IN_PROGRESS,
	FINISHED
};

typedef struct table_
{
	char *name;
	enum TABLE_STATE state;
	player **players;
	int num_players;
	int current_player;
	stack *card_deck;
} table;

table *table_new(char *name);
void table_add_player(table *t, player *p);
void table_broadcast(table *t, char *message, ...);
void table_state_changed(table *t, player *p);

void deal_out_new_cards(table *t);

#endif

