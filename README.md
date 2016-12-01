# Katsh
Wilson Berkow. Ely Sandine, Chris Sherling Systems Project

What features we included in our totally awesome shell:
  -Forking whenever a command is called
  -Working exit and cd commands
  -Unrestricted separation of commands with ;. Why run two commands when you can run three?
  -Redirection using >, >>, 2>, 2>>, &>, and <.
  -Unrestricted piping! There'll be so many pipes you might have to call a plumber!
  -Dedication to Mr. Kats.

What features we attempted to implement but were unsuccesful at:
  -Nothing here. Time management can really be helpful for these situations.

Any bugs or things we want you to know about our shell:
  -There aren't any bugs we know of. We used RAID. RAID 10.

Files and Function Headers:
exec.c
  int num_blanks(char* str);
  char** parse_cmd(char* input);
  int run_cmd_fork(char* input);
  void run_cmd_stdout(char* input);
  void run_cmd_stdin(char* input);
  void run_cmd_semi(char* input);
