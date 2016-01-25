/*
myshell.c

Bryce Paine bpaine@ucsc.edu
Taoh Green tmgreen@ucsc.edu

shell program to perform commands

*/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdlib.h>    /* exit */
#include <unistd.h>    /* _exit, fork */
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#define size 50

extern char **getline();
/*customize the symbol for the shell output*/
char* shell_line = "$ ";

/*this is a function for implementing pipes and infinite piping commands*/
int pipe_inf(char *cmd1, char *cmd2, char **args, char **args2) {
    pid_t pid;
    pid_t pid2;
    int stat;
    int i;
    int count = 0;
    pid = fork();
    /*check if fork fails*/
	if (pid == -1) {
		   perror("fork failed");
		   exit(EXIT_FAILURE);
	}
    /*fork succeeded - in child process*/
    if(pid == 0){
           /*make pipe*/
            int p[2];
            if (pipe(p)<0) {
                perror("Can't pipe\n");
                return EXIT_FAILURE;
            }
            /*if pipe is successful then fork into child process
            and manipulate file descriptors*/
            if ((pid2 = fork()) == 0) {
                     if (close(1) < 0){
						 perror("close fail");
						 exit(errno);
					 }                 
                     if(dup(p[1]) < 0){
						 perror("dup fail\n");
						 exit(errno);
					 }
                     if(close(p[0]) < 0){
						 perror("close fail\n");
						 exit(errno);
					 }
                     /*output goes into pipe, exits fork*/
                     if (execvp(cmd1, args) == -1){
						 perror("execvp failed\n");
						 return EXIT_FAILURE;
					 } 
                     _exit(EXIT_SUCCESS);
            } 
            /*back in parent*/
            else if (pid2 > 0) {
                    /*close stdin, stdin comes from pipe*/
                    if(close(0) < 0){
						perror("close fail");
						exit(errno);
					}
                    if (dup(p[0]) < 0){
						perror("dup fail\n");
						exit(errno);
					}
                    if (close(p[1]) < 0){
						perror("close fail\n");
						exit(errno);
					}
                    /*check for more pipes*/
                    for (i = 0; args2[i] != NULL; ++i) {
                        if (strcmp (args2[i], "|") == 0) {
                            cmd1 = args2[0];
                            cmd2 = args2[i+1];
                            args2[i] = NULL;
                            /*reset args to null*/
                            memset(args, 0, (int)size);
                            /*copy over portion of args following '|' */
                            while (args2[i+1] != NULL) {
                               args[count] = args2[i+1];
                               args2[i+1] = NULL;
                               i++;
                               count++;
                            }
                            /*recursively call pipe until all pipe symbols have
                            been exhausted*/
                            pipe_inf(cmd1, cmd2, args2, args);
                        }
                    }
                    /* execute last command in pipe */
                    if (execvp(cmd2, args2) == -1){
						perror("failed to execvp\n");
						return EXIT_FAILURE;
					}
            }
            else 
                    perror("fork failed\n");
    }
    else{
        /*wait for child to finish*/
		  (void)waitpid(pid, &stat, 0);
	 }
     
      return (EXIT_SUCCESS);
}

/*this is a function for implementing all commands and checks if
piping is present*/
int do_cmd(char** args){
    int i;
    int output = 0;
    int input = 0;
    int pipe_bool = 0;
    char *output_file;
    char *input_file;
    pid_t pid;
    pid_t pid2;
    int stat;
    int f1;
    int f2;
    
    /*the first cmd is the first argument*/
    char *cmd1 = args[0];
    /*piping variables*/
    
    char *cmd2;
    char *args2[(int)size];
    
    int count=0;
    /*initialize new args2 to NULL*/
    memset(args2, 0, (int)size);
    
    
	for(i = 0; args[i] != NULL; i++){
        
        /*if pipe is seen*/
		if (strcmp (args[i], "|") == 0) {
            /*cmd2 is the argument following the pipe symbol*/
            cmd2 = args[i+1];
            args[i] = NULL;
            /*copies portion of args after pipe to args2*/
            while (args[i+1] != NULL) {
               args2[count] = args[i+1];
               args[i+1] = NULL;
               i++;
               count++;
            }
            /*do pipes*/
            pipe_inf(cmd1, cmd2, args, args2);

            pipe_bool = 1;   
        }
        
        /*handles direction to output file*/
        else if(strcmp(args[i], ">") == 0){
			output = 1;
			args[i] = NULL; 
			output_file = args[i+1];
		
		}
        
        /*directs input from file*/
		else if(strcmp(args[i], "<") == 0){
			input = 1;
			args[i] = NULL; 
			input_file = args[i+1];
		}
	}
    /*because the pipe_inf function is separate there is 
    no need to repeat these cmds*/
    if(pipe_bool) {
        return (EXIT_SUCCESS);
    }
    
    pid = fork();
    
   
	if (pid == -1) {
		   perror("fork failed");
		   exit(EXIT_FAILURE);
	}
    if(pid == 0){
        /*redirect to file*/
		  if (output) {
			  f1 = open(output_file, O_TRUNC | O_WRONLY | 
                        O_CREAT, 00200);
			  if(f1<0){
				perror("could not open file\n");
				return EXIT_FAILURE;
			  }
			  if(close(1) < 0){
				  perror("close fail\n");
				  exit(errno);
			  }
			  if(dup(f1) < 0){
				perror("dup fail\n");
                exit(errno);				
			  }
			}
            /*read from file*/
		   if (input){
			   f2 = open(input_file, O_RDONLY);
			   if (f2<0){
				  perror("could not open file\n");
				  return EXIT_FAILURE;
			   }
			   if (close(0) < 0){
				   perror("close failed");
				   exit(errno);
			   }
			   if(dup(f2) < 0){
				   perror("dup fail\n");
				   exit(errno);
			   }
		   }
           
           
           if(execvp(cmd1, args) == -1){
			   perror("failed to execvp");
			   return EXIT_FAILURE;
		   } 
           
            
            
           _exit(EXIT_SUCCESS);   
      } 
	  else{
		 (void)waitpid(pid, &stat, 0);
	  } 
	  return EXIT_SUCCESS;
	  
	  
}	  


int main() {
   char **args;
   while(1) {
      printf(shell_line);
      
      args = getline();
      /*check for argument given*/
      if (args[0] == NULL) continue;
      /*exit call*/
      if (strcmp(args[0], "exit") == 0){
            exit(0);
      }
      /*implement cd command*/
       if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                chdir (getenv ("HOME"));
            }
            else {
                chdir(args[1]);
            }
       }
       else {     
            do_cmd(args);
       }
   }
}
