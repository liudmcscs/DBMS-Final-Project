#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Table.h"

///
/// Allocate a Table_t struct, then initialize some attributes, and
/// load table if the `file_name` is given
///
Table_t *new_Table(char *file_name) {
    Table_t *table = (Table_t*)malloc(sizeof(Table_t));
    memset((void*)table, 0, sizeof(Table_t));
    table->capacity = INIT_TABLE_SIZE;
    table->len = 0;
    table->users = (User_t*)malloc(
                            sizeof(User_t) * INIT_TABLE_SIZE);
    table->cache_map = (unsigned char*)malloc(sizeof(char)*INIT_TABLE_SIZE);
    memset(table->cache_map, 0, sizeof(char)*INIT_TABLE_SIZE);
    table->fp = NULL;
    table->file_name = NULL;
    load_table(table, file_name);
    return table;
}


Like_t *new_Like(char *file_name) {
    Like_t *like = (Like_t*)malloc(sizeof(Like_t));
    memset((void*)like, 0, sizeof(Like_t));
    like->capacity = INIT_TABLE_SIZE;
    like->len = 0;
    like->like_user = (Like_User_t*)malloc(
                            sizeof(Like_User_t) * INIT_TABLE_SIZE);
    like->cache_map = (unsigned char*)malloc(sizeof(char)*INIT_TABLE_SIZE);
    memset(like->cache_map, 0, sizeof(char)*INIT_TABLE_SIZE);
    like->fp = NULL;
    like->file_name = NULL;
    load_like(like, file_name);
    return like;
}
///
/// Add the `User_t` data to the given table
/// If the table is full, it will allocate new space to store more
/// user data
/// return 1 when the data successfully add to table
///
int add_User(Table_t *table, User_t *user) {
    size_t idx;
    User_t *usr_ptr;
    if (!table || !user) {
        return 0;
    }
    // Check id doesn't exist in the table
    for (idx = 0; idx < table->len; idx++) {
        usr_ptr = get_User(table, idx);
        if (usr_ptr->id == user->id) {
            return 0;
        }
    }
    if (table->len == table->capacity) {
        User_t *new_user_buf = (User_t*)malloc(sizeof(User_t)*(table->len+EXT_LEN));
        unsigned char *new_cache_buf = (unsigned char *)malloc(sizeof(unsigned char)*(table->len+EXT_LEN));

        memcpy(new_user_buf, table->users, sizeof(User_t)*table->len);

        memset(new_cache_buf, 0, sizeof(unsigned char)*(table->len+EXT_LEN));
        memcpy(new_cache_buf, table->cache_map, sizeof(unsigned char)*table->len);


        free(table->users);
        free(table->cache_map);
        table->users = new_user_buf;
        table->cache_map = new_cache_buf;
        table->capacity += EXT_LEN;
    }
    idx = table->len;
    memcpy((table->users)+idx, user, sizeof(User_t));
    table->cache_map[idx] = 1;
    table->len++;
    return 1;
}



int add_Like_User(Like_t *like, Like_User_t *like_user)
{
	size_t idx;
   // Like_User_t *like_usr_ptr;
    if (!like || !like_user) {
        return 0;
    }
    // Check id doesn't exist in the table
    //for (idx = 0; idx < like->len; idx++) {
      //  like_usr_ptr = get_Like_User(like, idx);
       // if (like_usr_ptr->id1 == like_user->id1) {
       //     return 0;
        //}
   // }
    if (like->len == like->capacity) {
        Like_User_t *new_like_user_buf = (Like_User_t*)malloc(sizeof(Like_User_t)*(like->len+EXT_LEN));
        unsigned char *new_like_cache_buf = (unsigned char *)malloc(sizeof(unsigned char)*(like->len+EXT_LEN));

        memcpy(new_like_user_buf, like->like_user, sizeof(Like_User_t)*like->len);

        memset(new_like_cache_buf, 0, sizeof(unsigned char)*(like->len+EXT_LEN));
        memcpy(new_like_cache_buf, like->cache_map, sizeof(unsigned char)*like->len);


        free(like->like_user);
        free(like->cache_map);
        like->like_user = new_like_user_buf;
        like->cache_map = new_like_cache_buf;
        like->capacity += EXT_LEN;
    }
    idx = like->len;
    memcpy((like->like_user)+idx, like_user, sizeof(Like_User_t));
    like->cache_map[idx] = 1;
    like->len++;
    return 1;
}
///
/// Return value is the archived table len
///
int archive_table(Table_t *table) {
    size_t archived_len;
    struct stat st;

    if (table->fp == NULL) {
        return 0;
    }
    if (stat(table->file_name, &st) == 0) {
        archived_len = st.st_size / sizeof(User_t);
    } else {
        archived_len = 0;
    }
    fwrite((void*)(table->users+archived_len), \
            sizeof(User_t), table->len-archived_len, \
            table->fp);

    fclose(table->fp);
    free(table->file_name);
    table->fp = NULL;
    table->file_name = NULL;
    return table->len;
}

