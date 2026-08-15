#include "myFunction.h"

char *strwok(char *str, const char *delim)
{
    static char *next_token = NULL;

    if (str)
        next_token = str;

    if (!next_token || *next_token == '\0')
        return NULL;

    char *token_start = next_token;
    char *current;

    for (current = next_token; *current != '\0'; current++)
    {
        const char *d;

        for (d = delim; *d != '\0'; d++)
        {
            if (*current == *d)
            {
                token_start++;
                break;
            }
        }

        if (*d == '\0')
            break;
    }

    if (*token_start == '\0')
    {
        next_token = token_start;
        return NULL;
    }

    next_token = token_start;

    for (current = token_start; *current != '\0'; current++)
    {
        const char *d;

        for (d = delim; *d != '\0'; d++)
        {
            if (*current == *d)
            {
                *current = '\0';
                next_token = current + 1;
                return token_start;
            }
        }
    }

    next_token = current;
    return token_start;
}


char *getInputFromUser()
{
    int ch;
    int size = 1;
    int index = 0;

    char *str = (char *)malloc(size * sizeof(char));

    if (str == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        str[index] = (char)ch;

        size++;
        index++;

        char *temp = (char *)realloc(
            str,
            size * sizeof(char)
        );

        if (temp == NULL)
        {
            free(str);
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        str = temp;
    }

    str[index] = '\0';

    return str;
}


char **splitArgument(char *str)
{
    char *subStr;
    int size = 2;
    int index = 0;

    subStr = strwok(str, " ");

    char **arguments =
        (char **)malloc(size * sizeof(char *));

    if (arguments == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    arguments[index] = subStr;

    while ((subStr = strwok(NULL, " ")) != NULL)
    {
        size++;
        index++;

        char **temp = (char **)realloc(
            arguments,
            size * sizeof(char *)
        );

        if (temp == NULL)
        {
            free(arguments);
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        arguments = temp;
        arguments[index] = subStr;
    }

    arguments[index + 1] = NULL;

    return arguments;
}


void getLocation()
{
    char location[BUFF_SIZE];
    char hostname[BUFF_SIZE];

    char *username = getenv("USER");

    if (getcwd(location, BUFF_SIZE) == NULL)
    {
        perror("Error retrieving current directory");
        return;
    }

    if (gethostname(hostname, BUFF_SIZE) != 0)
    {
        perror("Error retrieving hostname");
        return;
    }

    if (username == NULL)
    {
        fprintf(
            stderr,
            "Error retrieving username\n"
        );

        return;
    }

    green();
    bold();

    printf("%s@%s:", username, hostname);

    reset();
    blue();

    printf("%s", location);

    reset();

    printf("$ ");
}


void logout(char *input)
{
    free(input);

    puts("Logout successful.");

    exit(EXIT_SUCCESS);
}


void echo(char **arg)
{
    while (*(++arg))
    {
        printf("%s ", *arg);
    }

    puts("");
}


void echoppend(char **args)
{
    char textToAppend[2048] = {0};
    char filePath[2048] = {0};

    int redirectIndex = -1;

    for (int i = 0; args[i] != NULL; ++i)
    {
        if (strcmp(args[i], ">>") == 0)
        {
            redirectIndex = i;
            break;
        }
    }

    if (redirectIndex == -1)
    {
        fprintf(
            stderr,
            "Error: Command format incorrect. "
            "Expected '>>' for redirection.\n"
        );

        return;
    }

    if (args[redirectIndex + 1] == NULL)
    {
        fprintf(stderr, "File path not provided.\n");
        return;
    }

    for (int i = 1; i < redirectIndex; ++i)
    {
        if (strlen(textToAppend) +
                strlen(args[i]) + 2 >
            sizeof(textToAppend))
        {
            fprintf(stderr, "Text is too long.\n");
            return;
        }

        strcat(textToAppend, args[i]);

        if (i < redirectIndex - 1)
            strcat(textToAppend, " ");
    }

    for (
        int i = redirectIndex + 1;
        args[i] != NULL;
        ++i
    )
    {
        if (strlen(filePath) +
                strlen(args[i]) + 2 >
            sizeof(filePath))
        {
            fprintf(stderr, "File path is too long.\n");
            return;
        }

        strcat(filePath, args[i]);

        if (args[i + 1] != NULL)
            strcat(filePath, " ");
    }

    FILE *file = fopen(filePath, "a");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s\n", textToAppend);

    fclose(file);

    printf(
        "Appended '%s' to '%s'.\n",
        textToAppend,
        filePath
    );
}


void echorite(char **args)
{
    char textToWrite[2048] = {0};
    char filePath[2048] = {0};

    int foundRedirection = 0;

    for (int i = 1; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            foundRedirection = 1;

            if (args[i + 1] == NULL)
            {
                fprintf(
                    stderr,
                    "File path not provided.\n"
                );

                return;
            }

            for (
                int j = i + 1;
                args[j] != NULL;
                j++
            )
            {
                if (strlen(filePath) +
                        strlen(args[j]) + 2 >
                    sizeof(filePath))
                {
                    fprintf(
                        stderr,
                        "File path is too long.\n"
                    );

                    return;
                }

                if (j > i + 1)
                    strcat(filePath, " ");

                strcat(filePath, args[j]);
            }

            break;
        }
        else
        {
            if (strlen(textToWrite) +
                    strlen(args[i]) + 2 >
                sizeof(textToWrite))
            {
                fprintf(stderr, "Text is too long.\n");
                return;
            }

            if (i > 1)
                strcat(textToWrite, " ");

            strcat(textToWrite, args[i]);
        }
    }

    if (!foundRedirection)
    {
        fprintf(
            stderr,
            "Redirection operator '>' not found.\n"
        );

        return;
    }

    FILE *file = fopen(filePath, "w");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s\n", textToWrite);

    fclose(file);

    printf(
        "Content written to '%s'.\n",
        filePath
    );
}


/*
 * Change the current working directory.
 *
 * Supports:
 * cd ..
 * cd /home/user
 * cd "/home/user/My Folder"
 */
void cd(char **arg)
{
    if (arg == NULL || arg[1] == NULL)
    {
        fprintf(
            stderr,
            "-myShell: cd: missing operand\n"
        );

        return;
    }

    char path[1024] = {0};

    /*
     * Rebuild the path because splitArgument()
     * separates arguments by spaces.
     */
    for (int i = 1; arg[i] != NULL; i++)
    {
        size_t requiredLength =
            strlen(path) +
            strlen(arg[i]) +
            (i > 1 ? 1 : 0) +
            1;

        if (requiredLength > sizeof(path))
        {
            fprintf(
                stderr,
                "-myShell: cd: path is too long\n"
            );

            return;
        }

        if (i > 1)
        {
            strcat(path, " ");
        }

        strcat(path, arg[i]);
    }

    /*
     * Remove surrounding quotation marks.
     *
     * Example:
     * "/home/adi/My Folder"
     */
    size_t len = strlen(path);

    if (
        len >= 2 &&
        path[0] == '"' &&
        path[len - 1] == '"'
    )
    {
        memmove(
            path,
            path + 1,
            len - 2
        );

        path[len - 2] = '\0';
    }

    if (chdir(path) != 0)
    {
        perror("-myShell: cd");
    }
}


void cp(char **arguments)
{
    if (
        arguments == NULL ||
        arguments[1] == NULL ||
        arguments[2] == NULL
    )
    {
        fprintf(
            stderr,
            "Usage: cp <source> <destination>\n"
        );

        return;
    }

    int ch;

    FILE *src;
    FILE *des;

    src = fopen(arguments[1], "rb");

    if (src == NULL)
    {
        perror("Error opening source file");
        return;
    }

    des = fopen(arguments[2], "wb");

    if (des == NULL)
    {
        perror("Error opening destination file");
        fclose(src);
        return;
    }

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }

    fclose(src);
    fclose(des);
}


void delete(char **path)
{
    if (path == NULL || path[1] == NULL)
    {
        fprintf(
            stderr,
            "Error: Path is NULL or not provided.\n"
        );

        return;
    }

    char fullPath[1024] = {0};

    for (int i = 1; path[i] != NULL; ++i)
    {
        if (
            strlen(fullPath) +
                strlen(path[i]) + 2 >
            sizeof(fullPath)
        )
        {
            fprintf(
                stderr,
                "Error: Path is too long.\n"
            );

            return;
        }

        if (i > 1)
            strcat(fullPath, " ");

        strcat(fullPath, path[i]);
    }

    printf(
        "Attempting to delete file: %s\n",
        fullPath
    );

    if (unlink(fullPath) == 0)
    {
        printf(
            "File '%s' successfully deleted.\n",
            fullPath
        );
    }
    else
    {
        perror("Error deleting file");
    }
}


void systemCall(char **arg)
{
    if (arg == NULL || arg[0] == NULL)
        return;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        execvp(arg[0], arg);

        perror("execvp");

        exit(EXIT_FAILURE);
    }
}


