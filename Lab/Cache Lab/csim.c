#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct cache {
    int valid;
    int tag;
    int cnt;
};

void getOpt(int argc, char** argv, int *s, int *E, int *b, int * verbose, char *filename);
struct cache ** initCache(int s, int E, int b);
void printHelp();
int isHit(int s, int E, int b, int address, struct cache ** Cache);
int isColdMiss(int s, int E, int b, int address, struct cache ** Cache);
void updateColdMiss(int s, int E, int b, int address, struct cache ** Cache);
void updateOtherMiss(int s, int E, int b, int address, struct cache ** Cache);
void updateLRU(int s, int E, int b, int address, struct cache ** Cache);

int Time = 0;

int main(int argc, char** argv) 
{
    int hit = 0, miss = 0, eviction = 0;
    int s, E, b, verbose=0;
    char filename[50];

    getOpt(argc, argv, &s, &E, &b, &verbose, filename);
   
    struct cache ** Cache = initCache(s, E, b);
    
    FILE * pFile = fopen(filename, "r");

    char opt;
    int address, size;
    
    while(fscanf(pFile, " %c %x,%d", &opt, &address, &size) > 0) 
    {
        int total = 1;
        if(opt == 'M') total = 2;
        if(opt == 'I') total = 0;
        while(total--) 
        {
            if(isHit(s, E, b, address, Cache)) 
            {
                hit = hit + 1;
            }
            else 
            {
                miss = miss + 1; 
                if(isColdMiss(s, E, b, address, Cache)) 
                {
                    updateColdMiss(s, E, b, address, Cache);
                }
                else 
                {
                    updateOtherMiss(s, E, b, address, Cache);
                    eviction = eviction + 1;
                }
            }
            updateLRU(s, E, b, address, Cache);
        }
    }

    fclose(pFile);
    printSummary(hit, miss, eviction);
    return 0;
}

void getOpt(int argc, char** argv, int *s, int *E, int *b, int *verbose, char *filename) 
{
    int opt;
    while((opt = getopt(argc, argv, "hvs:E:b:t:") ) != -1) 
    {
        switch (opt)
        {
        case 'h': 
            printHelp();
            break;
        case 'v': 
            *verbose = 1;
            break;
        case 's': 
            *s = atoi(optarg);
            break;
        case 'E':
            *E = atoi(optarg);
            break;
        case 'b': 
            *b = atoi(optarg);
            break;
        case 't': 
            strcpy(filename, optarg);
            break;
        default:
            printHelp();
            break;
        }
    }
}
struct cache ** initCache(int s, int E, int b) 
{
    struct cache **S = (struct cache ** )malloc(sizeof(struct cache *) * (1<<s));
    for(int i = 0; i < (1<<s); i++) 
    {
        S[i] = (struct cache * )malloc(sizeof(struct cache) * E);
    }
    for(int i = 0; i < (1<<s); i++)
    {
        for(int j = 0; j < E; j++) 
        {
            S[i][j].valid = S[i][j].tag = S[i][j].cnt = 0; 
        }
    }
    return S;
}
void printHelp() 
{
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
int isHit(int s, int E, int b, int address, struct cache ** Cache) 
{
    int tag = address >> (s + b); 
    int indexS = address >> b;
    indexS = indexS & ((1 << s) - 1);
    for(int  i = 0; i < E; i++ ) 
    {
        if(Cache[indexS][i].tag == tag && Cache[indexS][i].valid)
        {
            return 1;
        }
    }
    return 0;
}
int isColdMiss(int s, int E, int b, int address, struct cache ** Cache)
{
    int indexS = address >> b;
    indexS = indexS & ((1 << s) - 1);
    for(int i = 0; i < E; i++) 
    {
        if(!Cache[indexS][i].valid) return 1;
    }
    return 0;
}
void updateColdMiss(int s, int E, int b, int address, struct cache ** Cache)
{
    int tag = address >> (s + b);
    int indexS = address >> b;
    indexS = indexS & ((1 << s) -1 );
    for(int i = 0; i < E; i++ )
    {
        if(!Cache[indexS][i].valid)
        {
            Cache[indexS][i].valid = 1; 
            Cache[indexS][i].tag = tag;
            return ;
        }
    }
    return ;
}
void updateOtherMiss(int s, int E, int b, int address, struct cache ** Cache)
{
    int tag = address >> (s + b);
    int indexS = address >> b;
    int LRU = 0, minCnt = 1e9;
    indexS = indexS & ((1 << s) - 1);
    for(int i = 0; i < E; i++) 
    {
        if(Cache[indexS][i].cnt < minCnt) 
        {
            minCnt = Cache[indexS][i].cnt;
            LRU = i;
        }
    }
    Cache[indexS][LRU].valid = 1;
    Cache[indexS][LRU].tag = tag;
    return ;
}
void updateLRU(int s, int E, int b, int address, struct cache ** Cache)
{
    int tag = address >> (s + b);
    int indexS = address >> b; 
    indexS = indexS & ((1 << s) -1);
    for(int i = 0; i < E; i++)
    {
        if(Cache[indexS][i].tag == tag && Cache[indexS][i].valid)
        {
            Cache[indexS][i].cnt = ++Time;
            return ;
        }
    }
    return ;
}
