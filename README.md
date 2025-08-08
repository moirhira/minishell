# What are the things that should doing ?
---
<!-- 1 - need to check the exit status on the failure and display the message
2 - unset is does not work
3 - check the case of exit with many args :
exit 10 10
exit
bash: exit: too many arguments
moirhira@c1r5p1:~$
4 - also this case:
exit exit
exit: exit: numeric argument required% (the exist status is right in this case)





========================================================================================
1 - multiple -n in the heredocs (just with the space betwen them):
    echo -n -n ... "string" 

2 - in the exit with two params should return 2 not 255:
    minishell$ exit exit
    exit
    exit: exit: numeric argument required
    moirhira@c1r6p1:~/Desktop/zahia$ echo $?
    255
 -->

=========================================================================================
<!-- 1 - in the executor try to complete the execute_external function with the exit return value you
will find comment where you need to cahnge (specially in the parent process)
    // try to make it int return exit status
    exit_st = execute_external(command, env); ✅



5 - in a not found command you should display like this :
bash:
moirhira@c1r4p1:~$ ls | vdsvv | wc -l
vdsvv: command not found
0
yours:
minishell$ ls | vdsvv | wc -l
execvp failed: No such file or directory
0

6 - you should add the setup redirections also in the builtins 
in the externals is working good do that just in the biultins but you should save
now the stdin to restore it you have now the setup_redirsction use it

============================================================================1 - echo "hello" | grep "e" | wc -l
    after execute thus line all the commands not bieng its desplay just command : not found  -->
===================================================================================
<!-- 1 - also the execute_pipeline try to make it int that return the right exit status in the diffrent cases
points:

    the pipeline function should always return the last exit code of the last child process 

    add setup redirection inside it 

    the exec_command must be int and return 

    Checks for builtins before forking

    try not exit in the errors :
     pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
            // return 
        }
    
    <!-- check if there is another builtin inside that child one  -->

=========================================================

<!-- 26 jul update :

** setup_redirections inside every child 

** execute_pipeline returns the exit status of the last child that terminates, which normally corresponds to the last command in the pipeline

** exec_command is an int function that return int 

** i check for (is_builtin) first before forking 

** when an error happend , there is no exit from the prgrm ,and the child return it's exit status as well 

**  i check if there is another builtin inside the child itself  -->


=====================================================================
<!-- - function to handel biultins before fork just writed not used 
- is biultins is not working and segfault
- not properly return the exact right exist status from the child process -->

===========================================================
<!-- 
27 jul updates : 

** I fixed pwd's and cd's behavior in case of deleting the current dir;
Old behavior   =>   pwd   = No such file or directory
                    cd .. = segfault 

Fixed behavoir =>   pwd   = still points on the oath of the directory even if it's delated (that's how bash behaves)
                    cd .. = parent dir

______________________________________________________________

** I fixed the exit_status of pwd builtin;

old behav = incorrectly return exit_status(-1) (should be 0 on success, 1 on error).

fixed one = On success returns exit_status(0), which is the standard success code (like Bash), on error it returns exit_status(1), which is a failure code
 -->
 -->

___________________

==================
1- cat < into deleted file (stuck) ✅
2 -minishell$ cat << $USER
> $USER
> moirhira
must exit when you enter $USER itself ✅

3 - echo "$"USER also in the heredoc ✅


5 -minishell$ echo $"" (if the dollar didnt o thier job it must stay ) ✅

6 -minishell$ echo "$"
need to print the $ ✅

7 -< existfile ✅

8 -export invalid var 121var || @var ... ✅

9 -minishell$ $a
[1]    43891 segmentation fault (core dumped)  ./minishell ✅

==================================================

 10 - echo hello << a dfs > d ✅

 11 - echo $unsetedvar hola ✅

 12 - minishell$ echo -n $DONTEXIST hola ✅
 holaminishell$ 

 13 - run command with the  absolut path ex: /bin/ls ✅

 14 -minishell$ ls -l | < a
minishell: syntax error near unexpected token `<'
should not display syntax error ✅

15 - cd $PWD arg1 arg2 more than one arg ✅

16 - 
=====================================================

minishell$ cd $PWD | wc -l ✅
minishell: cd: command not found
0
minishell$ cd $PWD | echo hello ✅
minishell: cd: command not found
hello       ✅


minishell$ << a < b ✅
> sdvsv
> a
minishell: b: No such file or directory
minishell$ sdvsv
minishell: sdvsv: command not found
minishell$ % 


minishell$ > a | > a ✅
[1]    352064 segmentation fault (core dumped)  ./minishell


minishell$ pwd | $efdgbdb
minishell: syntax error near unexpected token `newline' ✅
minishell$ $sffsfd

=======================================================

minishell$ echo "                      $  "✅
must print it as it with spaces befor and after



