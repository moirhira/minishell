# What are the things that should doing ?
---
1 - need to check the exit status on the failure and display the message
2 - unset is does not work
3 - check the case of exit with many args :
exit 10 10
exit
bash: exit: too many arguments
moirhira@c1r5p1:~$
4 - also this case:
exit exit
exit: exit: numeric argument required% (the exist status is right in this case)







i change in the main function that when the commande will be executed 