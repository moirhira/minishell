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










minishell$ ls -l | scc
minishell$ scc: : command not found
==15297== 
==15297== FILE DESCRIPTORS: 6 open (5 inherited) at exit.
==15297== Open file descriptor 0:
==15297==    at 0x49E1C1B: dup2 (in /usr/lib/x86_64-linux-gnu/libc.so.6)
==15297==    by 0x40066FF: child_process (executor_pipe_helpers.c:53)
==15297==    by 0x4006558: execute_pipeline (pipeline.c:95)
==15297==    by 0x4006236: execute_commands (executor.c:97)
==15297==    by 0x4001608: main (main.c:102)



















==61865== 496 bytes in 31 blocks are still reachable in loss record 123 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x408183: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x40777D: ft_calloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x402E68: handel_simple_str (tokenizer.c:138)
==61865==    by 0x402983: split_token (tokenizer.c:328)
==61865==    by 0x403908: parse_command (parser.c:138)
==61865==    by 0x4015DE: main (main.c:105)
==61865== 
==61865== 504 bytes in 1 blocks are still reachable in loss record 124 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401B01: convert_env_to_array (utils_2.c:40)
==61865==    by 0x4060DC: execute_external (single_command.c:71)
==61865==    by 0x4068CC: execute_commands (executor.c:104)
==61865==    by 0x4015F4: main (main.c:106)
==61865== 
==61865== 600 bytes in 15 blocks are still reachable in loss record 125 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x40327E: create_token (tokenizer_utils.c:18)
==61865==    by 0x402D5C: handel_operator (tokenizer.c:122)
==61865==    by 0x402967: split_token (tokenizer.c:326)
==61865==    by 0x403908: parse_command (parser.c:138)
==61865==    by 0x4015DE: main (main.c:105)
==61865== 
==61865== 720 bytes in 15 blocks are still reachable in loss record 128 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x403F61: creat_command (parser_utils_1.c:18)
==61865==    by 0x403B00: handel_pipe (parser_utils.c:17)
==61865==    by 0x40378A: parsing (parser.c:98)
==61865==    by 0x403967: parse_command (parser.c:159)
==61865==    by 0x4015DE: main (main.c:105)
==61865== 
==61865== 784 bytes in 62 blocks are still reachable in loss record 130 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401413: retrieve_envp (main.c:51)
==61865==    by 0x40151D: main (main.c:79)
==61865== 
==61865== 992 bytes in 62 blocks are still reachable in loss record 132 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x408183: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x4013C2: retrieve_envp (main.c:46)
==61865==    by 0x40151D: main (main.c:79)
==61865== 
==61865== 992 bytes in 62 blocks are still reachable in loss record 133 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x408183: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401413: retrieve_envp (main.c:51)
==61865==    by 0x40151D: main (main.c:79)
==61865== 
==61865== 992 bytes in 62 blocks are still reachable in loss record 134 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x408183: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x4071A8: ft_strdup (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401476: retrieve_envp (main.c:56)
==61865==    by 0x40151D: main (main.c:79)
==61865== 
==61865== 992 bytes in 62 blocks are still reachable in loss record 135 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x408183: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401B65: convert_env_to_array (utils_2.c:48)
==61865==    by 0x4060DC: execute_external (single_command.c:71)
==61865==    by 0x4068CC: execute_commands (executor.c:104)
==61865==    by 0x4015F4: main (main.c:106)
==61865== 
==61865== 1,080 bytes in 27 blocks are still reachable in loss record 144 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x40327E: create_token (tokenizer_utils.c:18)
==61865==    by 0x40320A: handel_simple_str (tokenizer.c:197)
==61865==    by 0x402983: split_token (tokenizer.c:328)
==61865==    by 0x403908: parse_command (parser.c:138)
==61865==    by 0x4015DE: main (main.c:105)
==61865== 
==61865== 1,120 bytes in 70 blocks are still reachable in loss record 145 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x408183: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x407695: ft_strjoin (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x4031CF: handel_simple_str (tokenizer.c:191)
==61865==    by 0x402983: split_token (tokenizer.c:328)
==61865==    by 0x403908: parse_command (parser.c:138)
==61865==    by 0x4015DE: main (main.c:105)
==61865== 
==61865== 1,488 bytes in 62 blocks are still reachable in loss record 146 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x4013C2: retrieve_envp (main.c:46)
==61865==    by 0x40151D: main (main.c:79)
==61865== 
==61865== 1,874 bytes in 31 blocks are still reachable in loss record 149 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x40777D: ft_calloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x402E68: handel_simple_str (tokenizer.c:138)
==61865==    by 0x402983: split_token (tokenizer.c:328)
==61865==    by 0x403908: parse_command (parser.c:138)
==61865==    by 0x4015DE: main (main.c:105)
==61865== 
==61865== 2,836 bytes in 62 blocks are still reachable in loss record 152 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x4071A8: ft_strdup (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401476: retrieve_envp (main.c:56)
==61865==    by 0x40151D: main (main.c:79)
==61865== 
==61865== 3,620 bytes in 62 blocks are still reachable in loss record 153 of 178
==61865==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==61865==    by 0x40815D: ft_malloc (in /home/moirhira/Desktop/minishell/minishell)
==61865==    by 0x401B65: convert_env_to_array (utils_2.c:48)
==61865==    by 0x4060DC: execute_external (single_command.c:71)
==61865==    by 0x4068CC: execute_commands (executor.c:104)
==61865==    by 0x4015F4: main (main.c:106)
==61865== 
==61865== LEAK SUMMARY:
==61865==    definitely lost: 0 bytes in 0 blocks
==61865==    indirectly lost: 0 bytes in 0 blocks
==61865==      possibly lost: 0 bytes in 0 blocks
==61865==    still reachable: 27,651 bytes in 1,348 blocks
==61865==         suppressed: 208,723 bytes in 252 blocks
==61865== 
==61865== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
--61865-- 
--61865-- used_suppression:     55 readline_general_leak readline.supp:80 suppressed: 187,995 bytes in 235 blocks
--61865-- used_suppression:     15 libtinfo_leak readline.supp:42 suppressed: 19,184 bytes in 15 blocks
--61865-- used_suppression:      1 readline_funmap_leak readline.supp:12 suppressed: 1,536 bytes in 1 blocks
--61865-- used_suppression:      1 readline_prompt_leak readline.supp:2 suppressed: 8 bytes in 1 blocks
==61865== 
==61865== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
minishell$ 
==61797== 
==61797== FILE DESCRIPTORS: 3 open (3 std) at exit.
==61797== 
==61797== HEAP SUMMARY:
==61797==     in use at exit: 208,648 bytes in 249 blocks
==61797==   total heap usage: 1,923 allocs, 1,674 frees, 258,709 bytes allocated
==61797== 
==61797== LEAK SUMMARY:
==61797==    definitely lost: 0 bytes in 0 blocks
==61797==    indirectly lost: 0 bytes in 0 blocks
==61797==      possibly lost: 0 bytes in 0 blocks
==61797==    still reachable: 0 bytes in 0 blocks
==61797==         suppressed: 208,648 bytes in 249 blocks
==61797== 
==61797== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
--61797-- 
--61797-- used_suppression:     52 readline_general_leak readline.supp:80 suppressed: 187,920 bytes in 232 blocks
--61797-- used_suppression:     15 libtinfo_leak readline.supp:42 suppressed: 19,184 bytes in 15 blocks
--61797-- used_suppression:      1 readline_funmap_leak readline.supp:12 suppressed: 1,536 bytes in 1 blocks
--61797-- used_suppression:      1 readline_prompt_leak readline.supp:2 suppressed: 8 bytes in 1 blocks
==61797== 
==61797== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