minishell$ echo $""✅
$
it must be tried to expande here 


minishell$ echo $"/"✅
$/
the dolar not sholde be prited

============================================================

2 > errfile (redirecting the file descripto 0 1 2 ) to a file directly 


===================================================================
minishell$  echo <"./test_files/infile" <missing <"./test_files/infile" 
minishell$ ./test_files/infile: No such file or directory
minishell$ echo $?
1
minishell$ 
==1149195== 
==1149195== FILE DESCRIPTORS: 6 open (3 std) at exit.
==1149195== Open file descriptor 5: /dev/pts/3
==1149195==    at 0x49D9FEB: dup (syscall-template.S:120)
==1149195==    by 0x403DDA: execute_builtin (builtin_cmd.c:17)
==1149195==    by 0x405BF9: execute_commands (executor.c:111)
==1149195==    by 0x40166C: main (main.c:111)
==1149195== 
==1149195== Open file descriptor 4: /dev/pts/3
==1149195==    at 0x49D9FEB: dup (syscall-template.S:120)
==1149195==    by 0x403DDA: execute_builtin (builtin_cmd.c:17)
==1149195==    by 0x405BF9: execute_commands (executor.c:111)
==1149195==    by 0x40166C: main (main.c:111)
==1149195== 
==1149195== Open file descriptor 3: /dev/pts/3
==1149195==    at 0x49D9FEB: dup (syscall-template.S:120)
==1149195==    by 0x403DDA: execute_builtin (builtin_cmd.c:17)
==1149195==    by 0x405BF9: execute_commands (executor.c:111)
==1149195==    by 0x40166C: main (main.c:111)





sh-5.2$ valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --track-fds=yes -s ./minishell
==1953== Memcheck, a memory error detector
==1953== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==1953== Using Valgrind-3.25.1 and LibVEX; rerun with -h for copyright info
==1953== Command: ./minishell
==1953== 
minishell$ 
minishell$ << a
> ctrl + d
warning: here-document delimited by end-of-file (wanted `a')
==1966== 
==1966== FILE DESCRIPTORS: 6 open (6 inherited) at exit.
==1966== 
==1966== HEAP SUMMARY:
==1966==     in use at exit: 219,829 bytes in 233 blocks
==1966==   total heap usage: 1,003 allocs, 770 frees, 256,119 bytes allocated
==1966== LEAK SUMMARY:
==1966==    definitely lost: 0 bytes in 0 blocks
==1966==    indirectly lost: 0 bytes in 0 blocks
==1966==      possibly lost: 0 bytes in 0 blocks
==1966==    still reachable: 14,689 bytes in 10 blocks
==1966==         suppressed: 205,140 bytes in 223 blocks
==1966== 
==1966== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
--1966-- 
--1966-- used_suppression:     64 readline_general_leak readline.supp:80 suppressed: 203,596 bytes in 221 blocks
--1966-- used_suppression:      1 readline_funmap_leak readline.supp:12 suppressed: 1,536 bytes in 1 blocks
--1966-- used_suppression:      1 readline_prompt_leak readline.supp:2 suppressed: 8 bytes in 1 blocks
==1966== 
==1966== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

minishell$ << a
> ctrl + d
warning: here-document delimited by end-of-file (wanted `a')
==1967== 
==1967== FILE DESCRIPTORS: 6 open (5 inherited) at exit.
==1967== Open file descriptor 0: /dev/pts/0
==1967==    at 0x49E2C1B: dup2 (in /usr/lib/x86_64-linux-gnu/libc.so.6)
==1967==    by 0x4006122: exec_redirs_no_command (executor.c:51)
==1967==    by 0x4006281: execute_commands (executor.c:91)
==1967==    by 0x4001607: main (main.c:103)
==1967== 
==1967== 
==1967== HEAP SUMMARY:
==1967==     in use at exit: 219,870 bytes in 236 blocks
==1967==   total heap usage: 1,037 allocs, 801 frees, 256,635 bytes allocated
==1967== 
==1967== 
==1967== LEAK SUMMARY:
==1967==    definitely lost: 0 bytes in 0 blocks
==1967==    indirectly lost: 0 bytes in 0 blocks
==1967==      possibly lost: 0 bytes in 0 blocks
==1967==    still reachable: 14,689 bytes in 10 blocks
==1967==         suppressed: 205,181 bytes in 226 blocks
==1967== 
==1967== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
--1967-- 
--1967-- used_suppression:     64 readline_general_leak readline.supp:80 suppressed: 203,637 bytes in 224 blocks
--1967-- used_suppression:      1 readline_funmap_leak readline.supp:12 suppressed: 1,536 bytes in 1 blocks
--1967-- used_suppression:      1 readline_prompt_leak readline.supp:2 suppressed: 8 bytes in 1 blocks
==1967== 
==1967== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)