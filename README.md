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



=======================================================




==631541== Command: ./minishell
==631541== 
minishell$ 
minishell$ pwd | $efdgbdb
minishell: syntax error near unexpected token `newline'
minishell$ $sffsfd
