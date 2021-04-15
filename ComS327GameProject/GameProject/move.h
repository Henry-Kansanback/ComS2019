#ifndef MOVE_H
# define MOVE_H

# include <stdint.h>

# include "dims.h"

class character;
class dungeon;

void rangedAttack(dungeon *d, character *atk, character *def);
void spellAttack(dungeon *d, character *atk, character *def, int spelltype, uint8_t sCost);
void blastArea(dungeon *d, pair_t center, uint8_t cost);
void next_move(dungeon *d,
               character *c,
               pair_t goal_pos,
               pair_t next_pos);
void do_moves(dungeon *d);
void dir_nearest_wall(dungeon *d, character *c, pair_t dir);
uint32_t in_corner(dungeon *d, character *c);
uint32_t against_wall(dungeon *d, character *c);
uint32_t move_pc(dungeon *d, uint32_t dir);
void move_character(dungeon *d, character *c, pair_t next);

#endif
