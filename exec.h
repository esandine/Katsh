#ifndef EXEC_H
#define EXEC_H
int num_blanks(char* str);
char** parse_cmd(char* input);
int run_cmd_fork(char* input);
void run_cmd_stdout(char* input);
void run_cmd_stdin(char* input);
void run_cmd_semi(char* input);

#endif
