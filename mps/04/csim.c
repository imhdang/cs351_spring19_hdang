#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#define MAXLEN 256

void printUsage();
void parseArg(int argc, char* argv[], char* tracefile);

int v = 0, s = 0, E = 0, b = 0;

int main(int argc, char* argv[])
{
    char tracefile[MAXLEN];
    parseArg(argc, argv, tracefile);
    printSummary(0, 0, 0);
    return 0;
}

void parseArg(int argc, char* argv[], char* tracefile)
{
    int c;
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (c)
        {
            case 'h':
                printUsage();
                exit(0);
            case 'v':
                v = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                strcpy(tracefile, optarg);
                break;
            default:
                printf("./csim: Invalid command line argument\n");
                printUsage();
                exit(0);
        }
    }

    if (!s || !E || !b || !strcmp(tracefile, "")) 
    {
        printf("./csim: Missing required command line argument\n");
        printUsage();
        exit(0);
    }
}

void printUsage()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n"
    "Options:\n"
    "  -h\t\tPrint this help message.\n"
    "  -v\t\tOptional verbose flag.\n"
    "  -s <num>\tNumber of set index bits.\n"
    "  -E <num>\tNumber of lines per set.\n"
    "  -b <num>\tNumber of block offset bits.\n"
    "  -t <file>\tTrace file.\n\n"
    "Examples:\n"
    "  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"
    "  linux>  ./csim -v -s 4 -E 1 -b 4 -t traces/yi.trace\n");
}
