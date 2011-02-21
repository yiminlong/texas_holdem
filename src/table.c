
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#include "util.h"
#include "table.h"
#include "card.h"
#include "logging.h"
#include "player.h"
#include "stack.h"
#include "card.h"

void clean_up_game(table *t);

void init_new_game(table *t);

table *table_new(char *name)
{
	table *t = malloc(sizeof(table));
	
	t->name = name;
	t->state = WAITING_FOR_PLAYERS;
	t->players = malloc(sizeof(player*) * 3);
	t->num_players = 0;
	t->current_player = 0;
	t->deck = NULL;

	return t;
}

void table_add_player(table *t, player *p)
{

	if (t->num_players == 3)
	{
		logging_critical("Can't add player %s to table %s because limit has been reached.", p->name, t->name);
		return;
	}

	*(t->players + t->num_players) = p;
	t->num_players++;

	send_str(p->socket, "Welcome to the table. Please wait for others to join.\n");
	send_str(p->socket, "(you can talk to other users in the room whilst you wait)\n");

	logging_debug("\nNow %i players on table %s. Waiting for %d more.\n",
		t->num_players,
		t->name,
		(3 - t->num_players)
	);

	table_broadcast(t, "Player %s has joined %s.\n", p->name, t->name);

	if (t->num_players == 3)
		init_new_game(t);

}

void table_broadcast(table *t, char *message, ...)
{
	va_list ap;
	char arg_merged[250];

	player *p;
	int i;
		
	va_start(ap, message);
	vsnprintf(arg_merged, 250, message, ap);
	va_end(ap);

	for (i = 0; i < t->num_players; i++)
	{
		p = t->players[i];
		send_str(p->socket, arg_merged);
	}
}

void init_new_game(table *t)
{
	int i;
	player *p;

	t->state = IN_PROGRESS;
	t->current_player = 0;
	t->card_deck = NULL;

	logging_debug("A game has started on table: %s!", t->name);
	table_broadcast(t, "The game has started!!!\n");
	table_broadcast(t, "\n-- BEGIN LIST OF PLAYERS --\n");
	
	for (i = 0; i < t->num_players; i++)
	{
		p = t->players[i];
		table_broadcast(t, "%d: %s\n", (i + 1), p->name);
	}

	table_broadcast(t, "-- END LIST OF PLAYERS --\n");

	//print the game rules
	table_broadcast(t, "Hopefully you already know how to play\n");
	table_broadcast(t, "Texas Hold'em Poker. You will be dealt\n");
	table_broadcast(t, "Your two cards and you make your choices\n");
	table_broadcast(t, "(when prompted to) of\n");
	table_broadcast(t, "b <<amount>> to bet or raise\n");
	table_broadcast(t, "c            to call\n");
	table_broadcast(t, "f            to fold\n");

	deal_out_new_cards(t);

	t->deck = generate_new_deck();
	
	//deal out the cards
	for (i = 0; i < t->num_players; i++)
	{
		p = t->players[i];
		p->card_one = (card*) stack_pop(t->deck);
		send_str(t->players[i]->socket, "Your first card is a %s\n", card_tostring(p->card_one));
		logging_info("%s dealt %s", t->players[i]->name, card_tostring(p->card_one));
	}

	for (i = 0; i < t->num_players; i++)
	{
		p = t->players[i];
		p->card_two = (card*) stack_pop(t->deck);
		send_str(t->players[i]->socket, "Your second card is a %s\n", card_tostring(p->card_two));
		logging_info("%s dealt %s", t->players[i]->name, card_tostring(p->card_two));
	}

}

//This is the work horse of the system. Any player on a
//table who sends a message gets recieved here. The following
//message codes are treated as genuine:

//f            (for fold)
//c            (for call)
//r <<amount>> (for bet / raise)

//The application treats those codes as genuine
//and all the rest as chat messages.

void table_state_changed(table *t, player *p)
{
	char *s;
	
	s = recv_str(p->socket);

	table_broadcast(t, "CHAT %s: %s\n", p->name, s);

}

void clean_up_game(table *t)
{
	int i;

	if (t->card_deck != NULL)
		free(t->card_deck);

	for (i = 0; i < t->num_players; i++)
	{
		if (t->players[i]->cards[0] != NULL)
		{
			free(t->players[i]->cards[0]);
			free(t->players[i]->cards[1]);
		}
	}
}

void deal_out_new_cards(table *t)
{
	int i;
	card *tmp;
	card *one, *two;

	clean_up_game(t);

	t->card_deck = generate_new_deck();

	//burn a card.
	tmp = (card*) stack_pop(t->card_deck);

	logging_debug("table %s, burning: %s", t->name, card_tostring(tmp));

	//first pass
	for (i = 0; i < t->num_players; i++)
	{
		t->players[i]->cards[0] = (card*) stack_pop(t->card_deck);
	}

	//second pass
	for (i = 0; i < t->num_players; i++)
	{
		t->players[i]->cards[1] = (card*) stack_pop(t->card_deck);

	//	send_str(t->players[i]->socket,
	//	logging_debug("%s, you have been dealt:\n%s,\n%s\n", t->players[i]->name, card_tostring(t->players[i]->cards[0]), card_tostring(t->players[i]->cards[1]));
		send_str(t->players[i]->socket, "%s, you have been dealt: %s, %s ", t->players[i]->name, card_tostring(t->players[i]->cards[0]), card_tostring(t->players[i]->cards[1]));
	}
	
}

