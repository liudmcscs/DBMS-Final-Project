#include <string.h>

#include <stdlib.h>

#include "Command.h"

#include "SelectState.h"



void field_state_handler(Command_t *cmd, size_t arg_idx) {

    cmd->cmd_args.sel_args.fields = NULL;

    cmd->cmd_args.sel_args.fields_len = 0;

    cmd->cmd_args.sel_args.limit = -1;

    cmd->cmd_args.sel_args.offset = -1;
			cmd->select_flag = 0;
			cmd->join_flag = 0;
			cmd->limit_flag = 0;
			cmd->offset_flag = 0;
	size_t iii=0;
	while(iii < cmd->args_len)
	{
		if (!strncmp(cmd->args[iii], "join", 4)) 
		{
			cmd->select_flag = 1;
			cmd->join_flag = 1;
		}
		if(cmd->join_flag == 1)
		{
			if(!strncmp(cmd->args[iii], "limit", 5))
			{
				cmd->limit_flag=1;
				cmd->limit_num =  atoi(cmd->args[iii+1]);
			}
			else if(!strncmp(cmd->args[iii], "offset", 6))
			{					
				cmd->offset_flag = 1;
				cmd->offset_num =  atoi(cmd->args[iii+1]);
			}
		}

	
		iii += 1;
	}
	
    while(arg_idx < cmd->args_len) {

        if (!strncmp(cmd->args[arg_idx], "*", 1)) {

            add_select_field(cmd, cmd->args[arg_idx]);

        } else if (!strncmp(cmd->args[arg_idx], "id", 2)) {

            add_select_field(cmd, cmd->args[arg_idx]);

        } else if (!strncmp(cmd->args[arg_idx], "name", 4)) {

            add_select_field(cmd, cmd->args[arg_idx]);

        } else if (!strncmp(cmd->args[arg_idx], "email", 5)) {

            add_select_field(cmd, cmd->args[arg_idx]);

        } else if (!strncmp(cmd->args[arg_idx], "age", 3)) {

            add_select_field(cmd, cmd->args[arg_idx]);

        }else if (!strncmp(cmd->args[arg_idx], "avg", 3)) {

            cmd->select_flag = 1;

        }else if (!strncmp(cmd->args[arg_idx], "count", 5)) {

            cmd->select_flag = 1;

        }else if (!strncmp(cmd->args[arg_idx], "sum", 3)) {

            cmd->select_flag = 1;

        } else if (!strncmp(cmd->args[arg_idx], "from", 4)) {

            table_state_handler(cmd, arg_idx+1);

            return;

        } else {

            cmd->type = UNRECOG_CMD;

            return;

        }

        arg_idx += 1;

    }

    cmd->type = UNRECOG_CMD;

    return;

}



void table_state_handler(Command_t *cmd, size_t arg_idx) 
{
    if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "user", 4)) 
	{
		cmd->select_flag = 1;
        for(size_t i = arg_idx; i < cmd->args_len;i++)
		{
			if (i == cmd->args_len) 
			{
				return;
			} 
			else if (!strncmp(cmd->args[i], "offset", 6)) 
			{
				offset_state_handler(cmd, i+1);
				return;
			} 
			else if (!strncmp(cmd->args[i], "limit", 5)) 
			{
				limit_state_handler(cmd, i+1);
				return;
			}
		}
    }
	
	else if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "like", 4)) 
	{
		cmd->select_flag = 2;
        for(size_t i = arg_idx; i < cmd->args_len;i++)
		{
			if (i == cmd->args_len) 
			{
				return;
			} 
			else if (!strncmp(cmd->args[i], "offset", 6)) 
			{
				offset_state_handler(cmd, i+1);
				return;
			} 
			else if (!strncmp(cmd->args[i], "limit", 5)) 
			{
				limit_state_handler(cmd, i+1);
				return;
			}
		}
    }
    cmd->type = UNRECOG_CMD;
    return;
}



void offset_state_handler(Command_t *cmd, size_t arg_idx) {

    if (arg_idx < cmd->args_len) {

        cmd->cmd_args.sel_args.offset = atoi(cmd->args[arg_idx]);



        for(size_t i = arg_idx; i < cmd->args_len;i++){

        if (i == cmd->args_len) {

            return;

        } else if (i < cmd->args_len

                && !strncmp(cmd->args[i], "limit", 5)) {



            limit_state_handler(cmd, i+1);

            return;

        }

	}

    }

    cmd->type = UNRECOG_CMD;

    return;

}



void limit_state_handler(Command_t *cmd, size_t arg_idx) {

    if (arg_idx < cmd->args_len) {

        cmd->cmd_args.sel_args.limit = atoi(cmd->args[arg_idx]);



        arg_idx++;



        if (arg_idx == cmd->args_len) {

            return;

        }

    }

    cmd->type = UNRECOG_CMD;

    return;

}







