
/*
     Phillip Taylor B.Sc. (HONS) Software Engineering. Texas Hold'em Poker Software
     Copyright (C) 2008  Phillip Taylor

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdbool.h>
#include "card.h"

#ifndef PLAYER_H
#define PLAYER_H

enum POLL_STATE
{
	USERNAME, //waiting for the user to provide a name
	PASSWORD, //waiting for their password
	TABLE,    //waiting for their table choice
	ON_TABLE  //table knows the status of the user.
};

typedef struct player_
{
    char *name;
    enum POLL_STATE state;
    int elapsed_time;
    int socket;
    card *card_one;
    card *card_two;
} player;

player *player_new();
void player_free(player *p);

#endif
