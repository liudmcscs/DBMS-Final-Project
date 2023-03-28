#ifndef DB_UTIL_H

#define DB_UTIL_H

#include "Command.h"

#include "Table.h"



typedef struct State {

    int saved_stdout;

} State_t;



State_t* new_State();

void print_prompt(State_t *state);

int where(User_t *user, Command_t *cmd);
int where_like(Like_User_t *like_user, Command_t *cmd);

//void aggre(User_t *user,Command_t *cmd);
void aggregation(Table_t *table, Like_t *like, Command_t *cmd,int offset);
void aggregation_like(Table_t *table, Like_t *like, Command_t *cmd);

int print_user(User_t *user, Like_User_t *like_user, SelectArgs_t *sel_args, Command_t *cmd,int offset);
void print_join(Table_t *table, Like_t *like,/* User_t *user, Like_User_t *like_user,*/ SelectArgs_t *sel_args, Command_t *cmd);
//int print_like_user(User_t *user, SelectArgs_t *sel_args, Command_t *cmd);

void print_users(Table_t *table, Like_t *like, int *idxList, size_t idxListLen, Command_t *cmd);
//void print_like_users(Table_t *table, Like_t *like, int *idxList, size_t idxListLen, Command_t *cmd);

int parse_input(char *input, Command_t *cmd);

void handle_builtin_cmd(Table_t *table, Command_t *cmd, State_t *state);

int handle_query_cmd(Table_t *table, Like_t *like, Command_t *cmd);

void handle_update_cmd(Table_t *table, Command_t *cmd);

void handle_delete_cmd(Table_t *table, Command_t *cmd);

int handle_insert_cmd(Table_t *table, Like_t *like, Command_t *cmd);

int handle_select_cmd(Table_t *table, Like_t *like, Command_t *cmd);

void print_help_msg();
void swap(Like_User_t  *a, Like_User_t *b);
int Partition(Like_User_t* like_user, int front, int end);
int Partition2(Like_User_t* like_user, int front, int end);
void quickSort(Like_User_t* like_user, int front, int end);
void quickSort2(Like_User_t* like_user, int front, int end);
int binarysearch(Like_User_t* like_user, int search, int n);
int binarysearch2(Like_User_t* like_user, int search, int n);


#endif

