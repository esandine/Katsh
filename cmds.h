#ifndef CMDS_H
#define CMDS_H

int run_pipeline(char * cmd);
int run_cmd_fork(char* input);
void run_cmd_stdout(char* input, int mode);
void run_cmd_stdin(char* input);
void run_cmd_semi(char* input);
void run_cmd_andor(char* input, char mode);

#endif
