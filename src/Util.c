#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Util.h"
#include "Command.h"
#include "Table.h"
#include "SelectState.h"

////
///
/// Allocate State_t and initialize some attributes
/// Return: ptr of new State_t
///
State_t* new_State() {
    State_t *state = (State_t*)malloc(sizeof(State_t));
    state->saved_stdout = -1;
    return state;
}

///
/// Print shell prompt
///
void print_prompt(State_t *state) {
    if (state->saved_stdout == -1) {
        printf("db > ");
    }
}

int where(User_t *user, Command_t *cmd) {
    size_t idx;
    size_t i;
    size_t j;
    //int offset = cmd->cmd_args.sel_args.offset;
    int count = 0;
    int andnum =0;
    int ornum=0;
    int flag=0;

    for(idx = 0; idx < cmd->args_len; idx++)
    {
        if(!strncmp(cmd->args[idx], "or", 2))
                ornum = 1;
        else if(!strncmp(cmd->args[idx], "and", 3))
                andnum++;
    }


        count =0;
            for(j = 0 ; j < cmd->args_len; j++)
            {
//              printf("j = %d, len = %d\n",j,cmd->args_len);    
//              printf("%s ",cmd->args[j] );
                if(!strncmp(cmd->args[j],"where",5))
                {
                    flag=1;
//                  printf("IN");           
                    for( i = j+1; i < cmd->args_len;i++)
                    {

                        if(!strncmp(cmd->args[i],"id", 2))
                        {
                                if(!strcmp(cmd->args[i+1],"="))
                                {
                                    if(atoi(cmd->args[i+2])==user->id)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"!="))
                                {
                                    if(atoi(cmd->args[i+2])!=user->id)
                                        count++;
				}
                                if(!strcmp(cmd->args[i+1],">"))
                                {
                                    if(atoi(cmd->args[i+2])<user->id)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<"))
                                {
                                    if(atoi(cmd->args[i+2])>user->id)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],">="))
                                {
                                    if(atoi(cmd->args[i+2])<=user->id)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<="))
                                {
                                    if(atoi(cmd->args[i+2])>=user->id)
                                        count++;

                                }
                        }
                        if(!strncmp(cmd->args[i],"name", 4))
                        {
                                char s1[100];
                                
                                strcpy(s1,user->name);

//                              printf("s1 = %s\n",s1);

//                              printf("com = %s\n",cmd->args[i+2]);          
                                if(!strcmp(cmd->args[i+1],"="))
                                {
                                        if(!strcmp(cmd->args[i+2],s1))
                                         count++;

                                }
                                if(!strcmp(cmd->args[i+1],"!="))
								{
                                        if(strcmp(cmd->args[i+2],s1))
                                         count++;

                                }
                        }
                        if(!strncmp(cmd->args[i],"email", 5))
                        {
                               char s1[100]/*,s2[100]*/;
                               strcpy(s1,user->email);
    

                                if(!strcmp(cmd->args[i+1],"="))
                                {
                                        if(!strcmp(cmd->args[i+2],s1))
                                         count++;

                                }
                                if(!strcmp(cmd->args[i+1],"!="))
                                {
                                        if(strcmp(cmd->args[i+2],s1))
                                         count++;

                                }

                        }
                        if(!strncmp(cmd->args[i],"age", 3))
                        {
                                if(!strcmp(cmd->args[i+1],"="))
                                {
                                    if(atoi(cmd->args[i+2])==user->age)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"!="))
                                {
                                    if(atoi(cmd->args[i+2])!=user->age)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],">"))
                                {
                                    if(atoi(cmd->args[i+2])<user->age)
					count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<"))
                                {
                                    if(atoi(cmd->args[i+2])>user->age)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],">="))
                                {
                                    if(atoi(cmd->args[i+2])<=user->age)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<="))
                                {
                                    if(atoi(cmd->args[i+2])>=user->age)
                                        count++;

                                }

                        }
                    }
				break;
                }
                
               
				
            }
//          printf("count = %d and = %d or = %d\n",count,andnum,ornum); 
            if( ((count == andnum+1 && ornum ==0 ) || (count >= ornum && andnum==0 )) && (andnum+ornum !=0)  )
	    {    
	//	    printf("where many");
		    return 1;
	    }
            else if (count==1 && andnum+ornum==0)
	    {
	//	    printf("where one");
		    return 1;
	    }
            else if (flag==0)
	    {
	//	    printf("nowhere");
		    return 1;
	    }
            else
	    {
	//	printf("return0");    
            	return 0;
	    }
}

