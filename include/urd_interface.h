#ifndef __URD_INTERFACE_H
#define __URD_INTERFACE_H
#include <gamedata.h>

void urd_main(int sockfd);
int parse_defaults(struct urd_status *game);
void print_description(struct urd_status *game);

void generate_begin(struct urd_status *game);
void urd_update_begin(struct urd_status *game);

void generate_rankings(struct urd_status *game);
void urd_update_rankings(struct urd_status *game);

#endif