int archive_like(Like_t *like)
{
    size_t archived_len;
    struct stat st;

    if (like->fp == NULL) {
        return 0;
    }
    if (stat(like->file_name, &st) == 0) {
        archived_len = st.st_size / sizeof(Like_User_t);
    } else {
        archived_len = 0;
    }
    fwrite((void*)(like->like_user+archived_len), \
            sizeof(Like_User_t), like->len-archived_len, \
            like->fp);

    fclose(like->fp);
    free(like->file_name);
    like->fp = NULL;
    like->file_name = NULL;
    return like->len;
}
///
/// Loading the db file will overwrite the existed records in table,
/// only if the ``file_name`` is NULL
/// Return: the number of records in the db file
///
int load_table(Table_t *table, char *file_name) {
    size_t archived_len;
    struct stat st;
    if (table->fp != NULL) {
        fclose(table->fp);
        free(table->file_name);
        table->fp = NULL;
        table->file_name = NULL;
    }
    if (file_name != NULL) {
        table->len = 0;
        memset(table->cache_map, 0, sizeof(char)*INIT_TABLE_SIZE);
        if (stat(file_name, &st) != 0) {
            //Create new file
            table->fp = fopen(file_name, "wb");
        } else {
            archived_len = st.st_size / sizeof(User_t);
            if (archived_len > table->capacity) {
                User_t *new_user_buf = (User_t*)malloc(sizeof(User_t)*(archived_len+EXT_LEN));
                unsigned char *new_cache_buf = (unsigned char *)malloc(sizeof(unsigned char)*(archived_len+EXT_LEN));

                memset(new_cache_buf, 0, sizeof(unsigned char)*(archived_len+EXT_LEN));

                free(table->users);
                free(table->cache_map);
                table->users = new_user_buf;
                table->cache_map = new_cache_buf;
                table->capacity = archived_len+EXT_LEN;
            }
            table->fp = fopen(file_name, "a+b");
            table->len = archived_len;
        }
        table->file_name = strdup(file_name);
    }
    return table->len;
}

int load_like(Like_t *like, char *file_name) {
    size_t archived_len;
    struct stat st;
    if (like->fp != NULL) {
        fclose(like->fp);
        free(like->file_name);
        like->fp = NULL;
        like->file_name = NULL;
    }
    if (file_name != NULL) {
        like->len = 0;
        memset(like->cache_map, 0, sizeof(char)*INIT_TABLE_SIZE);
        if (stat(file_name, &st) != 0) {
            //Create new file
            like->fp = fopen(file_name, "wb");
        } else {
            archived_len = st.st_size / sizeof(Like_User_t);
            if (archived_len > like->capacity) {
                Like_User_t *new_like_user_buf = (Like_User_t*)malloc(sizeof(Like_User_t)*(archived_len+EXT_LEN));
                unsigned char *new_like_cache_buf = (unsigned char *)malloc(sizeof(unsigned char)*(archived_len+EXT_LEN));

                memset(new_like_cache_buf, 0, sizeof(unsigned char)*(archived_len+EXT_LEN));

                free(like->like_user);
                free(like->cache_map);
                like->like_user = new_like_user_buf;
                like->cache_map = new_like_cache_buf;
                like->capacity = archived_len+EXT_LEN;
            }
            like->fp = fopen(file_name, "a+b");
            like->len = archived_len;
        }
        like->file_name = strdup(file_name);
    }
    return like->len;
}
///
/// Return the user in table by the given index
///
User_t* get_User(Table_t *table, size_t idx) {
    size_t archived_len;
    struct stat st;
    if (!table->cache_map[idx]) {
        if (idx > INIT_TABLE_SIZE) {
            goto error;
        }
        if (stat(table->file_name, &st) != 0) {
            goto error;
        }
        archived_len = st.st_size / sizeof(User_t);
        if (idx >= archived_len) {
            //neither in file, nor in memory
            goto error;
        }

        fseek(table->fp, idx*sizeof(User_t), SEEK_SET);
        fread(table->users+idx, sizeof(User_t), 1, table->fp);
        table->cache_map[idx] = 1;
    }
    return table->users+idx;

error:
    return NULL;
}

Like_User_t* get_Like_User(Like_t *like, size_t idx) {
    size_t archived_len;
    struct stat st;
    if (!like->cache_map[idx]) {
        if (idx > INIT_TABLE_SIZE) {
            goto error;
        }
        if (stat(like->file_name, &st) != 0) {
            goto error;
        }
        archived_len = st.st_size / sizeof(Like_User_t);
        if (idx >= archived_len) {
            //neither in file, nor in memory
            goto error;
        }

        fseek(like->fp, idx*sizeof(Like_User_t), SEEK_SET);
        fread(like->like_user+idx, sizeof(Like_t), 1, like->fp);
        like->cache_map[idx] = 1;
    }
    return like->like_user+idx;

error:
    return NULL;
}
