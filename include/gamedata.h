#ifndef __URD_GAMEDATA_H
#define __URD_GAMEDATA_H

#define MAX_REPLY 4096

/* Single command instance in stack of commands */
struct cmd_elem {
	char *msg;
	struct cmd_elem *next; /* Next element in stack */
};

/* Traditionally speaking, this stack grows from bottom to top, words are
 * inserted in the opposite way of a traditional sentence, the last word
 * inserted is the actual command. 
 * The bottom is the last word, the top is the first word inserted on the stack
 * aka the last word of the sentence.
 * EXAMPLE:
 * "Attack monster with sword" 
 * TOP    _ SWORD
 *        _ MONSTER
 * BOTTOM _ ATTACK
 */

/* Stack of commands */
struct cmd_stack {
	int args; /* Number of words in command */
	struct cmd_elem *bottom; /* Bottom of the stack */
	struct cmd_elem *top; /* Top of the stack */
};

typedef enum {
	URD_BEGIN, /* Beginning of the game, show intro message */
	URD_CREATION, /* Party creation time */
	URD_DUNGEON, /* Party exploring dungeon */
	URD_BATTLE, /* Party in battle, turn based combat with enemy */
	URD_RANKINGS /* Showing ranking table */
} state_t;

/* Base structure inherited by all other members of the state union */
struct base_state {
	state_t type;
	char description[MAX_REPLY];
	struct cmd_stack *cmd;
};

/* All possible game states */
struct urd_state {
	union {

		struct base_state base;

		struct {
			state_t type;
			char description[MAX_REPLY];
			struct cmd_stack *cmd;
			int intro_message;
		} begin;

		struct {
			state_t type;
			char description[MAX_REPLY];
			struct cmd_stack *cmd;
		} creation;

		struct {
			state_t type;
			char description[MAX_REPLY];
			struct cmd_stack *cmd;
		} dungeon;

		struct {
			state_t type;
			char description[MAX_REPLY];
			struct cmd_stack *cmd;
		} battle;

		struct {
			state_t type;
			char description[MAX_REPLY];
			struct cmd_stack *cmd;
		} rankings;
	};
};

struct urd_item {
	char *name;
	int item_id; /* Item ID for stacking and lookup */
	int uniq_id; /* Unique item ID for specific item */
	struct urd_item *s_next; /* Next sibling in the pile */
	struct urd_item *s_prev; /* Previous sibing in the pile */
	struct urd_item *next; /* Next item in the inventory */
	struct urd_item *prev; /* Previous item in the inventory */
};

struct urd_inventory {
	int count; /* Total amount of items in inventory */
	int uniq_count; /* Total amount of individual item types */
	int gold; /* Amount of gold */
	struct urd_item *first; /* First item in inventory */
	struct urd_item *last; /* Last item in inventory */
};

/* Single fighting character in a party */
struct urd_fighter {
	char *first_name;
	char *last_name;
	char *nickname;
	int level; /* Level of an individual fighter */
	int str; /* Strength influences damage multiplier of weapons */
	int dex; /* Dexterity influences initiative and accuracy */
	int sta; /* Stamina influences maximum weight carry and defense */
	int intl; /* Intelligence influences bonuses from items */
	int luck; /* Luck influences critical hits and trap resistance */
	struct urd_inventory items; /* Each fighter has its own inventory */
	struct urd_fighter *next; /* Next fighter in the party */
};

/* Party of fighters exploring the world of urd */
struct urd_party {
	int avg_level; /* Level average of the whole party */
	struct urd_fighter *first; /* First member of the party, ordered by 
				    * initiative 
				    */
	struct urd_fighter *current; /* Current character's turn */
};

/* Status of the game, it's pretty much the game world and everything related 
 * to it 
 */
struct urd_status {
	int exiting; /* When the client enters an exiting status */
	const char *command; /* Currently parsed command, player's input */
	size_t command_size;
	struct cmd_stack *action_stack; /* The command post-parsing in stack */
	char output[MAX_REPLY]; /* Reply to send to player */
	struct urd_party party; /* Party of fighters */
	struct urd_state state; /* Data of the game state */
};

/* Low level data passed around by telnet */
struct telnet_data {
	int sock; 
	telnet_t *telnet;
	struct urd_status game;
};

#endif 
