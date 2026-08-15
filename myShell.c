#include "myShell.h"
#include "myFunction.h"

int main()
{
    welcome();

    while (1)
    {
        getLocation();

        char *input = getInputFromUser();

        if (strcmp(input, "exit") == 0 ||
            strncmp(input, "exit ", 5) == 0)
        {
            logout(input);
        }

        /*
         * Handle pipe BEFORE splitArgument(),
         * because splitArgument modifies the input string.
         */
        if (strchr(input, '|') != NULL)
        {
            int pipeIndex = -1;

            char **beforePipe = splitInput(input, &pipeIndex);

            if (pipeIndex == -1)
            {
                fprintf(stderr, "Invalid pipe syntax\n");
                free(beforePipe);
                free(input);
                continue;
            }

            char **afterPipe = splitAfterPipe(input, pipeIndex);

            if (beforePipe[0] == NULL || afterPipe[0] == NULL)
            {
                fprintf(stderr, "Invalid pipe command\n");
                free(beforePipe);
                free(afterPipe);
                free(input);
                continue;
            }

            mypipe(beforePipe, afterPipe);
            wait(NULL);

            free(beforePipe);
            free(afterPipe);
            free(input);

            continue;
        }

        char **arguments = splitArgument(input);

        if (arguments == NULL || arguments[0] == NULL)
        {
            free(arguments);
            free(input);
            continue;
        }

        int foundTwoRedirect = 0;
        int foundOneRedirect = 0;

        for (int i = 0; arguments[i] != NULL; i++)
        {
            if (strcmp(arguments[i], ">>") == 0)
            {
                foundTwoRedirect = 1;
                break;
            }

            if (strcmp(arguments[i], ">") == 0)
            {
                foundOneRedirect = 1;
                break;
            }
        }

        if (foundTwoRedirect)
        {
            echoppend(arguments);
        }
        else if (foundOneRedirect)
        {
            echorite(arguments);
        }
        else if (strcmp(arguments[0], "echo") == 0)
        {
            echo(arguments);
        }
        else if (strcmp(arguments[0], "read") == 0)
        {
            readd(arguments);
        }
        else if (strcmp(arguments[0], "wc") == 0)
        {
            wordCount(arguments);
        }
        else if (strcmp(arguments[0], "cd") == 0)
        {
            cd(arguments);
        }
        else if (strcmp(arguments[0], "cp") == 0)
        {
            cp(arguments);
        }
        else if (strcmp(arguments[0], "delete") == 0)
        {
            delete(arguments);
        }
        else if (strcmp(arguments[0], "mv") == 0)
        {
            move(arguments);
        }
        else
        {
            systemCall(arguments);
            wait(NULL);
        }

        free(arguments);
        free(input);
    }

    return 0;
}
