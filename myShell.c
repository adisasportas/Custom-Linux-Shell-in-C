#include "myShell.h"
#include "myFunction.h"

int main()
{
    welcome();
    while (1)
    {

        int piping = 1;
        int pipeIndex;
        getLocation();
        char *input = getInputFromUser();

        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
            logout(input);
        char **arguments = splitArgument(input);
        int foundTwoRedirect = 0;
        int foundOneRedirect = 0;
        for (int i = 0; arguments[i] != NULL; i++)
        {
            if (strcmp(arguments[i], ">>") == 0)
            {
                foundTwoRedirect = 1;
                break;
            }
            else if (strcmp(arguments[i], ">") == 0)
            {
                foundOneRedirect = 1;
                break;
            }
        }
        if (foundTwoRedirect)
            echoppend(arguments);
        else if (foundOneRedirect)
            echorite(arguments);
        else if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "read") == 0)
            readd(arguments);
        else if (strcmp(input, "wc") == 0)
            wordCount(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (piping)
        {
            char **beforePipe = splitInput(input, &pipeIndex);
            char **afterPipe = splitAfterPipe(input, pipeIndex);
            mypipe(beforePipe, afterPipe);
            wait(NULL);
            free(beforePipe);
            free(afterPipe);
        }
        else if (strcmp(input, "mv") == 0)
            move(arguments);
        else
        {
            systemCall(arguments);
            wait(NULL);
        }
        free(arguments);
        free(input);
    }
    return 1;
}
/*יש לכתוב את פונקציית הברוכים הבאים כרצונכם אבל קצת יותר ממה שמוצג מטה לדוגמא:
//                     aSPY//YASa
//              apyyyyCY//////////YCa       |
//             sY//////YSpcs  scpCY//Pp     | Welcome to myShell
//  ayp ayyyyyyySCP//Pp           syY//C    | Version 2.4.3
//  AYAsAYYYYYYYY///Ps              cY//S   |
//          pCCCCY//p          cSSps y//Y   | https://github.com/<user>
//          SPPPP///a          pP///AC//Y   |
//               A//A            cyP////C   | Have fun!
//               p///Ac            sC///a   |
//               P////YCpc           A//A   | Craft packets like it is your last
//        scccccp///pSP///p          p//Y   | day on earth.
//       sY/////////y  caa           S//P   |                      -- Lao-Tze
//        cayCyayP//Ya              pY/Ya   |
//         sY/PsY////YCc          aC//Yp
//          sc  sccaCY//PCypaapyCP//YSs
//                   spCPY//////YPSps
//                        ccaacs                        using c                                    using c*/
void welcome()
{
    printf("\n");
    printf("*******************************************************\n");
    printf("*    AAAAA   dddd    iii   sssss    aaaa      sssss   *\n");
    printf("*   AA   AA  d   d    i   s        aa  aa    s        *\n");
    printf("*  AAAAAAAA  d   d    i    ssss   aaaaaaa    ssss     *\n");
    printf("*  AA    AA  d   d    i       s   aa   aa        s    *\n");
    printf("*  AA    AA  dddd    iii  ssss    aa   aa    ssss     *\n");
    printf("*******************************************************\n");
    printf("\n");
    printf("Welcome to myShell\n");
    printf("I Love Coding\n");
    printf("In the world of coding, every line is a thread in the fabric of functionality, weaving together the tapestry of technology.\n");
    printf("----Adi Sasportas----\n");
}