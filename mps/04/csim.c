#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#define MAXLEN 256

typedef struct {
    int hits;
    int misses;
    int evictions;
} summary_t;

typedef struct {
    int valid;
    int tag;
    int age;
} line_t;

typedef line_t* set_t;
typedef set_t* cache_t;

void printUsage();
void parseArg(int argc, char* argv[], char* tracefile);
void initCache();
void freeCache();
void simulateCache(unsigned long long address);

int v = 0, s = 0, E = 0, b = 0;
cache_t cache;
summary_t summary = {0, 0, 0};

int main(int argc, char* argv[])
{
    char tracefile[MAXLEN];
    parseArg(argc, argv, tracefile);
    initCache();

    FILE* file = fopen(tracefile, "r");
    char op;
    unsigned long long address;
    int size;

    while (fscanf(file, "%c %llx,%d\n", &op, &address, &size) != EOF)
    {
        switch(op)
        {
            case 'L':
                simulateCache(address);
                break;
            case 'S':
                simulateCache(address);
                break;
            case 'M':
                simulateCache(address);
                simulateCache(address);
                break;
            default:
                break;
        }
    }

    fclose(file);
    freeCache();
    printSummary(summary.hits, summary.misses, summary.evictions);
    return 0;
}

void initCache()
{
    int set_num = 1 << s;
    int line_num = E;
    cache = malloc(sizeof(set_t) * set_num);

    for (int i = 0; i < set_num; ++i)
    {
        cache[i] = malloc(sizeof(line_t) * line_num);
        for (int j = 0; j < line_num; ++j)
        {
            cache[i][j].valid = 0;
            cache[i][j].tag = -1;
            cache[i][j].age = 0;
        }
    }
}

void freeCache()
{
    int set_num = 1 << s;
    for (int i = 0; i < set_num; ++i)
    {
        free(cache[i]);
    }
    free(cache);
}

void simulateCache(unsigned long long address)
{

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
