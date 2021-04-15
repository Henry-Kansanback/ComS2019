#include <cstdlib>
#include <cstring>
#include <ncurses.h>

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "io.h"
#include "object.h"

uint32_t pc_is_alive(dungeon *d)
{
  return d->PC->alive;
}

void place_pc(dungeon *d)
{
  d->PC->position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->PC->position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));

  pc_init_known_terrain(d->PC);
  pc_observe_terrain(d->PC, d);
}

int avail_invent(dungeon *d)
{
    for(int i = 0; i < 10; i++)
      {
	if(!d->PC->held[i])
	  {
	    return i + 1;
	  }
      }
    return 0;
}

int avail_equipt(dungeon *d)
{
  for(int i = 0; i < 12; i++)
    {
      if(!d->PC->equipped[i])
	{
	  return i + 1;
	}
    }
  return 0;
}

int32_t roll_damg(dungeon *d)
{
  int32_t dmgfinal;
  uint8_t i;
  dmgfinal = 0;
  for(i = 0; i < 12; i++)
    {
      if(d->PC->equipped[i])
	{
	  dmgfinal = dmgfinal + d->PC->equipped[i]->roll_dice();
	}
    }
  return dmgfinal;
}


uint8_t costTest(dungeon *d, int spelltype)
{
  uint8_t sCost = 0;
  if(d->PC->equipped[12])
    {
      if(d->PC->equipped[12]->get_type() == objtype_WAND)
	{
	  if(spelltype == 'a')
	    {
	      sCost = finSpellCost(d, spelltype, reduxWand);
	    }
	  else
	    {
	      sCost = finSpellCost(d, spelltype, skewWand);
	    }
	}
      else if(d->PC->equipped[12]->get_type() == objtype_SCROLL)
	{
	  if(spelltype == 'b')
	    {
	      sCost = finSpellCost(d, spelltype, reduxScroll);
	    }
	  else
	    {
	      sCost = finSpellCost(d, spelltype, skewScroll);
	    }
	}
      else if(d->PC->equipped[12]->get_type() == objtype_BOOK)
	{
	  if(spelltype == 'c')
	    {
	      sCost = finSpellCost(d, spelltype, reduxBook);
	    }
	  else
	    {
	      sCost = finSpellCost(d, spelltype, skewBook);
	    }
	}
    }
  else
    {
      
      if(spelltype == 'a')
	{
	  sCost = costWand;
	}
      else if(spelltype == 'b')
	{
	  sCost = costScroll;
	}
      else if(spelltype == 'c')
	{
	  sCost = costBook;
	}
      /*
	if(d->PC->equipped[12]->type == objtype_WAND)
	{
	sCost = finSpellCost(d, spelltype, skewWand);
	}
	else if(d->PC->equipped[12]->type == objtype_SCROLL)
	{
	sCost = finSpellCost(d, spelltype, skewScroll);
	}
	else if(d->PC->equipped[12]->type == objtype_BOOK)
	{
	sCost = finSpellCost(d, spelltype, skewBook);
	}
      */
    }
  return sCost;
}

uint8_t finSpellCost(dungeon *d, int spelltype, uint8_t reduxCost)
{
  //uint8_t finalcost;
  if(spelltype == 'a')
    {
      if(costWand <= reduxCost)
	{
	  return costWand;
	}
      if(costWand > reduxCost)
	{
	  return costWand - reduxCost;
	}
    }
  if(spelltype == 'b')
    {
      if(costScroll <= reduxCost)
	{
	  return costScroll;
	}
      if(costScroll > reduxCost)
	{
	  return costScroll - reduxCost;
	}
    }
  if(spelltype == 'c')
    {
      if(costBook <= reduxCost)
	{
	  return costBook;
	}
      if(costBook > reduxCost)
	{
	  return costBook - reduxCost;
	}
    }
  return 0;
}


void drop(dungeon *d, int key)
{
  //int i = avail_invent(d);
  if(d->PC->held[key] && !(d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]]))
    {
	//d->PC->held[d->PC->numheld] = (object *) d->objmap[next[dim_y]][next[dim_x]];
	//d->PC->held[i - 1] = (object *) d->objmap[next[dim_y]][next[dim_x]];
	d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->held[key];
	const char *nam = d->PC->held[key]->get_name();
	d->PC->held[key] = NULL;
	//d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d-;
	//io_queue_message("You picked up %s!", d->PC->held[d->PC->numheld]->get_name());
	io_queue_message("You dropped %s!", nam);
	d->PC->numheld--;
    }
}


