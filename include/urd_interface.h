#ifndef __URD_INTERFACE_H
#define __URD_INTERFACE_H

_Noreturn void urd_main(int sockfd);
int parse_defaults(struct urd_status *game);
void print_description(struct urd_status *game);

/* Command stack operations */
struct cmd_stack* init_cmd_stack();
void push_cmd_stack(struct cmd_stack *s, const char *msg, size_t length);
char* pop_cmd_stack(struct cmd_stack *s);
char* peek_cmd_stack(struct cmd_stack *s);
void free_cmd_stack(struct cmd_stack *s);
struct cmd_stack* obtain_command(const char *cmd, size_t length,
					struct ignored_words *iw);

/* Ignored words operation */
struct ignored_words* init_ignored_words(sqlite3 *db);
void add_ignored_word(struct ignored_words *iw, const char *word);
void drop_ignored_word(struct ignored_words *iw);
void free_ignored_words(struct ignored_words *iw);

void generate_begin(struct urd_status *game);
void urd_update_begin(struct urd_status *game);

void generate_rankings(struct urd_status *game);
void urd_update_rankings(struct urd_status *game);

#endif
