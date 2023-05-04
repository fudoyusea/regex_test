#include <stdio.h>
#include <string.h>

#include <sys/time.h>

/**
 * memchar based regex engine test.
 * compile:gcc memchr_test.c -o memchr_test
 * run:./memchr_test ../regex/single_char_ragel.data ../data_gene/data_gen_results/dataGen_regex_javaclassconcate10000.txt
**/

#define MAXDATALENGTH 1000000

int main(int argc, char* argv[]) {
    FILE *dataPtr = fopen(argv[2], "r");
    FILE *regexPtr = fopen(argv[1], "r");
    char dataStream[MAXDATALENGTH];
    char regexChar;

    double matchingTotalTime = 0.0, matchingSingleTime;
    struct timeval matchingBeginTime, matchingEndTime;
    int matchedNum = 0, matchedPos = 0;
    while(fgets(dataStream, sizeof(dataStream), dataPtr) != NULL) {
        while((regexChar=fgetc(regexPtr)) != EOF) {
            if(regexChar == '\n')
                continue;
            
            char* ptr = dataStream;
            int len = strlen(dataStream);
            
            gettimeofday(&matchingBeginTime, NULL);
            while(ptr) {
                ptr = (char*) memchr(ptr + 1, regexChar, len - (ptr-dataStream) - 2);
                if(ptr != NULL)
                    ++matchedPos;
            }
            gettimeofday(&matchingEndTime, NULL);

            matchingSingleTime = matchingEndTime.tv_sec-matchingBeginTime.tv_sec 
                                        + (double)(matchingEndTime.tv_usec-matchingBeginTime.tv_usec)/1000000.0;
            matchingTotalTime += matchingSingleTime;
        }
    }

    fclose(dataPtr);
    fclose(regexPtr);

    printf("Matching total time: ");
    printf("%f ", matchingTotalTime*1000);
    printf("ms.\n");

    printf("Matching total pos: ");
    printf("%d\n ", matchedPos);

    return 0;
}