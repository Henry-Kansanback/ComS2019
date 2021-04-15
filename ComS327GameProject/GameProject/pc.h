#ifndef PC_H
# define PC_H

# include <stdint.h>
# include <vector>

# include "dims.h"
# include "character.h"
# include "dungeon.h"
# include "object.h"
# include "dice.h"

# define costScroll 80
# define costBook 60
# define costWand 30

//Default Reducers for costs for matching spell types
# define reduxScroll 40
# define reduxBook 30
# define reduxWand 15

//Default Reducers for costs for skewed spell types
# define skewScroll 25
# define skewBook 15
# define skewWand 10

//Default Damage without equipment.
# define DamScroll 30
# define DamBook   15
# define DamWand   20

//Default skewed damage with the wrong equipment
# define skewdamgScroll 35
# define skewdamgBook 20
# define skewdamgWand 25

class pc : public character {
 public:
  ~pc() {}
  terrain_type known_terrain[DUNGEON_Y][DUNGEON_X];
  uint8_t visible[DUNGEON_Y][DUNGEON_X];
  //std::vector<object> held;
  object *held[10];
  uint8_t numheld;
  //std::vector<object> equipped;
  //object *equipped[12];
  object *equipped[13];
  uint8_t numequipt;
  uint8_t available_invent;
  uint8_t mana;
  uint8_t manacap;
  uint8_t manaregen;
  //dice damage;
  //uint32_t hp;

};

void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint8_t costTest(dungeon *d, int spelltype);
uint8_t finSpellCost(dungeon *d, int spelltype, uint8_t reduxCost);
int avail_invent(dungeon *d);
int32_t roll_damg(dungeon *d);
int avail_equipt(dungeon *d);
void drop(dungeon *d, int key);
void equip(dungeon *d, int key);
void unequip(dungeon *d, int key);
uint32_t pc_next_pos(dungeon *d, pair_t dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);
void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter);
terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x);
void pc_init_known_terrain(pc *p);
void pc_observe_terrain(pc *p, dungeon *d);
int32_t is_illuminated(pc *p, int16_t y, int16_t x);
void pc_reset_visibility(pc *p);

#endif
