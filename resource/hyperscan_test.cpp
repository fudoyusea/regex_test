#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/time.h>

#include <hs/hs_common.h>
#include <hs/hs_runtime.h>
#include <hs/hs_compile.h>

#include "dataBlock.h"

/**
 * hyperscan based regex engine test.
 * compile:g++ hyperscan_test.cpp /usr/local/lib/libhs.a /usr/local/lib/libhs_runtime.a -o hyperscan_test -l sqlite3
 * run: ./hyperscan_test ../regex/single_char_format.data ../data_gene/data_gen_results/dataGen_regex_javaclassconcate10000.txt
**/

#define MAXREGEXNUM 4000 //max regex number

typedef struct regexPattern {
    uint32_t id;
    char regex[5000];
}regPatt;

typedef struct regexPatternArray {
    uint32_t ids[4000];
    uint32_t flags[4000];
    const char *regexs[5000];
}regPattArray;

class hyperscanTest
{

public:

    hyperscanTest(std::string rPath, std::string dPath, bool isFormat);
    ~hyperscanTest();
    int compilePattern();
    double compileTest();
    std::vector<double> matchingTest();
    static int onMatching(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *context);

private:

    std::string regexPath;              // regex store path
    std::string dataPath;               // corpora store path
    bool isRegexFormat;                 // regex is format data(1:/<regex>/<opt>)
    std::vector<regPatt> originRegexs;  // regex array
    regexPatternArray regexsArray;      // regex combinated array
    std::vector<dataBlock> datasArray;  // data array
    hs_database_t *regexDatabase = NULL;// compile regex db
    uint32_t regexCount;                // total regex count

public:

    static std::vector<uint32_t> matchedNum;    // matched num cache

};

hyperscanTest::hyperscanTest(std::string rPath, std::string dPath, bool isFormat) : regexPath(rPath), dataPath(dPath), isRegexFormat(isFormat) {
    // initialize regex pattern
    std::ifstream regexIn(regexPath);
    std::string s;
    int num = 0;
    regPatt rp;
    if(!isRegexFormat) {
        while(getline(regexIn, s)) {
            rp.id = num;
            strcpy(rp.regex, s.c_str());
            originRegexs.push_back(rp);
            ++num;
        }
    }
    else {
        getline(regexIn, s, ':');
        while(getline(regexIn,s)) {
            rp.id = num;

            //数据集给出正则规则形式：/<regex>/<flags>；flags可能为1位、2位或3位；
            int i; 
            for(i = s.size()-1; i >= 0; --i) {
                if(s[i] != '/')
                    continue;
                else
                    break;
            }
            s = s.substr(1, i-1);
            strcpy(rp.regex, s.c_str());
            // std::cout << rp.regex << std::endl;
            originRegexs.push_back(rp);
            ++num;
            getline(regexIn, s, ':');
        }
    }

    regexCount = num;

    /**
     * initialize regex pattern array
    */
    for(int i = 0; i < regexCount; ++i) {
        regexsArray.ids[i] = i;
        regexsArray.regexs[i] = originRegexs[i].regex;
        regexsArray.flags[i] = HS_FLAG_CASELESS;
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
    load_data(dataPath, datasArray, flag);

    // initialize matched num array
    matchedNum.resize(regexCount);

    // initialize regex database;
    regexDatabase = NULL;

}

hyperscanTest::~hyperscanTest() {}

int hyperscanTest::compilePattern() {
    hs_error_t error;
    hs_compile_error_t *compError;
    error = hs_compile_multi(regexsArray.regexs, regexsArray.flags, regexsArray.ids, regexCount, 
                            HS_MODE_BLOCK, NULL, (hs_database_t **) &regexDatabase, 
                            (hs_compile_error_t **) &compError);
    if(error != HS_SUCCESS) {
        std::cout << "compile failed! error code:" << error << ". error:" << compError->message << std::endl;
    }
    return 0;
}

int hyperscanTest::onMatching(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *context) {
    ++matchedNum[id]; 
    return 0;
}

double hyperscanTest::compileTest() {
    /**
     * compiling;
    */
    struct timeval compBeginTime, compEndTime;
    double compTime;
    gettimeofday(&compBeginTime, NULL);
    compilePattern();
    gettimeofday(&compEndTime, NULL);
    compTime = (compEndTime.tv_sec - compBeginTime.tv_sec) 
                + (double)(compEndTime.tv_usec - compBeginTime.tv_usec)/1000000.0;
    return compTime*1000;
}

std::vector<double> hyperscanTest::matchingTest() {
    matchedNum.assign(matchedNum.size(), 0);

    /*
     *initialize;
    */
    hs_scratch_t * hsScra = NULL;
    hs_alloc_scratch(regexDatabase, (hs_scratch_t **) &hsScra);
    double matchingSingleTime = 0.0;
    struct timeval matchingBeginTime, matchingEndTime;
    char *cString;
    hs_error_t err;
    double totalScanBytes = 0;
    /**
     * scanning;
    */
    for(int i = 0; i < datasArray.size(); ++i) {
        cString = const_cast<char *>(datasArray[i].dataLoad.c_str());
        gettimeofday(&matchingBeginTime, NULL);
        err = hs_scan(regexDatabase, cString, strlen(cString), 0, hsScra, onMatching, cString);
        gettimeofday(&matchingEndTime, NULL);
        if(err != HS_SUCCESS)
            std::cout << "matching failed, error code:" << err << std::endl;
        matchingSingleTime += matchingEndTime.tv_sec-matchingBeginTime.tv_sec 
                                + (double)(matchingEndTime.tv_usec-matchingBeginTime.tv_usec)/1000000.0;
        totalScanBytes += datasArray[i].bytesNum;
    }

    double totalMatched = 0;
    for(int i = 0; i < regexCount; ++i) {
        totalMatched += matchedNum[i];
    }
    
    std::cout << "matching single iteration timeout:" << matchingSingleTime*1000 << " ms" << std::endl;

    return std::vector<double>{matchingSingleTime, totalScanBytes, totalMatched};

}

std::vector<uint32_t> hyperscanTest::matchedNum(MAXREGEXNUM,0);

int main(int argc, char **argv) {

    /*
     *read path config;
    */
    std::string regexPath = argv[1];//"../regex/single_char.data";
    std::string dataPath = argv[2];//"../data_gene/data_gen_results/dataGen_regex_javaclassconcate10000.txt";

    /**
     * regex config;
     * flag == 0:load data with format;
     * flag == 1:load data without format;
    */
    int flag;
    if(regexPath.find("format") != std::string::npos)
        flag = true;
    else
        flag = false;
    
    /**
     * compile;
    */
    hyperscanTest hsTest(regexPath, dataPath, flag);
    double compileTime = hsTest.compileTest();

    /**
     * scan and match;
    */
    int testNum = 10000;
    double totalMatchTime = 0.0;
    std::vector<double> matchingResult;
    for(int i = 0; i < testNum; ++i) {
        matchingResult = hsTest.matchingTest();
        totalMatchTime += matchingResult[0]*1000;
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