int where_like(Like_User_t *like_user, Command_t *cmd) {
    size_t idx;
    size_t i;
    size_t j;
    int count = 0;
    int andnum =0;
    int ornum=0;
    int flag=0;

    for(idx = 0; idx < cmd->args_len; idx++)
    {
        if(!strncmp(cmd->args[idx], "or", 2))
                ornum = 1;
        else if(!strncmp(cmd->args[idx], "and", 3))
                andnum++;
    }


        count =0;
            for(j = 0 ; j < cmd->args_len; j++)
            {
//              printf("j = %d, len = %d\n",j,cmd->args_len);    
//              printf("%s ",cmd->args[j] );
                if(!strncmp(cmd->args[j],"where",5))
                {
                    flag=1;
//                  printf("IN");           
                    for( i = j+1; i < cmd->args_len;i++)
                    {

                        if(!strncmp(cmd->args[i],"id1", 3))
                        {
                                if(!strcmp(cmd->args[i+1],"="))
                                {
                                    if(atoi(cmd->args[i+2])==like_user->id1)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"!="))
                                {
                                    if(atoi(cmd->args[i+2])!=like_user->id1)
                                        count++;
				}
                                if(!strcmp(cmd->args[i+1],">"))
                                {
                                    if(atoi(cmd->args[i+2])<like_user->id1)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<"))
                                {
                                    if(atoi(cmd->args[i+2])>like_user->id1)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],">="))
                                {
                                    if(atoi(cmd->args[i+2])<=like_user->id1)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<="))
                                {
                                    if(atoi(cmd->args[i+2])>=like_user->id1)
                                        count++;

                                }
                        }
                       
                       
                        if(!strncmp(cmd->args[i],"id2", 3))
                        {
                                if(!strcmp(cmd->args[i+1],"="))
                                {
                                    if(atoi(cmd->args[i+2])==like_user->id2)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"!="))
                                {
                                    if(atoi(cmd->args[i+2])!=like_user->id2)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],">"))
                                {
                                    if(atoi(cmd->args[i+2])<like_user->id2)
					count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<"))
                                {
                                    if(atoi(cmd->args[i+2])>like_user->id2)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],">="))
                                {
                                    if(atoi(cmd->args[i+2])<=like_user->id2)
                                        count++;

                                }
                                if(!strcmp(cmd->args[i+1],"<="))
                                {
                                    if(atoi(cmd->args[i+2])>=like_user->id2)
                                        count++;

                                }

                        }
                    }
				break;
                }
                
               
				
            }
//          printf("count = %d and = %d or = %d\n",count,andnum,ornum); 
            if( ((count == andnum+1 && ornum ==0 ) || (count >= ornum && andnum==0 )) && (andnum+ornum !=0)  )
	    {    
	//	    printf("where many");
		    return 1;
	    }
            else if (count==1 && andnum+ornum==0)
	    {
	//	    printf("where one");
		    return 1;
	    }
            else if (flag==0)
	    {
	//	    printf("nowhere");
		    return 1;
	    }
            else
	    {
	//	printf("return0");    
            	return 0;
	    }
}


