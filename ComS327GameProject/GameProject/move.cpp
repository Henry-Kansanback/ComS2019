#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"
#include "event.h"
#include "io.h"
#include "npc.h"
#include "object.h"

void do_combat(dungeon *d, character *atk, character *def)
{
  //int can_see_atk, can_see_def;
  const char *organs[] = {
    "liver",                   /*  0 */
    "pancreas",                /*  1 */
    "heart",                   /*  2 */
    "eye",                     /*  3 */
    "arm",                     /*  4 */
    "leg",                     /*  5 */
    "intestines",              /*  6 */
    "gall bladder",            /*  7 */
    "lungs",                   /*  8 */
    "hand",                    /*  9 */
    "foot",                    /* 10 */
    "spinal cord",             /* 11 */
    "pituitary gland",         /* 12 */
    "thyroid",                 /* 13 */
    "tongue",                  /* 14 */
    "bladder",                 /* 15 */
    "diaphram",                /* 16 */
    "stomach",                 /* 17 */
    "pharynx",                 /* 18 */
    "esophagus",               /* 19 */
    "trachea",                 /* 20 */
    "urethra",                 /* 21 */
    "spleen",                  /* 22 */
    "ganglia",                 /* 23 */
    "ear",                     /* 24 */
    "subcutaneous tissue"      /* 25 */
    "cerebellum",              /* 26 */ /* Brain parts begin here */
    "hippocampus",             /* 27 */
    "frontal lobe",            /* 28 */
    "brain",                   /* 29 */
  };
  int part;
  int32_t damg;

  
  if(def->alive) {
    
    if(def == d->PC) 
      {
	if((damg = atk->damage->roll()) > d->PC->hp)
	  {
	    def->alive = 0;
	    charpair(def->position) = NULL;
	    {
	      if ((part = rand() % (sizeof (organs) / sizeof (organs[0]))) < 26) 
		{
		  io_queue_message("As %s%s eats your %s,", is_unique(atk) ? "" : "the ",
				   atk->name, organs[rand() % (sizeof (organs) /
							       sizeof (organs[0]))]);
		  io_queue_message("   ...you wonder if there is an afterlife.");
		  /* Queue an empty message, otherwise the game will not pause for *
		   * player to see above.                                          */
		  io_queue_message("");
		} 
	      else 
		{
		  io_queue_message("Your last thoughts fade away as "
				   "%s%s eats your %s...",
				   is_unique(atk) ? "" : "the ",
				   atk->name, organs[part]);
		  io_queue_message("");
		}
	      /* Queue an empty message, otherwise the game will not pause for *
	       * player to see above.                                          */
	      io_queue_message("");
	      
	      atk->kills[kill_direct]++;
	      atk->kills[kill_avenged] += (def->kills[kill_direct] +
					   def->kills[kill_avenged]);
	    }
	  } 
	else if(damg <= d->PC->hp) 
	  {
	    d->PC->hp = d->PC->hp - damg;
	    io_queue_message("%s%s hits you for %d !", is_unique(atk) ? "" : "the ", atk->name, damg);
	    //io_queue_message("You now have %d hp", d->PC->hp);
	    
	    //atk->kills[kill_direct]++;
	    //atk->kills[kill_avenged] += (def->kills[kill_direct] +
	    //				 def->kills[kill_avenged]);
	  }
	
      }
    else if(def != d->PC) 
      {
	if(!d->PC->equipped[0]){
	  if((damg = (d->PC->damage->roll()) + roll_damg(d)) > def->hp)
	    {
	      if(has_characteristic(def, BOSS))
		{
		    d->quit = 1;
		}
	      d->num_monsters--;
	      io_queue_message("You smite %s%s!", is_unique(def) ? "" : "the ", def->name);
	      def->alive = 0;
	      charpair(def->position) = NULL;
	    }
	  else if(damg <= def->hp)
	    {
	      def->hp = def->hp - damg;
	      io_queue_message("You hit %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
	    //io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
	      
	    }
	}
	else
	  {
	    
	    if((damg = roll_damg(d)) > def->hp)
	      {
		if(has_characteristic(def, BOSS))
		  {
		    d->quit = 1;
		  }
		d->num_monsters--;
		io_queue_message("You smite %s%s!", is_unique(def) ? "" : "the ", def->name);
		def->alive = 0;
		charpair(def->position) = NULL;
	      }
	    else if(damg <= def->hp)
	      {
		def->hp = def->hp - damg;
		io_queue_message("You hit %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		//io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
		
	      }
	  }
	
      }
  }
  
  
  //if (atk == d->PC) {
    //if((damg = d->PC->damage.roll()) >= def->hp){
  //io_queue_message("You smite %s%s!", is_unique(def) ? "" : "the ", def->name);
      //}
    //io_queue_message("You smite %s%s!", is_unique(def) ? "" : "the ", def->name); //if has killed
      // }
  
  /*can_see_atk = can_see(d, character_get_pos(d->PC),
                        character_get_pos(atk), 1, 0);
  can_see_def = can_see(d, character_get_pos(d->PC),
  character_get_pos(def), 1, 0);*/
  
  /*if (atk != d->PC && def != d->PC) {
    if (can_see_atk && !can_see_def) {
      io_queue_message("%s%s callously murders some poor, "
                       "defenseless creature.",
                       is_unique(atk) ? "" : "The ", atk->name);
    }
    if (can_see_def && !can_see_atk) {
      io_queue_message("Something kills %s%s.",
                       is_unique(def) ? "" : "the helpless ", def->name);
    }
    if (can_see_atk && can_see_def) {
      io_queue_message("You watch in abject horror as %s%s "
                       "gruesomely murders %s%s!",
                       is_unique(atk) ? "" : "the ", atk->name,
                       is_unique(def) ? "" : "the ", def->name);
    }
    }*/
}

void spellAttack(dungeon *d, character *atk, character *def, int spelltype, uint8_t sCost)
{
  const char *organs[] = {
    "liver",                   /*  0 */
    "pancreas",                /*  1 */
    "heart",                   /*  2 */
    "eye",                     /*  3 */
    "arm",                     /*  4 */
    "leg",                     /*  5 */
    "intestines",              /*  6 */
    "gall bladder",            /*  7 */
    "lungs",                   /*  8 */
    "hand",                    /*  9 */
    "foot",                    /* 10 */
    "spinal cord",             /* 11 */
    "pituitary gland",         /* 12 */
    "thyroid",                 /* 13 */
    "tongue",                  /* 14 */
    "bladder",                 /* 15 */
    "diaphram",                /* 16 */
    "stomach",                 /* 17 */
    "pharynx",                 /* 18 */
    "esophagus",               /* 19 */
    "trachea",                 /* 20 */
    "urethra",                 /* 21 */
    "spleen",                  /* 22 */
    "ganglia",                 /* 23 */
    "ear",                     /* 24 */
    "subcutaneous tissue"      /* 25 */
    "cerebellum",              /* 26 */ /* Brain parts begin here */
    "hippocampus",             /* 27 */
    "frontal lobe",            /* 28 */
    "brain",                   /* 29 */
  };

  int32_t damg;
  int part;

  if(def->alive) {
    

    //if for whatever reason a monster is able to do spell attack
    if(def == d->PC) 
      {
	//Special case if player is dumb enough or desperate enough to blast themself.
	if(atk == d->PC)
	{
	    if(d->PC->equipped[12])
	      {
		if(spelltype == 'a' && d->PC->equipped[12]->get_type() == objtype_WAND)
		  {
		    damg = d->PC->equipped[12]->roll_dice();
		  }
		else if(spelltype == 'b' && d->PC->equipped[12]->get_type() == objtype_SCROLL)
		  {
		    damg = d->PC->equipped[12]->roll_dice();
		  }
		else if(spelltype == 'c' && d->PC->equipped[12]->get_type() == objtype_BOOK)
		  {
		    damg = d->PC->equipped[12]->roll_dice();
		  }
		else
		  {
		    if(d->PC->equipped[12]->get_type() == objtype_SCROLL)
		      {
			damg = skewdamgScroll;
		      }
		    if(d->PC->equipped[12]->get_type() == objtype_WAND)
		      {
			damg = skewdamgWand;
		      }
		    if(d->PC->equipped[12]->get_type() == objtype_BOOK)
		      {
			damg = skewdamgBook;
		      }
		  }
		if((damg) > def->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    if(has_characteristic(def, BOSS))
		      {
			d->quit = 1;
		      }
		    
		    io_queue_message("You spatter across a the room!");
		    io_queue_message("Congratulations, you just played yourself");
		    io_queue_message("");
		    def->alive = 0;
		    charpair(def->position) = NULL;
		  }
		else if(damg <= def->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    def->hp = def->hp - damg;
		    io_queue_message("You blast yourelf for %d, idiot", damg);
		    io_queue_message("");
		    //io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
		  }
	      }
	    else if(!d->PC->equipped[12])
	      {
		if(spelltype == 'a')
		  {
		    damg = DamWand;
		  }
		else if(spelltype == 'b')
		  {
		    damg = DamScroll;
		  }
		else if(spelltype == 'c')
		  {
		    damg = DamBook;
		  }
		if((damg) > d->PC->hp)
		  {
		    def->alive = 0;
		    charpair(def->position) = NULL;
		    d->PC->mana = d->PC->mana - sCost;
		    //d->num_monsters--;
		    io_queue_message("You spatter across a the room!");
		    io_queue_message("Congratulations, you just played yourself");
		    io_queue_message("");
		    //def->alive = 0;
		    //charpair(def->position) = NULL;
		  }
		else if(damg <= d->PC->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    d->PC->hp = d->PC->hp - damg;
		    io_queue_message("You blast yourelf for %d, idiot", damg);
		    io_queue_message("");
		    //io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
		  }
	      }
	}
	else if((damg = atk->damage->roll()) > d->PC->hp)
	  {
	    def->alive = 0;
	    charpair(def->position) = NULL;
	    {
	      if ((part = rand() % (sizeof (organs) / sizeof (organs[0]))) < 26) 
		{
		  io_queue_message("As %s%s eats your %s,", is_unique(atk) ? "" : "the ",
				   atk->name, organs[rand() % (sizeof (organs) /
							       sizeof (organs[0]))]);
		  io_queue_message("   ...you wonder if there is an afterlife.");

		  io_queue_message("");
		} 
	      else 
		{
		  io_queue_message("Your last thoughts fade away as "
				   "%s%s eats your %s...",
				   is_unique(atk) ? "" : "the ",
				   atk->name, organs[part]);
		  io_queue_message("");
		}
	      /* Queue an empty message, otherwise the game will not pause for *
	       * player to see above.                                          */
	      io_queue_message("");
	      
	      atk->kills[kill_direct]++;
	      atk->kills[kill_avenged] += (def->kills[kill_direct] +
					   def->kills[kill_avenged]);
	    }
	  } 
	else if(damg <= d->PC->hp) 
	  {
	    d->PC->hp = d->PC->hp - damg;
	    io_queue_message("%s%s hits you for %d !", is_unique(atk) ? "" : "the ", atk->name, damg);
	    io_queue_message("");
	    //io_queue_message("You now have %d hp", d->PC->hp);
	    
	    //atk->kills[kill_direct]++;
	    //atk->kills[kill_avenged] += (def->kills[kill_direct] +
	    //				 def->kills[kill_avenged]);
	  }
	
      }
    else if(def != d->PC) 
      {
	//objtype_SCROLL, objtype_BOOK, objtype_WAND
	if((d->PC->mana <= 0))
	  {
	    io_queue_message("Not enough mana!");
	    
	  }
	else
	  {
	    if(d->PC->equipped[12])
	      {
		if(spelltype == 'a' && d->PC->equipped[12]->get_type() == objtype_WAND)
		  {
		    damg = d->PC->equipped[12]->roll_dice();
		  }
		else if(spelltype == 'b' && d->PC->equipped[12]->get_type() == objtype_SCROLL)
		  {
		    damg = d->PC->equipped[12]->roll_dice();
		  }
		else if(spelltype == 'c' && d->PC->equipped[12]->get_type() == objtype_BOOK)
		  {
		    damg = d->PC->equipped[12]->roll_dice();
		  }
		else
		  {
		    if(d->PC->equipped[12]->get_type() == objtype_SCROLL)
		      {
			damg = skewdamgScroll;
		      }
		    if(d->PC->equipped[12]->get_type() == objtype_WAND)
		      {
			damg = skewdamgWand;
		      }
		    if(d->PC->equipped[12]->get_type() == objtype_BOOK)
		      {
			damg = skewdamgBook;
		      }
		  }
		if((damg) > def->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    if(has_characteristic(def, BOSS))
		      {
			d->quit = 1;
		      }
		    d->num_monsters--;
		    //mvprintw(3,9,"You polymorphed %s%s into atoms!", is_unique(def) ? "" : "the ", def->name);
		    io_queue_message("You polymorphed %s%s into atoms!", is_unique(def) ? "" : "the ", def->name);
		    def->alive = 0;
		    charpair(def->position) = NULL;
		  }
		else if(damg <= def->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    def->hp = def->hp - damg;
		    //mvprintw(3,9,"You blast %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		    io_queue_message("You blast %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		    io_queue_message("");
		    //io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
		  }
	      }
	    else if(!d->PC->equipped[12])
	      {
		if(spelltype == 'a')
		  {
		    damg = DamWand;
		  }
		else if(spelltype == 'b')
		  {
		    damg = DamScroll;
		  }
		else if(spelltype == 'c')
		  {
		    damg = DamBook;
		  }
		if((damg) > def->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    if(has_characteristic(def, BOSS))
		      {
			d->quit = 1;
		      }
		    d->num_monsters--;
		    io_queue_message("You polymorphed %s%s into atoms!", is_unique(def) ? "" : "the ", def->name);
		    io_queue_message("");
		    def->alive = 0;
		    charpair(def->position) = NULL;
		  }
		else if(damg <= def->hp)
		  {
		    d->PC->mana = d->PC->mana - sCost;
		    def->hp = def->hp - damg;
		    //io_queue_message("You blast %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		    //mvprintw(3,9,"You blast %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		    io_queue_message("You blast %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		    io_queue_message("");
		    //io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
		  }
	      }
	  }
	
      }
  }
}

void blastArea(dungeon *d, pair_t center, uint8_t cost)
{
  pair_t nextpt;
  d->PC->mana = d->PC->mana - cost;
  for(nextpt[dim_y] = center[dim_y] - 1; nextpt[dim_y] < center[dim_y] + 1; nextpt[dim_y]++)
    {
      
      for(nextpt[dim_x] = center[dim_x] - 1; nextpt[dim_x] < center[dim_x] + 1; nextpt[dim_x]++)
	{
	  if(d->character_map[nextpt[dim_y]][nextpt[dim_x]])
	    {
	      spellAttack(d, d->PC, d->character_map[nextpt[dim_y]][nextpt[dim_x]], 'c', 0);
	    }
	}
    }
  
}


void rangedAttack(dungeon *d, character *atk, character *def)
{

  const char *organs[] = {
    "liver",                   /*  0 */
    "pancreas",                /*  1 */
    "heart",                   /*  2 */
    "eye",                     /*  3 */
    "arm",                     /*  4 */
    "leg",                     /*  5 */
    "intestines",              /*  6 */
    "gall bladder",            /*  7 */
    "lungs",                   /*  8 */
    "hand",                    /*  9 */
    "foot",                    /* 10 */
    "spinal cord",             /* 11 */
    "pituitary gland",         /* 12 */
    "thyroid",                 /* 13 */
    "tongue",                  /* 14 */
    "bladder",                 /* 15 */
    "diaphram",                /* 16 */
    "stomach",                 /* 17 */
    "pharynx",                 /* 18 */
    "esophagus",               /* 19 */
    "trachea",                 /* 20 */
    "urethra",                 /* 21 */
    "spleen",                  /* 22 */
    "ganglia",                 /* 23 */
    "ear",                     /* 24 */
    "subcutaneous tissue"      /* 25 */
    "cerebellum",              /* 26 */ /* Brain parts begin here */
    "hippocampus",             /* 27 */
    "frontal lobe",            /* 28 */
    "brain",                   /* 29 */
  };

  int32_t damg;
  int part;
  if(def->alive) {
    

    //if for whatever reason a monster is able to ranged attack
    if(def == d->PC) 
      {
	if((damg = atk->damage->roll()) > d->PC->hp)
	  {
	    def->alive = 0;
	    charpair(def->position) = NULL;
	    {
	      if ((part = rand() % (sizeof (organs) / sizeof (organs[0]))) < 26) 
		{
		  io_queue_message("As %s%s eats your %s,", is_unique(atk) ? "" : "the ",
				   atk->name, organs[rand() % (sizeof (organs) /
							       sizeof (organs[0]))]);
		  io_queue_message("   ...you wonder if there is an afterlife.");

		  io_queue_message("");
		} 
	      else 
		{
		  io_queue_message("Your last thoughts fade away as "
				   "%s%s eats your %s...",
				   is_unique(atk) ? "" : "the ",
				   atk->name, organs[part]);
		  io_queue_message("");
		}
	      /* Queue an empty message, otherwise the game will not pause for *
	       * player to see above.                                          */
	      io_queue_message("");
	      
	      atk->kills[kill_direct]++;
	      atk->kills[kill_avenged] += (def->kills[kill_direct] +
					   def->kills[kill_avenged]);
	    }
	  } 
	else if(damg <= d->PC->hp) 
	  {
	    d->PC->hp = d->PC->hp - damg;
	    io_queue_message("%s%s hits you for %d !", is_unique(atk) ? "" : "the ", atk->name, damg);
	    //io_queue_message("You now have %d hp", d->PC->hp);
	    
	    //atk->kills[kill_direct]++;
	    //atk->kills[kill_avenged] += (def->kills[kill_direct] +
	    //				 def->kills[kill_avenged]);
	  }
	
      }
    else if(def != d->PC) 
      {
	if(!d->PC->equipped[2]){
	  io_queue_message("You must have a ranged weapon equipped!");
	  
	}
	else
	  {
	    
	    if((damg = roll_damg(d)) > def->hp)
	      {
		if(has_characteristic(def, BOSS))
		  {
		    d->quit = 1;
		  }
		d->num_monsters--;
		io_queue_message("You smite %s%s!", is_unique(def) ? "" : "the ", def->name);
		def->alive = 0;
		charpair(def->position) = NULL;
	      }
	    else if(damg <= def->hp)
	      {
		def->hp = def->hp - damg;
		io_queue_message("You hit %s%s for %d !", is_unique(def) ? "" : "the ", def->name, damg);
		//io_queue_message("The %s%s now has %d", is_unique(def) ? "" : "the ", def->name, def->hp);
		
	      }
	  }
	
      }
  }
}


void move_character(dungeon *d, character *c, pair_t next)
{
  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x])) && (c != d->PC) && (charpair(next) != d->PC)) {
    //character *intile = charpair(next);
    pair_t intile_next;// = charpair(next)->position;
    uint16_t nel = 0;
    do{
      intile_next[dim_x] = charpair(next)->position[dim_x];
      intile_next[dim_y] = charpair(next)->position[dim_y];
      //nel = rand() % 8;

      switch (nel) {
      case (0):
	intile_next[dim_x] = intile_next[dim_x] - 1;
	intile_next[dim_y] = intile_next[dim_y] - 1;
	break;
      case (1):
	//intile_next[dim_x] = intile_next[dim_x] - 1;
	intile_next[dim_y] = intile_next[dim_y] - 1;
	break;
      case (2):
	intile_next[dim_x] = intile_next[dim_x] + 1;
	intile_next[dim_y] = intile_next[dim_y] - 1;
	break;
      case (3):
	intile_next[dim_x] = intile_next[dim_x] - 1;
	//intile_next[dim_y] = intile_next[dim_y] - 1;
	break;
      case (4):
	intile_next[dim_x] = intile_next[dim_x] + 1;
	//intile_next[dim_y] = intile_next[dim_y] - 1;
	break;
      case (5):
	intile_next[dim_x] = intile_next[dim_x] - 1;
	intile_next[dim_y] = intile_next[dim_y] + 1;
	break;
      case (6):
	//intile_next[dim_x] = intile_next[dim_x] - 1;
	intile_next[dim_y] = intile_next[dim_y] + 1;
	break;
      case (7):
	intile_next[dim_x] = intile_next[dim_x] + 1;
	intile_next[dim_y] = intile_next[dim_y] + 1;
	break;
      }
      nel++;
    }while((charpair(intile_next) || hardnesspair(intile_next)) && (nel < 8));

    if((charpair(intile_next) && hardnesspair(intile_next)))
      {
    character *intile = charpair(next);
    //d->character_map[next[dim_y]][next[dim_x]] = NULL;
    d->character_map[intile->position[dim_y]][intile->position[dim_x]] = NULL;
    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
    intile->position[dim_y] = c->position[dim_y];
    intile->position[dim_x] = c->position[dim_x];
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character_map[intile->position[dim_y]][intile->position[dim_x]] = intile;
    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;
      }
    else if(!charpair(intile_next) && !hardnesspair(intile_next))
      {
    character *intile = charpair(next);
    //d->character_map[next[dim_y]][next[dim_x]] = NULL;
    d->character_map[intile->position[dim_y]][intile->position[dim_x]] = NULL;
    intile->position[dim_y] = intile_next[dim_y];
    intile->position[dim_x] = intile_next[dim_x];
    d->character_map[intile->position[dim_y]][intile->position[dim_x]] = intile;

    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;
      }

    //do_combat(d, c, charpair(next));
  } else if(c != d->PC && !charpair(next)){// && (charpair(next) != d->PC)){
    /* No character in new position. */

    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;

  } else if(c == d->PC && charpair(next) && ((next[dim_y] != c->position[dim_y]) || (next[dim_x] != c->position[dim_x]))){

    do_combat(d, c, charpair(next));

  } else if(c == d->PC && !charpair(next) && ((next[dim_y] != c->position[dim_y]) ||
					      (next[dim_x] != c->position[dim_x]))) {
    if(objpair(next) && d->PC->numheld < 10)
      {
	int i = avail_invent(d);
	//d->PC->held[d->PC->numheld] = (object *) d->objmap[next[dim_y]][next[dim_x]];
	d->PC->held[i - 1] = (object *) d->objmap[next[dim_y]][next[dim_x]];
	d->objmap[next[dim_y]][next[dim_x]] = NULL;
	//io_queue_message("You picked up %s!", d->PC->held[d->PC->numheld]->get_name());
	io_queue_message("You picked up %s!", d->PC->held[i - 1]->get_name());
	d->PC->numheld++;
      }
    else if(d->PC->numheld >= 10 && objpair(next)){
      io_queue_message("Inventory Full!");
      //io_queue_message("Could not pick up %s!", d->objmap[next[dim_y]][next[dim_x]].get_name());
    }
    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;
  }
  else if(charpair(next) == d->PC) {
    do_combat(d, c, charpair(next));
  }
  
  if ((c == d->PC)) {
    pc_reset_visibility(d->PC);
    pc_observe_terrain(d->PC, d);
  }
}

