#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/time.h>
#include <memory>

#include "re2/re2.h"

#include "dataBlock.h"

/**
 * re2 based regex engine test.
 * compile:g++ re2_test.cpp /usr/local/lib/libre2.a -lpthread -o re2_test -l sqlite3
 * run:./re2_test ../regex/single_char_format.data ../data_gene/data_gen_results/dataGen_regex_javaclassconcate10000.txt
**/

class re2Test {
public:
    re2Test(std::string regexPath, std::vector<dataBlock> inBlocks, bool isFormat);
    ~re2Test();
    int compilePattern();
    double compileTest();
    std::vector<double> matchingTest();

private:
    std::string regexPath;
    bool isRegexFormat;

    std::vector<std::string> regexArray;
    std::vector<dataBlock> dataArray;
    std::vector<std::unique_ptr<RE2>> re2Database;
    int regexCount;

};

re2Test::re2Test(std::string rPath, std::vector<dataBlock> inBlocks, bool isFormat): regexPath(rPath), dataArray(inBlocks), isRegexFormat(isFormat){

    // initialize regex array;
    std::ifstream regexIn(regexPath);
    std::string s;
    int num = 0;
    if(!isRegexFormat) {
        while(getline(regexIn, s)) {
            regexArray.emplace_back(s);
            ++num;
        }
    }
    else {
        getline(regexIn, s, ':');
        while(getline(regexIn,s)) {
            // 数据集给出正则规则形式：/<regex>/<flags>；flags可能为1位、2位或3位；
            int i; 
            for(i = s.size()-1; i >= 0; --i) {
                if(s[i] != '/')
                    continue;
                else
                    break;
            }
            s = s.substr(1, i-1);
            regexArray.emplace_back(s);
            ++num;
            getline(regexIn, s, ':');
        }
    }

    regexCount = num;

    // initialize regexdatabase;
    re2Database.resize((std::size_t)(regexCount));
    
}

re2Test::~re2Test() {}

int re2Test::compilePattern() {
    for(int i = 0; i < regexCount; ++i) {
        re2Database[i].reset(new RE2(regexArray[i]));
    }
    return 0;
}

double re2Test::compileTest() {
    // compile process;
    struct timeval compBeginTime, compEndTime;
    double compTime;
    gettimeofday(&compBeginTime, NULL);
    compilePattern();
    gettimeofday(&compEndTime, NULL);
    compTime = (compEndTime.tv_sec - compBeginTime.tv_sec) 
                + (double)(compEndTime.tv_usec - compBeginTime.tv_usec)/1000000.0;
    return compTime*1000;
}

std::vector<double> re2Test::matchingTest() {

    // scanning
    double matchingTotalTime = 0.0;
    struct timeval matchingBeginTime, matchingEndTime;
    double totalBytes = 0, matchingTotalNum = 0;
    for(int i = 0; i < regexArray.size(); ++i) {
        int matchingNum = 0;
        double matchingSingleTime = 0.0;
        long totalScanBytes = 0;

        for(int j = 0; j < dataArray.size(); ++j) {
            re2::StringPiece sp(dataArray[j].dataLoad);
            gettimeofday(&matchingBeginTime, NULL);
            while(RE2::FindAndConsume(&sp, *re2Database[i]))
                ++matchingNum;
            gettimeofday(&matchingEndTime, NULL);

            matchingSingleTime += matchingEndTime.tv_sec-matchingBeginTime.tv_sec 
                                    + (double)(matchingEndTime.tv_usec-matchingBeginTime.tv_usec)/1000000.0;
            totalScanBytes += dataArray[j].bytesNum;
        }
        
        matchingTotalNum += matchingNum;
        matchingTotalTime += matchingSingleTime;
        totalBytes = totalScanBytes;
    }
    
    std::cout << "matching single iteration timeout:" << matchingTotalTime*1000 << "ms" << std::endl;
    return std::vector<double> {matchingTotalTime, totalBytes, matchingTotalNum};
}

int main(int argc, char **argv) {
    /*
     *read path config;
    */
    std::string regexPath = argv[1];
    std::string dataPath = argv[2];

    /**
     * read scanning corpora data;
     * flag == 0:load data from txt file;
     * flag == 1:load data from database file;
    */
    int flag;
    if(dataPath[dataPath.size()-1] == 'b')
        flag = 1;
    else
        flag = 0;  
    std::vector<dataBlock> inputBlock;
    load_data(dataPath, inputBlock, flag);

    /**
     * regex config;
    */
    if(regexPath.find("format") != std::string::npos)
        flag = true;
    else
        flag = false;

    /**
     * compile;
    */
    re2Test rTest(regexPath, inputBlock, flag);
    double compileTime = rTest.compileTest();

    /**
     * scan and match;
    */
    double totalMatchTime = 0.0;
    int testNum = 10000;
    std::vector<double> matchingResult;
    for(int i = 0; i < testNum; ++i) {
        matchingResult = rTest.matchingTest();
        totalMatchTime += matchingResult[0] * 1000;
    }

    /**
     * output results;
    */
    std::cout << "compile regular expression time:" << compileTime << "ms" << std::endl;
    std::cout << "scanning total bytes:" << matchingResult[1] << std::endl;
    std::cout << "matching total pos:" << matchingResult[2] << std::endl;
    std::cout << "matched " + std::to_string(testNum) + "-average time: " << totalMatchTime / testNum  << "ms;" << std::endl;
    return 0;
}