int print_user(User_t *user,  Like_User_t *like_user, SelectArgs_t *sel_args, Command_t *cmd, int offset) 
{
	size_t idx;
	int count = 0;
	if(cmd->select_flag==1)
	{
		/*size_t idx;
		int count = 0;*/
		for (idx = 0; idx < sel_args->fields_len; idx++)
		{
			if (!strncmp(sel_args->fields[idx], "*", 1))
			{
				if(where(user ,cmd)){
					if(offset !=0)
					{
						return 2;
					}
					else
					{
						printf("(%d, %s, %s, %d)\n", user->id, user->name, user->email, user->age);
					count =1;
					}
				}
			
			}
		else
		{
				if(where(user, cmd))
				{
					if(offset !=0)
					{
						return 2;
					}
					else{
						if( idx == 0)
						printf("(");
						if (idx > 0)
						printf(", ");

						if (!strncmp(sel_args->fields[idx], "id", 2)) {
							printf("%d", user->id);
						} else if (!strncmp(sel_args->fields[idx], "name", 4)) {
							printf("%s", user->name);
						} else if (!strncmp(sel_args->fields[idx], "email", 5)) {
							printf("%s", user->email);
						} else if (!strncmp(sel_args->fields[idx], "age", 3)) {
							printf("%d", user->age);
						}
						if(idx == sel_args->fields_len-1)
							printf(")\n");
						count=1;
					}
				}
			}
		}
		/*if(count !=0)
			return 1;
		else
			return 0;*/
	}
    else if(cmd->select_flag==2)
	{
		/*size_t idx;
		int count = 0;*/
		for (idx = 0; idx < sel_args->fields_len; idx++)
		{
			if (!strncmp(sel_args->fields[idx], "*", 1))
			{
		//	printf("inthe print");
				if(where_like(like_user ,cmd))
				{
					printf("(%d, %d)\n", like_user->id1, like_user->id2);
					count =1;
				}
			}
		else
		{
				if(where_like(like_user, cmd))
					{
				if( idx == 0)
								printf("(");
						if (idx > 0)
					printf(", ");

					if (!strncmp(sel_args->fields[idx], "id1", 3)) {
						printf("%d", like_user->id1);
					}  else if (!strncmp(sel_args->fields[idx], "id2", 3)) {
						printf("%d", like_user->id2);
					}
					if(idx == sel_args->fields_len-1)
							printf(")\n");
			count=1;
				}
			}
		}
		/*if(count !=0)
			return 1;
		else
			return 0;*/
	}
	if(count !=0)
			return 1;
		else
			return 0;
}


void swap(Like_User_t  *a, Like_User_t *b){
    Like_User_t temp;
    temp.id1 = a->id1;
    temp.id2 = a->id2;
    
    a->id1 = b->id1;
    a->id2 = b->id2;
    
    b->id1 = temp.id1;
    b->id2 = temp.id2;
}

int Partition(Like_User_t* like_user, int front, int end){
    int pivot = ((like_user)+end)->id1;
    int i = front -1;
    for (int j = front; j < end; j++) {
        if ((like_user+j)->id1 < pivot) {
            i++;
            swap(like_user+i, like_user+j);
        }
    }
    i++;
    swap(like_user+i, like_user+end);
    return i;
}

int Partition2(Like_User_t* like_user, int front, int end){
    int pivot = ((like_user)+end)->id2;
    int i = front -1;
    for (int j = front; j < end; j++) {
        if ((like_user+j)->id2 < pivot) {
            i++;
            swap(like_user+i, like_user+j);
        }
    }
    i++;
    swap(like_user+i, like_user+end);
    return i;
}

void quickSort(Like_User_t* like_user, int front, int end){
    if (front < end) {
        int pivot = Partition(like_user, front, end);
        quickSort(like_user, front, pivot - 1);
        quickSort(like_user, pivot + 1, end);
    }
}

void quickSort2(Like_User_t* like_user, int front, int end){
    if (front < end) {
        int pivot = Partition2(like_user, front, end);
        quickSort2(like_user, front, pivot - 1);
        quickSort2(like_user, pivot + 1, end);
    }
}

int binarysearch(Like_User_t* like_user, int search, int n)
{
    int low = 0, high = n - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if ((like_user+mid)->id1 == search)
        {
            return mid;
        }
        else if ((like_user+mid)->id1 > search)
        {
            high = mid - 1;
        }
        else if ((like_user+mid)->id1 < search)
        {
            low = mid + 1;
        }
    }

    return -1;
}

int binarysearch2(Like_User_t* like_user, int search, int n)
{
    int low = 0, high = n - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if ((like_user+mid)->id2 == search)
        {
            return mid;
        }
        else if ((like_user+mid)->id2 > search)
        {
            high = mid - 1;
        }
        else if ((like_user+mid)->id2 < search)
        {
            low = mid + 1;
        }
    }

    return -1;
}



