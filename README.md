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
1 - in the executor try to complete the execute_external function with the exit return value you
will find comment where you need to cahnge (specially in the parent process)
    // try to make it int return exit status
    exit_st = execute_external(command, env); 

2 - also the execute_pipeline try to make it int that return the right exit status in the diffrent cases

3 - i change and impliment new setup redirection function so its work perfectly now its yours i  add 
just some changes 


4 - also the excute commands function i add the pipeline structure in thi area
else if (command && command->pipe)
{
    execute_pipeline(command);
    return (exit_st);
}

its yours i didnt change anything

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
    after execute thus line all the commands not bieng its desplay just command : not found 