void do_moves(dungeon *d)
{
  pair_t next;
  character *c;
  event *e;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
   * use to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                       */

  if (pc_is_alive(d)) {
    /* The PC always goes first one a tie, so we don't use new_event().  *
     * We generate one manually so that we can set the PC sequence       *
     * number to zero.                                                   */
    e = (event *) malloc(sizeof (*e));
    e->type = event_character_turn;
    /* Hack: New dungeons are marked.  Unmark and ensure PC goes at d->time, *
     * otherwise, monsters get a turn before the PC.                         */
    if (d->is_new) {
      d->is_new = 0;
      e->time = d->time;
    } else {
      e->time = d->time + (1000 / d->PC->speed);
    }
    e->sequence = 0;
    e->c = d->PC;
    heap_insert(&d->events, e);
  }

  while (pc_is_alive(d) &&
         (e = (event *) heap_remove_min(&d->events)) &&
         ((e->type != event_character_turn) || (e->c != d->PC))) {
    d->time = e->time;
    if (e->type == event_character_turn) {
      c = e->c;
    }
    if (!c->alive) {
      if (d->character_map[c->position[dim_y]][c->position[dim_x]] == c) {
        d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
      }
      if (c != d->PC) {
        event_delete(e);
      }
      continue;
    }

    npc_next_pos(d, (npc *) c, next);
    move_character(d, (npc *) c, next);

    heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
  }

  io_display(d);
  if (pc_is_alive(d) && e->c == d->PC) {
    c = e->c;
    d->time = e->time;
    /* Kind of kludgey, but because the PC is never in the queue when   *
     * we are outside of this function, the PC event has to get deleted *
     * and recreated every time we leave and re-enter this function.    */
    e->c = NULL;
    event_delete(e);
    io_handle_input(d);
  }
}

