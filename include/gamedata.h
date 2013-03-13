
#define MAX_REPLY 4096

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
	char *name;
	int level; /* Level of an individual fighter */
	struct urd_inventory items; /* Each fighter has its own inventory */
	struct urd_fighter *next; /* Next fighter in the party */
};

/* Party of fighters exploring the world of urd */
struct urd_party {
	int avg_level; /* Level average of the whole party */
	struct urd_fighter *first; /* First member of the party, ordered by 
				    * initiative 
				    */
};

/* Status of the game, it's pretty much the game world and everything related 
 * to it 
 */
struct urd_status {
	int exiting; /* When the client enters an exiting status */
	char *command; /* Currently parsed command, player's input */
	char output[MAX_REPLY]; /* Reply to send to player */
	struct urd_party party; /* Party of fighters */
};

/* Low level data passed around by telnet */
struct telnet_data {
	int sock; 
	telnet_t *telnet;
	struct urd_status game;
};
