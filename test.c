#include <stdio.h>
#include <string.h>


// int main(int ac, char **av, char **envp)
// {
//     (void)ac;  // Silence unused parameter warnings
//     (void)av;

//     printf("Environment Variables:\n");
//     printf("---------------------\n");
    
//     // Loop through envp until we hit NULL
//     for (int i = 0; envp[i] != NULL; i++)
//     {
//         printf("%d: %s\n", i, envp[i]);
//     }
    
//     return 0;
// }



// strtok test 
int main(void)
{
    char path[] = "/home/ekhallaf/.local/nvim/bin:/home/ekhallaf/bin:/home/ekhallaf/.local/nvim/bin:/home/ekhallaf/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin";
    char *dir = strtok(path, ":");

    int index = 1;
    while(dir != NULL)
    {
        if(*dir != '\0')
            printf("Dir %d : %s is valid\n", index, dir);
        else
            printf("Dir %d : %s is invalid\n", index, dir);
        dir = strtok(NULL, ":");
        index ++;
    }
}
