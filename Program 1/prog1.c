/* ----------------------------------------------------------- */
/* NAME : Lucas Buccill                      User ID: lpbuccil */
/* DUE DATE : 08/26/2019                                       */
/* PROGRAM ASSIGNMENT 1                                        */
/* FILE NAME : prog1.c        (your unix file name)            */
/* PROGRAM PURPOSE :                                           */
/*    Takes seven positive integers anc creates four child     */
/*    process that will calculate the nth fibonacci number,    */
/*    buffons needle problem, estimate the area of an ellipse  */
/*    simulate a simpe pinball game.                           */
/* ----------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

/* ----------------------------------------------------------- */
/* FUNCTION  fib :                                             */
/*     Calculates the nth fibonacci number                     */
/* PARAMETER USAGE :                                           */
/*    (long) nth fibonacci number                              */
/* FUNCTION CALLED :                                           */
/*    fib(long)                                                */
/* ----------------------------------------------------------- */
long fib(long);

/* ----------------------------------------------------------- */
/* FUNCTION  buffon :                                          */
/*     Calculate the probability of a dropped needle           */
/*     crossing a divided line                                 */
/* PARAMETER USAGE :                                           */
/*    (int) number of times to drop the needle                 */
/* FUNCTION CALLED :                                           */
/*    srand, rand, time, sin                                   */
/* ----------------------------------------------------------- */
double buffon(int);

/* ----------------------------------------------------------- */
/* FUNCTION  ellipseHits :                                     */
/*     Given the semi major and semi minor axis, calculate     */
/*     the approximate area  by randomally selecting points    */
/*     and checking if they are in the ellipse                 */
/* PARAMETER USAGE :                                           */
/*    (int) semi-major, (int) semi-minor, (int) points         */
/*     to select                                               */
/* FUNCTION CALLED :                                           */
/*    srand, time,  rand, pow                                  */
/* ----------------------------------------------------------- */
int ellipseHits(int, int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  dropBalls :                                       */
/*     Calculate the number of balls in each bin               */
/* PARAMETER USAGE :                                           */
/*    (int) number of balls,(int) number of bins, (long *)     */
/*    pointer to a long array of (bins)                        */
/* FUNCTION CALLED :                                           */
/*    srand, time, rand                                        */
/* ----------------------------------------------------------- */
void dropBalls(int, int, long *);

/* ----------------------------------------------------------- */
/* FUNCTION  getAsterisks :                                    */
/*     Populates a char* with the number of asterisks          */
/*     scaled to 50 given the number of balls to max balls     */
/* PARAMETER USAGE :                                           */
/*    (int) max balls in a bin, (int) number of balls in the   */
/*    current bin, (char*) string to store the asterisks       */
/* FUNCTION CALLED :                                           */
/*    round                                                    */
/* ----------------------------------------------------------- */
void getAsterisks(long, long, char *);

int main(int argc, char *argv[])
{
    int status, n, r, a, b, s, x, y, i;
    char buffer[100];

    if (argc != 8)
    {
        printf("Usage ./prog1 n r a b s x y\n");
        exit(0);
    }

    n = atoi(argv[1]);
    r = atoi(argv[2]);
    a = atoi(argv[3]);
    b = atoi(argv[4]);
    s = atoi(argv[5]);
    x = atoi(argv[6]);
    y = atoi(argv[7]);

    printf("Main Process Started\n");
    printf("Fibonacci Input            = %d\n", n);
    printf("Buffon's Needle Iterations = %d\n", r);
    printf("Total random Number Pairs  = %d\n", s);
    printf("Semi-Major Axis Length     = %d\n", a);
    printf("Semi-Minor Axis Length     = %d\n", b);
    printf("Number of Bins             = %d\n", x);
    printf("Number of Ball Droppings   = %d\n", y);

    if (fork() == 0)
    {
        //child 1
        //fib number
        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "   Fibonacci Process Started\n");
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "   Input Number %d\n", n);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "   Fibonacci Number f(%d) is %ld\n", n, fib(n));
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "   Fibonacci Process Exits\n");
        write(1, buffer, strlen(buffer));

        exit(0);
    }
    else
    {
        //parent
        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Fibonacci Process Created\n");
        write(1, buffer, strlen(buffer));
    }

    if (fork() == 0)
    {
        //child 2
        //buffons needle

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "      Buffon's Needle Process Started\n");
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "      Input Number %d\n", r);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "      Estimated Probability is %1.5f\n", buffon(r));
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "      Buffon's Needle Process Exits\n");
        write(1, buffer, strlen(buffer));

        exit(0);
    }
    else
    {
        //parent
        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Buffon's Needle Process Created\n");
        write(1, buffer, strlen(buffer));
    }

    if (fork() == 0)
    {
        //child 3
        //area of ellipse
        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Ellipse Area Process Started\n");
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Total random Number Pairs %d\n", s);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Semi-Major Axis Length %d\n", a);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Semi-Minor Axis Length %d\n", b);
        write(1, buffer, strlen(buffer));

        int totalHits = ellipseHits(a, b, s);

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Total Hits %d\n", totalHits);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Estimated Area is %.5f\n", (((double)totalHits / (double)s) * (double)a * (double)b) * 4.0);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Actual Area is %.5f\n", acos(-1.0) * (double)a * (double)b);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "         Ellipse Area Process Exits\n");
        write(1, buffer, strlen(buffer));

        exit(0);
    }
    else
    {
        //parent
        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Ellipse Area Process Created\n");
        write(1, buffer, strlen(buffer));
    }

    if (fork() == 0)
    {
        //child 4
        //pinball

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Simple Pinball Process Started\n");
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Number of Bins %d\n", x);
        write(1, buffer, strlen(buffer));

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Number of Ball Droppings %d\n", y);
        write(1, buffer, strlen(buffer));

        //execute simulation and store results in bins
        long bins[x];
        memset(&bins, 0, x * sizeof(long));
        dropBalls(y, x, bins);

        //get the max balls any bin contains
        long maxBalls = 0;
        for (i = 0; i < x; i++)
        {
            if (bins[i] > maxBalls)
            {
                maxBalls = bins[i];
            }
        }

        //for each bin print out the data and the histogram
        for (i = 0; i < x; i++)
        {
            double percent = ((double)bins[i] / (double)y) * 100.0;

            char asterisks[50];
            memset(asterisks, '\0', 50);
            bzero(&buffer, sizeof(buffer));
            //get buffer full of asterisks scaled for percent
            getAsterisks(maxBalls, bins[i], asterisks);

            sprintf(buffer, "%3d-(%7ld)-(%5.2f%%)|%s\n", i + 1, bins[i], percent, asterisks);
            write(1, buffer, strlen(buffer));
        }

        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Simple Pinball Process Exits\n");
        write(1, buffer, strlen(buffer));

        exit(0);
    }
    else
    {
        bzero(&buffer, sizeof(buffer));
        sprintf(buffer, "Pinball Process Created\n");
        write(1, buffer, strlen(buffer));
    }

    bzero(&buffer, sizeof(buffer));
    sprintf(buffer, "Main Process Waits\n");
    write(1, buffer, strlen(buffer));
    while (wait(&status) != -1);

    bzero(&buffer, sizeof(buffer));
    sprintf(buffer, "Main Process Exits\n");
    write(1, buffer, strlen(buffer));
    return 0;
}

