#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"


void free_wp(int);
void print_wp();
int judge_wp();

void init_wp_pool();





typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
        int value;
        int newvalue;
        char type;
        char Enb;
        char str[32];
       
	/* TODO: Add more members if necessary */


} WP;

#endif
