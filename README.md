# Katsh
Wilson Berkow. Ely Sandine, Chris Sherling Systems Project

What features we included in our totally awesome shell:
- Forking whenever a command is called
- Working exit and cd commands
- Unrestricted separation of commands with ;. Why run two commands when you can run three?
- Redirection using >, >>, 2>, 2>>, &>, and <.
- Unrestricted piping! There'll be so many pipes you might have to call a plumber!
- Execution of commands with && and || operators.
- Dedication to Mr. Kats.

What features we attempted to implement but were unsuccesful at:
- Tab completion.
- Cycling through previous commands with up and down arrow keys.

Any bugs or things we want you to know about our shell:
- There aren't any bugs we know of. We used RAID. RAID 10. (Gotta be fast and safe!)

Files and Function Headers:

  exec.c (nothing, just the sighandler and the main function)

  utils.c

    /*========int num_blanks()========
      *Args: char* str
      *Return: the number of spaces and tabs in the Input
      *What it Does: counts the number of blanks in the string. This is used to parse in parse_cmd
      ========*/
	  
    /*========int wrath_of_kats()========
      *Args: None
      *Returns: It fork bombs the user recursively
      *What it does: Forks and then calls itself. It acts as a remote force quit for all processes running. Because why not?
    ========*/
	  
    /*========char** parse_cmd()========
      *Args: char* input
      *Returns: An array of strings representing the command
      *What it does: Counts the number of spaces then callocs the appropriate ammount of space. It then uses strsep to split the command into a list of arguements.
    ========*/

  cmds.c

     /*========int run_cmd_fork()========
       *Args: char* input
       *Returns: 0
       *What it does: It parses the input, then checks if the first command is cd or exit. If so it runs the command. If this is not the case it forks a child process to run the command, and waits for the child process to run
     ========*/

	/*========void run_cmd_stdout()========
      *Args: char* input, char mode
      *Return: Void
      *What it Does: Executes a command and instead of printing to stdout, it prints to a file.
    ========*/

	/*========void run_cmd_stdin()========
      *Args: char* input
      *Return: Void
	  *What it Does: Executes a command and instead taking arguments from stdin, it takes commands from a file.
    ========*/
	  
	/*========void run_cmd_semi()========
      *Args: char* input
      *Return: Void
      *What it does: It first checks if the input is negative. If this is not the 
      *case it checks for special characters, and runs either run_cmd_stdin, run_cmd_stdout, or just the normal run_cmd_fork on the portion of the command before the first semicolon. It then recursively calls itself on the rest of the input string.
    ========*/

	/*========void run_cmd_andor()========
	  *Args: char* input
	  *Return: void
	  *What it Does: If mode = 1: Executes the first command, and if it works executes the second command If mode = 0: executes the first command and if it fails executes the second command.
    ========*/

    /*========int run_pipeline()========
	  *Args: char* cmd
	  *Return:
	  *What it Does: Runs output of one command into another command.
    ========*/


