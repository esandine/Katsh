# Katsh
Wilson Berkow. Ely Sandine, Chris Sherling Systems Project

What features we included in our totally awesome shell:
- Forking whenever a command is called
- Working exit and cd commands
- Unrestricted separation of commands with ;. Why run two commands when you can run three?
- Redirection using >, >>, 2>, 2>>, &>, and <.
- Unrestricted piping! There'll be so many pipes you might have to call a plumber!
- Dedication to Mr. Kats.

What features we attempted to implement but were unsuccesful at:
- Nothing here. Time management can really be helpful for these situations.

Any bugs or things we want you to know about our shell:
- There aren't any bugs we know of. We used RAID. RAID 10.

Files and Function Headers:

exec.c

    /*========int num_blanks()========
	  *Args: char* str
      *Return: the number of spaces and tabs in the Input
      *What it Does: counts the number of blanks in the string. This is used to
      *parse in parse_cmd
    ========*/

    /*========char** parse_cmd()========
      *Args: char* input
      *Returns: An array of strings representing the command
      *What it does: Counts the number of spaces then callocs the appropriate
      *ammount of space. It then uses strsep to split the command into a list 
      *of arguements
    ========*/

     /*========int run_cmd_fork()========
       *Args: char* input
       *Returns: 0
       *What it does: It parses the input, then checks if the first command is cd or
       *exit. If so it runs the command. If this is not the case it forks a child
       *process to run the command, and waits for the child process to run
     ========*/

    int num_blanks(char* str);
    char** parse_cmd(char* input);
    int run_cmd_fork(char* input);
    void run_cmd_stdout(char* input);
    void run_cmd_stdin(char* input);
    void run_cmd_semi(char* input);