long fib(long n)
{
    if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else
    {
        return (fib(n - 1) + fib(n - 2));
    }
}

double buffon(int r)
{
    const double pi = acos(-1.0);
    double d, a;
    int totalCrosses, i;

    totalCrosses = 0;
    srand(time(NULL));

    for (i = 0; i < r; i++)
    {
        d = (double)rand() / RAND_MAX;
        a = (double)rand() / (RAND_MAX / (2 * pi));

        if ((d + sin(a)) < 0 || (d + sin(a)) > 1)
        {
            totalCrosses++;
        }
    }

    return totalCrosses / (double)r;
}

int ellipseHits(int a, int b, int s)
{
    double x, y;
    int totalHits, i;

    totalHits = 0;
    srand(time(NULL));

    for (i = 0; i < s; i++)
    {
        x = (double)rand() / (double)(RAND_MAX / a);
        y = (double)rand() / (double)(RAND_MAX / b);
        if (((pow(x, 2) / pow(a, 2)) + (pow(y, 2) / pow(b, 2))) <= 1)
        {
            totalHits++;
        }
    }

    return totalHits;
}

void dropBalls(int numberOfBalls, int numberOfBins, long bins[])
{

    int totalLevels, currentPosition, i, k;

    totalLevels = numberOfBins - 1;
    srand(time(NULL));
    // balls start in the middle, every time they go right they increase their
    // current index, below is a disgram showing the index for each possible path

    /*
                0
              0 * 1
            0 * 1 * 2
          0 * 1 * 2 * 3
        | 0 | 1 | 2 | 3 |
    */
    for (i = 0; i < numberOfBalls; i++)
    {
        currentPosition = 0;

        for (k = 0; k < totalLevels; k++)
        {
            if ((double)rand() / (double)RAND_MAX > .5)
            {
                currentPosition++;
            }
        }
        bins[currentPosition]++;
    }
}

void getAsterisks(long maxBalls, long curBalls, char string[])
{
    int i;
    //Calculate the percent and then scale to 50
    for (i = 0; i < round((50.0 * ((double)curBalls / (double)maxBalls))); i++)
    {
        string[strlen(string)] = '*';
    }
}