void print_join(Table_t *table, Like_t *like,/*  User_t *user,Like_User_t *like_user,*/ SelectArgs_t *sel_args, Command_t *cmd)
{
	Like_t *copy = (Like_t*)malloc(sizeof(Like_t));
	copy->like_user = (Like_User_t*)malloc(sizeof(Like_User_t)*like->len);
	copy->len = like->len;
	for(int x=0;x<like->len;x++){
		((copy->like_user)+x)->id1 = ((like->like_user)+x)->id1;
		((copy->like_user)+x)->id2 = ((like->like_user)+x)->id2;
	}
	
	size_t idx,idx1;
	User_t *user;
	//Like_User_t *Like_user;
	size_t count=0;  
	if(!strncmp(cmd->args[9],"id1",3))
	{//
		quickSort(copy->like_user,0,copy->len-1);
		for (idx = 0; idx < table->len; idx++) 
		{
			user = get_User(table, idx);
			
			if(where(user ,cmd))
			{
				size_t index = -1;
				index = binarysearch(copy->like_user, user->id, copy->len);
				if(index==-1){
					continue;
				}
			 	while(1){
			 		if(index==0){
			 			break;
					}
			 		if(((copy->like_user)+index)->id1==((copy->like_user)+index-1)->id1){
						index--;
					}
					else{
						break;
					}
				}
				
				for (idx1 = index; idx1 < copy->len ; idx1++) 
				{
					if(((copy->like_user)+idx1)->id1==user->id)
					{
						count++;
						//break;
					}
					else{
						break;
					}
				}  
				
			}			
		}
	}
	else if(!strncmp(cmd->args[9],"id2",3))
	{//printf("id2");
		quickSort2(copy->like_user,0,copy->len-1);
		for (idx = 0; idx < table->len; idx++) 
		{
			user = get_User(table, idx);
			
			if(where(user ,cmd))
			{
				size_t index = -1;
				index = binarysearch2(copy->like_user, user->id, copy->len);
				if(index==-1){
					continue;
				}
				while(1){
			 		if(index==0){
			 			break;
					}
			 		if(((copy->like_user)+index)->id2==((copy->like_user)+index-1)->id2){
						index--;
					}
					else{
						break;
					}
				}
				for (idx1 = index; idx1 < copy->len ; idx1++) 
				{
					if(((copy->like_user)+idx1)->id2==user->id)
					{
						count++;
						//break;
					}
					else{
						break;
					}
				}  
				
			}			
		}
	}
	///////////**************///////////////9999999999999     508-565
	
		//printf("compar_offset_flag: %ld\n",cmd->offset_flag);
	size_t compar_limit_num=cmd->limit_num, compar_offset_num=cmd->offset_num;
		//printf("compar_offset_num: %ld\n",compar_offset_num);
		//printf("count: %ld\n",count);
	if(cmd->limit_flag==0&&cmd->offset_flag==0)
	{
		printf("(%ld)\n",count);
	}
	else if(cmd->limit_flag==1&&cmd->offset_flag==0)
	{
		/*printf("hererer 1 0\n");
		printf("compar_limit_num: %ld\n",compar_limit_num);
		printf("count: %ld\n",count);*/
		if(compar_limit_num<1)
		{
			
		}
		else if(compar_limit_num>=1)
		{
			printf("(%ld)\n",count);
		}
	}
	else if(cmd->limit_flag==0&&cmd->offset_flag==1)
	{
		/*printf("hererer 0 1\n");
		printf("compar_offset_num: %ld\n",compar_offset_num);
		printf("count: %ld\n",count);*/
		/*if((count-compar_offset_num)<=0)printf("(0)\n");
		else */
		if(compar_offset_num>0)
		{
			
		}
		else if(compar_offset_num==0)printf("(%ld)\n",count);		
	}
	else if(cmd->limit_flag==1&&cmd->offset_flag==1)
	{
		if(compar_offset_num==0)
		{
			if(compar_limit_num<1)
			{
				
			}
			else if(compar_limit_num>=1)
			{
				printf("(%ld)\n",count);
			}
		}
		else if(compar_offset_num!=0)
		{
			
		}
		//printf("(%ld)\n",count);
	}
	//return 1;
	///////////**************///////////////9999999999999   508 565
}


