
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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logging.h"
#include "card.h"
#include "player.h"

player *player_new()
{
	player *p = (player*) malloc (sizeof (player));

	p->name = NULL;
	p->state = USERNAME;
	p->elapsed_time = 0;
	p->socket = 0;

	p->cards[0] = NULL;
	p->cards[1] = NULL;

	return p;
}

void player_free(player *p)
{
	free(p);
}