char **splitInput(
    char *input,
    int *pipeIndex
)
{
    char **arguments =
        malloc(
            (strlen(input) + 1) *
            sizeof(char *)
        );

    if (arguments == NULL)
    {
        fprintf(
            stderr,
            "Memory allocation failed\n"
        );

        exit(EXIT_FAILURE);
    }

    int argCount = 0;

    *pipeIndex = -1;

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == '|')
        {
            *pipeIndex = i;
            break;
        }

        if (input[i] != ' ')
        {
            int start = i;

            while (
                input[i] != ' ' &&
                input[i] != '|' &&
                input[i] != '\0'
            )
            {
                i++;
            }

            int end = i;

            arguments[argCount] =
                malloc(
                    (end - start + 1) *
                    sizeof(char)
                );

            if (arguments[argCount] == NULL)
            {
                fprintf(
                    stderr,
                    "Memory allocation failed\n"
                );

                exit(EXIT_FAILURE);
            }

            strncpy(
                arguments[argCount],
                input + start,
                end - start
            );

            arguments[argCount][end - start] =
                '\0';

            argCount++;
        }
    }

    arguments[argCount] = NULL;

    return arguments;
}


char **splitAfterPipe(
    char *input,
    int pipeIndex
)
{
    char **arguments =
        malloc(
            (strlen(input) + 1) *
            sizeof(char *)
        );

    if (arguments == NULL)
    {
        fprintf(
            stderr,
            "Memory allocation failed\n"
        );

        exit(EXIT_FAILURE);
    }

    int argCount = 0;

    for (
        int i = pipeIndex + 1;
        input[i] != '\0';
        i++
    )
    {
        while (input[i] == ' ')
            i++;

        if (input[i] != '\0')
        {
            int start = i;

            while (
                input[i] != ' ' &&
                input[i] != '\0'
            )
            {
                i++;
            }

            int end = i;

            arguments[argCount] =
                malloc(
                    (end - start + 1) *
                    sizeof(char)
                );

            if (arguments[argCount] == NULL)
            {
                fprintf(
                    stderr,
                    "Memory allocation failed\n"
                );

                exit(EXIT_FAILURE);
            }

            strncpy(
                arguments[argCount],
                input + start,
                end - start
            );

            arguments[argCount][end - start] =
                '\0';

            argCount++;
        }
    }

    arguments[argCount] = NULL;

    return arguments;
}