void aggregation(Table_t *table, Like_t *like, Command_t *cmd, int offset){
	size_t idx;
	int aggre = 0;
	int idfun = 0;
	int namefun = 0;
	int emailfun = 0;
	int avgcount = 0;
	int agefun = 0;
	float idavg = 0;
	float ageavg = 0;
	
	for(int i =0;i < cmd->args_len;i++)
    	{
        	if((!strncmp(cmd->args[i],"avg",3)) || (!strncmp(cmd->args[i],"count",5)) || (!strncmp(cmd->args[i],"sum",3)) )
            		aggre++;
    	}
   	 
    printf("(");
	int j = aggre;
    for(int i =0;i < cmd->args_len;i++)
    {
        if(!strncmp(cmd->args[i],"avg",3))
        {	
		idfun = 0;
         namefun = 0;
         emailfun = 0;
         avgcount = 0;
         agefun = 0;
         idavg = 0;
         ageavg = 0;
//		printf("IN AVG\n");
	    for(idx= 0;idx < table->len ; idx++)
        { 	
            	if(where(get_User(table, idx),cmd))
            	{
            		if(offset !=0)
					{
						break;
					}
					else
					{
//						printf("IN where\n");
//            			printf("%s",(cmd->args[i]));
						if(!strcmp(cmd->args[i],"avg(id)"))
            			idfun = idfun + (table->users+idx)->id;
			
            			else if(!strcmp(cmd->args[i],"avg(age)"))
            			{
//							printf("IN AGE\n");
            				agefun = agefun + (table->users+idx)->age;
//							printf("??");
						}
//						printf("if");	
						avgcount++;
					}
				}
//		printf("table\n");
	    }
//            printf("end");
            idavg = (float)idfun/(float)avgcount;
            ageavg = (float)agefun/(float)avgcount;
            		
           if(idfun!=0)
            	printf("%.3f",idavg);
           if(agefun!=0)
            	printf("%.3f",ageavg);
            			
           j--;
           if(j!=0)
            	printf(", ");
            	
        }
            	
        if(!strncmp(cmd->args[i],"count",5))
        {
	    idfun = 0;
         namefun = 0;
         emailfun = 0;
         avgcount = 0;
         agefun = 0;
         idavg = 0;
         ageavg = 0;

	    for(idx= 0;idx < table->len ; idx++)
            { 
            	if(where(get_User(table, idx), cmd))
       			{
       				if(offset !=0)
					{
						break;
					}
					else
					{
						if(!strcmp(cmd->args[i],"count(id)"))
            				idfun++;
            			else if(!strcmp(cmd->args[i],"count(name)"))
            				namefun++;
            			else if(!strcmp(cmd->args[i],"count(email)"))
            				emailfun++;
            			else if(!strcmp(cmd->args[i],"count(age)"))
            				agefun++;
						else if (!strcmp(cmd->args[i],"count(*)"))
						avgcount++;
					}
       			
				}
            }
            		
            if(idfun!=0)printf("%d",idfun);
            if(namefun!=0)printf("%d",namefun);
            if(emailfun!=0)printf("%d",emailfun);
            if(agefun!=0)printf("%d",agefun);
	    	if(avgcount!=0)printf("%d",avgcount);
            			
            j--;
            if(j!=0)
            	printf(", ");
        }
        
        if(!strncmp(cmd->args[i],"sum",3))
	{
		idfun = 0;
         namefun = 0;
         emailfun = 0;
         avgcount = 0;
         agefun = 0;
         idavg = 0;
         ageavg = 0;

	    for(idx= 0;idx < table->len ; idx++)
	    {
	        if(where(get_User(table, idx),cmd))
            	{
            		if(offset !=0)
					{
						break;
					}
					else
					{
						if(!strcmp(cmd->args[i],"sum(id)"))
            			idfun = idfun + (table->users+idx)->id;
            		else if(!strcmp(cmd->args[i],"sum(age)"))
            			agefun = agefun + (table->users+idx)->age;
					}
            		
			}
	    }
            if(idfun!=0)
            	printf("%d",idfun);
            if(agefun!=0)
            	printf("%d",agefun);
            			
            j--;
            if(j!=0)
            	printf(", ");
        }
    }
				
    printf(")\n");
}

