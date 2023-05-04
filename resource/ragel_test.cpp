
#line 1 "ragel_test_cpp_temp.rl"
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



#line 20 "ragel_test.cpp"
static const char _regexCompiel_actions[] = {
	0, 1, 0
};

static const char _regexCompiel_key_offsets[] = {
	0, 1, 3, 5, 7, 9, 11, 13, 
	15, 17, 19, 21, 23, 25, 27, 29, 
	31, 33, 35, 37, 39, 41, 43, 45, 
	47, 49, 51, 53, 55, 57, 59, 61, 
	63
};

static const char _regexCompiel_trans_keys[] = {
	111, 111, 114, 103, 111, 46, 111, 97, 
	111, 111, 112, 97, 111, 99, 111, 104, 
	111, 101, 111, 46, 111, 109, 111, 111, 
	121, 102, 111, 97, 111, 99, 111, 101, 
	111, 111, 115, 46, 111, 111, 118, 105, 
	111, 101, 111, 111, 119, 46, 111, 102, 
	111, 97, 111, 99, 111, 101, 111, 108, 
	111, 101, 111, 111, 116, 111, 115, 111, 
	0
};

static const char _regexCompiel_single_lengths[] = {
	1, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	1
};

static const char _regexCompiel_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const char _regexCompiel_index_offsets[] = {
	0, 2, 5, 8, 11, 14, 17, 20, 
	23, 26, 29, 32, 35, 38, 41, 44, 
	47, 50, 53, 56, 59, 62, 65, 68, 
	71, 74, 77, 80, 83, 86, 89, 92, 
	95
};

static const char _regexCompiel_indicies[] = {
	1, 0, 1, 2, 0, 3, 1, 0, 
	4, 1, 0, 5, 1, 0, 1, 6, 
	0, 7, 1, 0, 8, 1, 0, 9, 
	1, 0, 10, 1, 0, 11, 1, 0, 
	12, 1, 0, 1, 13, 0, 14, 1, 
	0, 15, 1, 0, 16, 1, 0, 17, 
	1, 0, 1, 18, 0, 19, 1, 0, 
	1, 20, 0, 21, 1, 0, 22, 1, 
	0, 1, 23, 0, 24, 1, 0, 25, 
	1, 0, 26, 1, 0, 27, 1, 0, 
	28, 1, 0, 29, 1, 0, 30, 1, 
	0, 1, 31, 0, 1, 32, 0, 1, 
	0, 0
};

static const char _regexCompiel_trans_targs[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 
	8, 9, 10, 11, 12, 13, 14, 15, 
	16, 17, 18, 19, 20, 21, 22, 23, 
	24, 25, 26, 27, 28, 29, 30, 31, 
	32
};

static const char _regexCompiel_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1
};

static const int regexCompiel_start = 0;
static const int regexCompiel_first_final = 32;
static const int regexCompiel_error = -1;

static const int regexCompiel_en_main = 0;


#line 23 "ragel_test_cpp_temp.rl"


int ragel_test(const char *buf, int &matchedNum) {
    int cs;
    const char* p = buf;
    const char* pe = p + strlen(p);
    
#line 116 "ragel_test.cpp"
	{
	cs = regexCompiel_start;
	}

#line 30 "ragel_test_cpp_temp.rl"
    
#line 123 "ragel_test.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
_resume:
	_keys = _regexCompiel_trans_keys + _regexCompiel_key_offsets[cs];
	_trans = _regexCompiel_index_offsets[cs];

	_klen = _regexCompiel_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _regexCompiel_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _regexCompiel_indicies[_trans];
	cs = _regexCompiel_trans_targs[_trans];

	if ( _regexCompiel_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _regexCompiel_actions + _regexCompiel_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 18 "ragel_test_cpp_temp.rl"
	{
        matchedNum++;
    }
	break;
#line 201 "ragel_test.cpp"
		}
	}

_again:
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	}

#line 31 "ragel_test_cpp_temp.rl"
    return 0;
}


int main(int argc, char *argv[]) {
    // std::string regexPath = "../opensource_dataset/hsbench-samples/pcre/teakettle_2500";
    // std::string dataPath = "../opensource_dataset/hsbench-samples/corpora/alexa200.db";
    std::string dataPath = "../data_gene/data_gen_results/dataGen_regex_javaclassconcate50000.txt";

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
            // std::cout << inputBlock[j].dataLoad << std::endl;
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