void mypipe(
    char **argv1,
    char **argv2
)
{
    if (
        argv1 == NULL ||
        argv1[0] == NULL ||
        argv2 == NULL ||
        argv2[0] == NULL
    )
    {
        fprintf(stderr, "Invalid pipe command\n");
        return;
    }

    int fildes[2];

    if (pipe(fildes) == -1)
    {
        perror("pipe");
        return;
    }

    pid_t firstPid = fork();

    if (firstPid == -1)
    {
        perror("fork");

        close(fildes[0]);
        close(fildes[1]);

        return;
    }

    if (firstPid == 0)
    {
        if (
            dup2(
                fildes[1],
                STDOUT_FILENO
            ) == -1
        )
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(fildes[0]);
        close(fildes[1]);

        execvp(argv1[0], argv1);

        perror("execvp");

        exit(EXIT_FAILURE);
    }

    pid_t secondPid = fork();

    if (secondPid == -1)
    {
        perror("fork");

        close(fildes[0]);
        close(fildes[1]);

        waitpid(firstPid, NULL, 0);

        return;
    }

    if (secondPid == 0)
    {
        if (
            dup2(
                fildes[0],
                STDIN_FILENO
            ) == -1
        )
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(fildes[0]);
        close(fildes[1]);

        execvp(argv2[0], argv2);

        perror("execvp");

        exit(EXIT_FAILURE);
    }

    close(fildes[0]);
    close(fildes[1]);

    waitpid(firstPid, NULL, 0);
    waitpid(secondPid, NULL, 0);
}