void aggregation_like(Table_t *table, Like_t *like, Command_t *cmd){
	size_t idx;
	int aggre = 0;
	int id1fun = 0;
	int id2fun = 0;
	int avgcount = 0;
	float id1avg = 0;
	float id2avg = 0;
	
	for(int i =0;i < cmd->args_len;i++)
    	{
        	if((!strncmp(cmd->args[i],"avg",3)) || (!strncmp(cmd->args[i],"count",5)) || (!strncmp(cmd->args[i],"sum",3)) )
            		aggre++;
    	}
    
    printf("(");
	int j = aggre;
    for(int i =0;i < cmd->args_len;i++)
    {
        if(!strncmp(cmd->args[i],"avg",3))
        {	
		id1fun = 0;
         id2fun = 0;
         avgcount = 0;
         id1avg = 0;
         id2avg = 0;
//		printf("IN AVG\n");
	    for(idx= 0;idx < like->len ; idx++)
            { 	
            	if(where_like(get_Like_User(like, idx),cmd))
            	{
//			printf("IN where\n");
//            		printf("%s",(cmd->args[i]));
			if(!strcmp(cmd->args[i],"avg(id1)"))
            			id1fun = id1fun + (like->like_user+idx)->id1;
			
            		else if(!strcmp(cmd->args[i],"avg(id2)"))
            		{
//				printf("IN AGE\n");
            			id2fun = id2fun + (like->like_user+idx)->id2;
//				printf("??");
			}
//			printf("if");	
			avgcount++;
		}
//		printf("table\n");
	    }
//            printf("end");
            id1avg = (float)id1fun/(float)avgcount;
            id2avg = (float)id2fun/(float)avgcount;
            		
           if(id1fun!=0)
            	printf("%.3f",id1avg);
           if(id2fun!=0)
            	printf("%.3f",id2avg);
            			
           j--;
           if(j!=0)
            	printf(", ");
            	
        }
            	
        if(!strncmp(cmd->args[i],"count",5))
        {
		id1fun = 0;
         id2fun = 0;
         avgcount = 0;
         id1avg = 0;
         id2avg = 0;

	    for(idx= 0;idx < like->len ; idx++)
            { 
            	if(where_like(get_Like_User(like, idx), cmd))
       		{
       			if(!strcmp(cmd->args[i],"count(id1)"))
            			id1fun++;
            		else if(!strcmp(cmd->args[i],"count(id2)"))
            			id2fun++;
			else if (!strcmp(cmd->args[i],"count(*)"))
				avgcount++;
		}
            }
            		
            if(id1fun!=0)
            	printf("%d",id1fun);
            if(id2fun!=0)
            	printf("%d",id2fun);
	    if(avgcount!=0)
		printf("%d",avgcount);
            			
            j--;
            if(j!=0)
            	printf(", ");
        }
        if(!strncmp(cmd->args[i],"sum",3))
        {
			id1fun = 0;
         id2fun = 0;
         avgcount = 0;
         id1avg = 0;
         id2avg = 0;

	    for(idx= 0;idx < like->len ; idx++)
	    {
	        if(where_like(get_Like_User(like, idx),cmd))
            	{
            		if(!strcmp(cmd->args[i],"sum(id1)"))
            			id1fun = id1fun + (like->like_user+idx)->id1;
            		else if(!strcmp(cmd->args[i],"sum(id2)"))
            			id2fun = id2fun + (like->like_user+idx)->id2;
		}
	    }
            if(id1fun!=0)
            	printf("%d",id1fun);
            if(id2fun!=0)
            	printf("%d",id2fun);
            			
            j--;
            if(j!=0)
            	printf(", ");
        }
    }
				
    printf(")\n");
}
///
/// Print the users for given offset and limit restriction
///
void print_users(Table_t *table, Like_t *like, int *idxList, size_t idxListLen, Command_t *cmd) {
    size_t idx;
    int limit = cmd->cmd_args.sel_args.limit;
    int offset = cmd->cmd_args.sel_args.offset;
    int aggre = 0;
    int count=0;    

    if (offset == -1) {
        offset = 0;
    }

	
    for(int i =0;i < cmd->args_len;i++)
    {
        if((!strncmp(cmd->args[i],"avg",3)) || (!strncmp(cmd->args[i],"count",5)) || (!strncmp(cmd->args[i],"sum",3)) )
            aggre++;
    }
    
	if (idxList) {
    	for (idx = 0; idx < idxListLen; idx++) {
        	if (limit != -1 && count >= limit) {
               		break;
           	}
//		printf("idx=%d limit=%d\n",idx,limit);
//		printf("11\n");
           	if(aggre ==0){
	//		printf("21 \n");

			if(print_user(get_User(table, idxList[idx]),NULL, &(cmd->cmd_args.sel_args),cmd,offset)==1)
			{		count++;

			}	
           		else if(print_user(get_User(table, idxList[idx]),NULL, &(cmd->cmd_args.sel_args),cmd,offset)==2)
           		{

   //        			printf("OFFSET1 %d \n",offset);
           			offset--;
  //         			printf("OFFSET2 %d \n",offset);
				   }
           			
	           
			}
			else if (aggre != 0)
			{
				if(offset==0)
        				aggregation(table,NULL, cmd,offset);
				break;
			}
		}
    } 
    
	else 
	{
		if(cmd->select_flag==1)
		{
			if(cmd->join_flag==0)
			{
				for (idx = 0; idx < table->len; idx++) 
				{
					if (limit != -1 && count >= limit) 
					{
							break;
					}
//					printf("12 \n");
							if(aggre ==0){
//						printf("2 2\n");
							 if(print_user(get_User(table, idx),NULL, &(cmd->cmd_args.sel_args),cmd,offset)==1)
						{
								
								count++;
						}
					else if(print_user(get_User(table,idx),NULL, &(cmd->cmd_args.sel_args),cmd,offset)==2)
           						{
  //         			printf("OFFSET1 %d \n",offset);
           			offset--;
//           			printf("OFFSET2 %d \n",offset);
				   }
					}
						else if (aggre != 0)
					{
						if(offset==0)
							aggregation(table,NULL, cmd,offset);
						break;
					}
				}
			}				
			else if(cmd->join_flag==1)
			{//printf("hhhhhhhhhhhhhhh");
				idx=0;
				/*while(idx< table->len)
				{*/
					print_join(table,like,/*get_User(table, idx),get_Like_User(like, idx),*/ &(cmd->cmd_args.sel_args),cmd);
				/*	idx++;
				}*/
				
			}	
		}
        else if(cmd->select_flag==2)
		{
			for (idx = offset; idx < like->len; idx++) 
			{
				if (limit != -1 && count >= limit) 
				{
						break;
				}
		//		printf("idx=%d limit=%d\n",idx,limit);
						if(aggre ==0){
						if(print_user(NULL,get_Like_User(like, idx), &(cmd->cmd_args.sel_args),cmd,offset))
						count++;
				}
					else if (aggre != 0)
				{
						aggregation_like(NULL,like, cmd);
					break;
				}
			}
		}
    }
}

