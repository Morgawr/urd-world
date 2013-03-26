#ifndef __URD_INTERFACE_H
#define __URD_INTERFACE_H
#include <gamedata.h>

void urd_main(int sockfd);
int parse_defaults(struct urd_status *game);
void print_description(struct urd_status *game);

/* Command stack operations */
struct cmd_stack* init_cmd_stack();
void push_cmd_stack(struct cmd_stack *s, const char *msg, size_t length);
char* pop_cmd_stack(struct cmd_stack *s);
char* peek_cmd_stack(struct cmd_stack *s);
void free_cmd_stack(struct cmd_stack *s);
struct cmd_stack* obtain_command(const char *cmd, size_t length);

void generate_begin(struct urd_status *game);
void urd_update_begin(struct urd_status *game);

void generate_rankings(struct urd_status *game);
void urd_update_rankings(struct urd_status *game);

#endif