void move(char **args)
{
    if (
        args == NULL ||
        args[1] == NULL ||
        args[2] == NULL
    )
    {
        fprintf(
            stderr,
            "Usage: mv <source> <destination>\n"
        );

        return;
    }

    char srcPath[2048] = {0};
    char destPath[2048] = {0};

    /*
     * Basic two-argument move.
     *
     * Paths containing spaces should preferably
     * be supplied inside quotation marks.
     */
    strncpy(
        srcPath,
        args[1],
        sizeof(srcPath) - 1
    );

    strncpy(
        destPath,
        args[2],
        sizeof(destPath) - 1
    );

    printf(
        "Attempting to move from '%s' to '%s'\n",
        srcPath,
        destPath
    );

    if (rename(srcPath, destPath) == 0)
    {
        printf("File successfully moved.\n");
    }
    else
    {
        perror("Error moving file");
    }
}


void readd(char **args)
{
    if (args == NULL || args[1] == NULL)
    {
        fprintf(
            stderr,
            "Usage: read <path>\n"
        );

        return;
    }

    char filePath[2048] = {0};

    for (int i = 1; args[i] != NULL; ++i)
    {
        if (
            strlen(filePath) +
                strlen(args[i]) + 2 >
            sizeof(filePath)
        )
        {
            fprintf(
                stderr,
                "File path is too long.\n"
            );

            return;
        }

        if (i > 1)
            strcat(filePath, " ");

        strcat(filePath, args[i]);
    }

    FILE *file = fopen(filePath, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char buffer[1024];

    while (
        fgets(
            buffer,
            sizeof(buffer),
            file
        ) != NULL
    )
    {
        printf("%s", buffer);
    }

    fclose(file);
}


void wordCount(char **args)
{
    if (
        args == NULL ||
        args[1] == NULL ||
        args[2] == NULL
    )
    {
        fprintf(
            stderr,
            "Usage: wc <-l/-w> <file path>\n"
        );

        return;
    }

    char *option = args[1];

    char filePath[2048] = {0};

    for (int i = 2; args[i] != NULL; ++i)
    {
        if (
            strlen(filePath) +
                strlen(args[i]) + 2 >
            sizeof(filePath)
        )
        {
            fprintf(
                stderr,
                "File path is too long.\n"
            );

            return;
        }

        if (i > 2)
            strcat(filePath, " ");

        strcat(filePath, args[i]);
    }

    FILE *file = fopen(filePath, "r");

    if (file == NULL)
    {
        fprintf(
            stderr,
            "File '%s' does not exist "
            "or cannot be opened.\n",
            filePath
        );

        return;
    }

    int count = 0;

    if (strcmp(option, "-l") == 0)
    {
        char buffer[1024];

        while (
            fgets(
                buffer,
                sizeof(buffer),
                file
            ) != NULL
        )
        {
            count++;
        }

        printf(
            "Number of lines in the file: %d\n",
            count
        );
    }
    else if (strcmp(option, "-w") == 0)
    {
        int previousChar = ' ';
        int currentChar;

        while (
            (currentChar = fgetc(file)) != EOF
        )
        {
            if (
                currentChar == ' ' ||
                currentChar == '\n' ||
                currentChar == '\t'
            )
            {
                if (
                    previousChar != ' ' &&
                    previousChar != '\n' &&
                    previousChar != '\t'
                )
                {
                    count++;
                }
            }

            previousChar = currentChar;
        }

        if (
            previousChar != ' ' &&
            previousChar != '\n' &&
            previousChar != '\t'
        )
        {
            count++;
        }

        printf(
            "Number of words in the file: %d\n",
            count
        );
    }
    else
    {
        fprintf(
            stderr,
            "Invalid option. "
            "Usage: wc <-l/-w> <file path>\n"
        );
    }

    fclose(file);
}