///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " ,\n");
    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " ,\n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
void handle_builtin_cmd(Table_t *table, Command_t *cmd, State_t *state) {
    if (!strncmp(cmd->args[0], ".exit", 5)) {
        archive_table(table);
        exit(0);
    } else if (!strncmp(cmd->args[0], ".output", 7)) {
        if (cmd->args_len == 2) {
            if (!strncmp(cmd->args[1], "stdout", 6)) {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1], 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
                __fpurge(stdout); //This is used to clear the stdout buffer
            }
        }
    } else if (!strncmp(cmd->args[0], ".load", 5)) {
        if (cmd->args_len == 2) {
            load_table(table, cmd->args[1]);
        }
    } else if (!strncmp(cmd->args[0], ".help", 5)) {
        print_help_msg();
    }
}

///
/// Handle query type commands
/// Return: command type
///
int handle_query_cmd(Table_t *table, Like_t *like, Command_t *cmd) {
    if (!strncmp(cmd->args[0], "insert", 6)) {
        handle_insert_cmd(table, like, cmd);
        return INSERT_CMD;
    } else if (!strncmp(cmd->args[0], "select", 6)) {
        handle_select_cmd(table, like, cmd);
        return SELECT_CMD;
    }
	else if(!strncmp(cmd->args[0], "update", 6)){
		handle_update_cmd(table, cmd);
		return UPDATE_CMD;
	}
	else if(!strncmp(cmd->args[0], "delete", 6)){
		handle_delete_cmd(table, cmd);
		return DELETE_CMD;
	}
	else {
        return UNRECOG_CMD;
    }
}