void dir_nearest_wall(dungeon *d, character *c, pair_t dir)
{
  dir[dim_x] = dir[dim_y] = 0;

  if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2) {
    dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
  }
  if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2) {
    dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
  }
}

uint32_t against_wall(dungeon *d, character *c)
{
  return ((mapxy(c->position[dim_x] - 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x] + 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] - 1) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] + 1) == ter_wall_immutable));
}

uint32_t in_corner(dungeon *d, character *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

static void new_dungeon_level(dungeon *d, uint32_t dir)
{
  /* Eventually up and down will be independantly meaningful. *
   * For now, simply generate a new dungeon.                  */

  switch (dir) {
  case '<':
  case '>':
    new_dungeon(d);
    break;
  default:
    break;
  }
}


uint32_t move_pc(dungeon *d, uint32_t dir)
{
  pair_t next;
  uint32_t was_stairs = 0;
  const char *wallmsg[] = {
    "There's a wall in the way.",
    "BUMP!",
    "Ouch!",
    "You stub your toe.",
    "You can't go that way.",
    "You admire the engravings.",
    "Are you drunk?"
  };

  next[dim_y] = d->PC->position[dim_y];
  next[dim_x] = d->PC->position[dim_x];


  switch (dir) {
  case 1:
  case 2:
  case 3:
    next[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    next[dim_y]--;
    break;
  }
  switch (dir) {
  case 1:
  case 4:
  case 7:
    next[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    next[dim_x]++;
    break;
  case '<':
    if (mappair(d->PC->position) == ter_stairs_up) {
      was_stairs = 1;
      new_dungeon_level(d, '<');
    }
    break;
  case '>':
    if (mappair(d->PC->position) == ter_stairs_down) {
      was_stairs = 1;
      new_dungeon_level(d, '>');
    }
    break;
  }

  if (was_stairs) {
    return 0;
  }

  if ((dir != '>') && (dir != '<') && (mappair(next) >= ter_floor)) {
    move_character(d, d->PC, next);
    dijkstra(d);
    dijkstra_tunnel(d);

    return 0;
  } else if (mappair(next) < ter_floor) {
    io_queue_message(wallmsg[rand() % (sizeof (wallmsg) /
                                       sizeof (wallmsg[0]))]);
    io_display(d);
  }

  return 1;
}
