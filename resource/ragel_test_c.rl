#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/**
 * ragel based regex engine test.
 * compile and run: ragel -G2 ragel_test_c.rl      gcc ragel_test_c.c -o ragel_test_G2
**/
#define MAXLENGTH 100

%%{
    machine regexComplie;
    action matchedAct {
        (*matchedNum)++;
    }
    main := (any* "REGEX" @matchedAct );
    write data;
}%%

int ragelTest(const char *buf, int *matchedNum) {
    int cs;
    const char* p = buf;
    const char* pe = p + strlen(p);
    %% write init;
    %% write exec;
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *fptr = fopen(argv[1], "r");
    char dataArray[MAXLENGTH];

    double matchingTotalTime = 0.0, matchingSingleTime;
    struct timeval matchingBeginTime, matchingEndTime;
    int matchedTotalNum = 0;
    while(fgets(dataArray, sizeof(dataArray), fptr) != NULL) {
        gettimeofday(&matchingBeginTime, NULL);
        ragelTest(dataArray, &matchedTotalNum);
        gettimeofday(&matchingEndTime, NULL);
        matchingSingleTime = matchingEndTime.tv_sec-matchingBeginTime.tv_sec 
                                    + (double)(matchingEndTime.tv_usec-matchingBeginTime.tv_usec)/1000000.0;
        matchingTotalTime += matchingSingleTime;
    }

    fclose(fptr);

    printf("%f", matchingTotalTime*1000);
    printf("\n");
    printf("%d", matchedTotalNum);
    return 0;
}