void handle_delete_cmd(Table_t *table, Command_t *cmd) {
	int idx;
    for(idx= 0;idx<table->len;idx++)
    {
    	if(where(get_User(table, idx),cmd))
    	{
    		for(int i = idx ; i<table->len;i++)
    		{
    			(table->users+i)->id = (table->users+i+1)->id;
    			strcpy((table->users+i)->name, (table->users+i+1)->name);
    			strcpy((table->users+i)->email, (table->users+i+1)->email);
    			(table->users+i)->age = (table->users+i+1)->age;
			}
			idx--;
			table->len--;
		}
	}

}

void handle_update_cmd(Table_t *table, Command_t *cmd) {
    size_t idx;
    int flag=0;
    for(int i = 0 ; i<cmd->args_len ; i++)
    {
		if(!strncmp(cmd->args[i],"set",3))
    	{
    		for(idx = 0; idx< table->len ; idx++)
    		{
    			if(where(get_User(table, idx),cmd))
    			{
    				if(!strncmp(cmd->args[i+1],"id",2))
    				{
    					for(int search = 0; search< table->len ; search++)
    					{
    						if((table->users+search)->id==atoi(cmd->args[i+3]))
    						{
    							flag=1;
							}
    					}
    					if(flag==0)
    						(table->users+idx)->id = atoi(cmd->args[i+3]);
					}
    				if(!strncmp(cmd->args[i+1],"name",4))
    				{
    					strcpy((table->users+idx)->name , cmd->args[i+3]);
					}
    				if(!strncmp(cmd->args[i+1],"email",5))
    				{
    					strcpy((table->users+idx)->email , cmd->args[i+3]);
					}
    				if(!strncmp(cmd->args[i+1],"age",3))
    				{
    					(table->users+idx)->age = atoi(cmd->args[i+3]);
					}
				}
			}
		}
	}
}

///
/// The return value is the number of rows insert into table
/// If the insert operation success, then change the input arg
/// `cmd->type` to INSERT_CMD
///
int handle_insert_cmd(Table_t *table, Like_t *like, Command_t *cmd) {
    int ret = 0;
    User_t *user = command_to_User(cmd);
	Like_User_t *like_user = command_to_Like_User(cmd);
    if (user) {
        ret = add_User(table, user);
        if (ret > 0) {
            cmd->type = INSERT_CMD;
        }
    }
	if (like_user) {
        ret = add_Like_User(like, like_user);
        if (ret > 0) {
            cmd->type = INSERT_CMD;
        }
    }
    return ret;
}

///
/// The return value is the number of rows select from table
/// If the select operation success, then change the input arg
/// `cmd->type` to SELECT_CMD
///
int handle_select_cmd(Table_t *table, Like_t *like, Command_t *cmd) {
    cmd->type = SELECT_CMD;
    field_state_handler(cmd, 1);

    print_users(table, like, NULL, 0, cmd);
    return table->len;
}

///
/// Show the help messages
///
void print_help_msg() {
    const char msg[] = "# Supported Commands\n"
    "\n"
    "## Built-in Commands\n"
    "\n"
    "  * .exit\n"
    "\tThis cmd archives the table, if the db file is specified, then exit.\n"
    "\n"
    "  * .output\n"
    "\tThis cmd change the output strategy, default is stdout.\n"
    "\n"
    "\tUsage:\n"
    "\t    .output (<file>|stdout)\n\n"
    "\tThe results will be redirected to <file> if specified, otherwise they will display to stdout.\n"
    "\n"
    "  * .load\n"
    "\tThis command loads records stored in <DB file>.\n"
    "\n"
    "\t*** Warning: This command will overwrite the records already stored in current table. ***\n"
    "\n"
    "\tUsage:\n"
    "\t    .load <DB file>\n\n"
    "\n"
    "  * .help\n"
    "\tThis cmd displays the help messages.\n"
    "\n"
    "## Query Commands\n"
    "\n"
    "  * insert\n"
    "\tThis cmd inserts one user record into table.\n"
    "\n"
    "\tUsage:\n"
    "\t    insert <id> <name> <email> <age>\n"
    "\n"
    "\t** Notice: The <name> & <email> are string without any whitespace character, and maximum length of them is 255. **\n"
    "\n"
    "  * select\n"
    "\tThis cmd will display all user records in the table.\n"
    "\n";
    printf("%s", msg);
}           

