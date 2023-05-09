#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/time.h>

#include <pcre.h>

#include "dataBlock.h"

/**
 * compile: g++ pcre_test.cpp /usr/local/lib/libpcre.a -o pcre_test -l sqlite3
 * run: ./pcre_test ../regex/single_char_format.data ../data_gene/data_gen_results/dataGen_regex_javaclassconcate10000.txt
**/

class pcreTest {
public:
    pcreTest(std::string rPath, std::string dPath, bool isFormat);
    ~pcreTest();
    int compile_regex();
    double compiling();
    std::vector<double> matching();

private:
    std::string regexPath;
    std::string dataPath;        
    bool isRegexFormat;
    std::vector<std::string> regexArray;
    std::vector<dataBlock> dataArray;
    std::vector<pcre*> pcreDatabase;
    int regexCount;
    
    int ovecsize = 10000;

};

pcreTest::pcreTest(std::string rPath, std::string dPath, bool isFormat) : regexPath(rPath),dataPath(dPath),isRegexFormat(isFormat){

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
            // std::cout << s <<std::endl;
            regexArray.emplace_back(s);
            // printf("%s\n", rp.regex);
            ++num;
            getline(regexIn, s, ':');
        }
    }

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
    load_data(dataPath, dataArray, flag);


    regexCount = num;

}

pcreTest::~pcreTest() {}

int pcreTest::compile_regex() {
    pcre* compileResult;
    const char* err;
    int erroffset;
    for(int i = 0; i < regexCount; ++i) {
        compileResult = pcre_compile(regexArray[i].c_str(), 0, &err, &erroffset, NULL);
        if(!compileResult) {
            std::cout << "compile regex failed!" << std::endl;
            return 0;
        }
        pcreDatabase.emplace_back(compileResult);
    }
    return 0;
}

double pcreTest::compiling() {
    // compile process;
    struct timeval compBeginTime, compEndTime;
    double compTime;
    gettimeofday(&compBeginTime, NULL);
    compile_regex();
    gettimeofday(&compEndTime, NULL);
    compTime = (compEndTime.tv_sec - compBeginTime.tv_sec) 
                + (double)(compEndTime.tv_usec - compBeginTime.tv_usec)/1000000.0;
    return compTime*1000;
}

std::vector<double> pcreTest::matching() {

    // scanning process;
    double matchingTotalTime = 0.0;
    struct timeval matchingBeginTime, matchingEndTime;
    double matchingTotalNum = 0.0, totalBytes = 0.0;
    for(int i = 0; i < regexArray.size(); ++i) {
        int matchingNum = 0;
        int ovector[ovecsize];
        double matchingSingleTime = 0.0;
        char* cString;
        int matchingResult;
        long totalScanBytes = 0;

        for(int j = 0; j < dataArray.size(); ++j) {
            cString = const_cast<char *>(dataArray[j].dataLoad.c_str());
            char *ptr = cString;
            int len = dataArray[j].dataLoad.size();

            gettimeofday(&matchingBeginTime, NULL);
            while(1) {
                matchingResult = pcre_exec(pcreDatabase[i], NULL, ptr, len
                            , 0, 0, ovector, ovecsize);
                if(matchingResult < 0) {
                    break;
                }
                ptr += ovector[1];
                len -= ovector[1];
                ++matchingNum;
            }
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

    return std::vector<double>{matchingTotalTime, totalBytes, matchingTotalNum};
}

int main(int argc, char **argv) {
    /*
     *read path config;
    */
    std::string regexPath = argv[1];
    std::string dataPath = argv[2];

    /**
     * regex config;
    */
    int flag;
    if(regexPath.find("format") != std::string::npos)
        flag = true;
    else
        flag = false;

    /**
     * compile;
    */
    pcreTest pTest(regexPath, dataPath, flag);
    double compileTime = pTest.compiling();

    /**
     * scan and match;
    */
    double totalMatchTime = 0.0;
    int testNum = 10000;
    std::vector<double> matchingResult;
    for(int i = 0; i < testNum; ++i) {
        matchingResult = pTest.matching();
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