void equip(dungeon *d, int key)
{
  int32_t typ = d->PC->held[key]->get_type();
  if(typ < objtype_SCROLL || (typ == objtype_SCROLL) || (typ == objtype_BOOK) || (typ == objtype_WAND))
    {
      typ = typ - 1;
      //switch(key) {
      //case (0):
      if(typ < 12 && !d->PC->equipped[typ])
	{
	  if(typ < objtype_SCROLL)
	    {
	  d->PC->equipped[typ] = d->PC->held[key];
	  d->PC->held[key] = NULL;
	  d->PC->speed = d->PC->speed + d->PC->equipped[typ]->get_speed();
	  d->PC->numheld--;
	  d->PC->numequipt++;
	    }
	  else
	    {
	      d->PC->equipped[12] = d->PC->held[key];
	      d->PC->held[key] = NULL;
	      d->PC->speed = d->PC->speed + d->PC->equipped[12]->get_speed();
	      d->PC->numheld--;
	      d->PC->numequipt++;
	    }
	}
      else
	{
	  if((typ + 1) == objtype_RING)
	    {
	      if(!d->PC->equipped[typ + 1])
		{
		  d->PC->equipped[typ + 1] = d->PC->held[key];
		  d->PC->held[key] = NULL;
		  d->PC->speed = d->PC->speed + d->PC->equipped[typ]->get_speed();
		  d->PC->numheld--;
		  d->PC->numequipt++;
		}
	      else
		{
		  object *temp = d->PC->equipped[typ];
		  d->PC->equipped[typ] = d->PC->held[key];
		  d->PC->speed = d->PC->speed + d->PC->equipped[typ]->get_speed();
		  d->PC->speed = d->PC->speed - d->PC->held[key]->get_speed();
		  d->PC->held[key] = temp;
		  d->PC->numheld--;
		  d->PC->numequipt++;
		}
	    }
	  else
	    {
	      if(typ < 12)
		{
	      object *temp = d->PC->equipped[typ];
	      d->PC->speed = d->PC->speed + d->PC->equipped[typ]->get_speed();
	      d->PC->speed = d->PC->speed - d->PC->held[key]->get_speed();
	      d->PC->equipped[typ] = d->PC->held[key];
	      d->PC->held[key] = temp;
	      d->PC->numheld--;
	      d->PC->numequipt++;
		}
	      else if(typ == objtype_SCROLL || typ == objtype_BOOK || typ == objtype_WAND)
		{
	      object *temp = d->PC->equipped[12];
	      d->PC->speed = d->PC->speed + d->PC->equipped[12]->get_speed();
	      d->PC->speed = d->PC->speed - d->PC->held[key]->get_speed();
	      d->PC->equipped[12] = d->PC->held[key];
	      d->PC->held[key] = temp;
	      d->PC->numheld--;
	      d->PC->numequipt++;
		}
	    }
	}
      
    }
}

void unequip(dungeon *d, int key)
{


  uint8_t i;

  if((i = avail_invent(d)) != 0)
    {
      d->PC->held[i - 1] = d->PC->equipped[key];
      d->PC->speed = d->PC->speed - d->PC->held[i - 1]->get_speed();
      d->PC->equipped[key] = NULL;
      d->PC->numequipt--;
      d->PC->numheld++;
    }
  
}


void config_pc(dungeon *d)
{
  static dice pc_dice(0, 1, 4);
  
  d->PC = new pc;

  d->PC->symbol = '@';

  place_pc(d);

  d->PC->speed = PC_SPEED;
  d->PC->alive = 1;
  d->PC->sequence_number = 0;
  d->PC->hp = 500;
  d->PC->mana = 100;
  d->PC->manacap = 100;
  d->PC->manaregen = 10;
  d->PC->kills[kill_direct] = d->PC->kills[kill_avenged] = 0;
  d->PC->color.push_back(COLOR_WHITE);
  d->PC->damage = &pc_dice;
  d->PC->name = "Isabella Garcia-Shapiro";

  d->character_map[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC;
  int i = 0;  
  d->PC->numheld = 0;
  for(i = 0; i < 10; i++)
    {
      d->PC->held[i] = NULL;
    }
  d->PC->numequipt = 0;
  for(i = 0; i < 13; i++)
    {
      d->PC->equipped[i] = NULL;
    }

  
  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, d->PC)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, d->PC, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->PC->position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->PC->position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->PC->position[dim_x] + dir[dim_x],
              d->PC->position[dim_y] + dir[dim_y]) &&
      ((charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

uint32_t pc_in_room(dungeon *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->PC->position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->PC->position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->PC->position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->PC->position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}

void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter)
{
  p->known_terrain[pos[dim_y]][pos[dim_x]] = ter;
  p->visible[pos[dim_y]][pos[dim_x]] = 1;
}

void pc_reset_visibility(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->visible[y][x] = 0;
    }
  }
}

terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x)
{
  if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) {
    io_queue_message("Invalid value to %s: %d, %d", __FUNCTION__, y, x);
  }

  return p->known_terrain[y][x];
}

void pc_init_known_terrain(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->known_terrain[y][x] = ter_unknown;
      p->visible[y][x] = 0;
    }
  }
}

void pc_observe_terrain(pc *p, dungeon *d)
{
  pair_t where;
  int16_t y_min, y_max, x_min, x_max;

  y_min = p->position[dim_y] - PC_VISUAL_RANGE;
  if (y_min < 0) {
    y_min = 0;
  }
  y_max = p->position[dim_y] + PC_VISUAL_RANGE;
  if (y_max > DUNGEON_Y - 1) {
    y_max = DUNGEON_Y - 1;
  }
  x_min = p->position[dim_x] - PC_VISUAL_RANGE;
  if (x_min < 0) {
    x_min = 0;
  }
  x_max = p->position[dim_x] + PC_VISUAL_RANGE;
  if (x_max > DUNGEON_X - 1) {
    x_max = DUNGEON_X - 1;
  }

  for (where[dim_y] = y_min; where[dim_y] <= y_max; where[dim_y]++) {
    where[dim_x] = x_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_x] = x_max;
    can_see(d, p->position, where, 1, 1);
  }
  /* Take one off the x range because we alreay hit the corners above. */
  for (where[dim_x] = x_min - 1; where[dim_x] <= x_max - 1; where[dim_x]++) {
    where[dim_y] = y_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_y] = y_max;
    can_see(d, p->position, where, 1, 1);
  }       
}

int32_t is_illuminated(pc *p, int16_t y, int16_t x)
{
  return p->visible[y][x];
}

void pc_see_object(character *the_pc, object *o)
{
  if (o) {
    o->has_been_seen();
  }
}
