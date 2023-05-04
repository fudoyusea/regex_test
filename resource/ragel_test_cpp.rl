#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/time.h>

#include "dataBlock.h"

/**
 * ragel based regex engine test.
 * -o compile:ragel -o ragel_test.cpp ragel_test_cpp.rl    g++ ragel_test.cpp -o ragel_test_o -l sqlite3
**/


%%{
    machine regexComplie;
    action matchedAct {
        matchedNum++;
    }
    main := (any* "REGEX" @matchedAct );
    write data;
}%%

int ragel_test(const char *buf, int &matchedNum) {
    int cs;
    const char* p = buf;
    const char* pe = p + strlen(p);
    %% write init;
    %% write exec;
    return 0;
}


int main(int argc, char *argv[]) {
    std::string dataPath = argv[1];
    int flag;
    if(dataPath[dataPath.size()-1] == 'b')
        flag = 1;
    else
        flag = 0;  
    std::vector<dataBlock> inputBlock;
    load_data(dataPath, inputBlock, flag);

    double matchingTotalTime = 0.0, matchingSingleTime;
    struct timeval matchingBeginTime, matchingEndTime;
    int matchedNum = 0;
    for(int i = 0; i < 1; ++i) {
        for(int j = 0; j < inputBlock.size(); ++j) {
            gettimeofday(&matchingBeginTime, NULL);
            ragel_test(inputBlock[j].dataLoad.c_str(), matchedNum);
            gettimeofday(&matchingEndTime, NULL);
            matchingSingleTime = matchingEndTime.tv_sec-matchingBeginTime.tv_sec 
                                        + (double)(matchingEndTime.tv_usec-matchingBeginTime.tv_usec)/1000000.0;
            matchingTotalTime += matchingSingleTime;
        }
    }
    std::cout <<  matchingTotalTime*1000 << std::endl;
    std::cout << matchedNum << std::endl;
    return 0;
}