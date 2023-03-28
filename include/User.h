#ifndef USER_H

#define USER_H

#include "Command.h"



#define MAX_USER_NAME 255

#define MAX_USER_EMAIL 255



typedef struct User {

    unsigned int id;

    char name[MAX_USER_NAME+1];

    char email[MAX_USER_EMAIL+1];

    unsigned int age;

} User_t;

typedef struct Like {

    unsigned int id1;

    unsigned int id2;

} Like_User_t;


User_t* new_User();
Like_User_t* new_Like_User();

User_t* command_to_User(Command_t *cmd);
Like_User_t* command_to_Like_User(Command_t *cmd);



#endif

