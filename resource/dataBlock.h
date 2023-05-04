#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "sqlite3.h"

class dataBlock {

public:

    dataBlock(unsigned int in_id, unsigned int in_stream, std::string in_data, unsigned int bytes_num)
    : id(in_id), streamId(in_stream), dataLoad(in_data), bytesNum(bytes_num) {}

    unsigned int id;
    unsigned int streamId;
    std::string dataLoad;
    unsigned int bytesNum;

};

int load_data_from_txt(std::string inPath, std::vector<dataBlock> &dB) {
    std::ifstream dataIn(inPath);
    std::string str;
    int id = 0;
    while(getline(dataIn, str)) {
        dB.emplace_back(id, id, str, str.size());
        ++id;
    }
    return 0;
}

int read_row(sqlite3_stmt *stmt, std::vector<dataBlock> &dB) {
    unsigned int id = sqlite3_column_int(stmt, 0);
    unsigned int stream_id = sqlite3_column_int(stmt, 1);
    const char* data = (const char*)sqlite3_column_blob(stmt, 2);
    unsigned int bytes = sqlite3_column_bytes(stmt, 2);
    if(!(data && bytes > 0))
        std::cout << "read error" << std::endl;
    
    dB.emplace_back(id, stream_id, std::string(data, data + bytes), bytes);
}

int load_data_from_DB(std::string inPath, std::vector<dataBlock> &dB) {
    sqlite3 *dataDB = nullptr;
    int err = sqlite3_open_v2(inPath.c_str(), &dataDB, SQLITE_OPEN_READONLY, nullptr);

    if(err != SQLITE_OK) {
        std::cout << "open database failed!" << std::endl;
        err = sqlite3_close(dataDB);
        assert(err == SQLITE_OK);
    }

    static const std::string query("SELECT id,stream_id,data FROM chunk ORDER BY id;");

    sqlite3_stmt *statement = nullptr;

    err = sqlite3_prepare_v2(dataDB, query.c_str(), query.size(), &statement, nullptr);
    if(err != SQLITE_OK) {
        std::cout << "read db failed" << std::endl;
        err = sqlite3_close(dataDB);
        assert(err == SQLITE_OK);
    }

    err = sqlite3_step(statement);
    while(err == SQLITE_ROW) {
        read_row(statement, dB);
        err = sqlite3_step(statement);
    }

    if(err != SQLITE_DONE) {
        std::cout << "read db errored!" << std::endl;
        err = sqlite3_close(dataDB);
        assert(err == SQLITE_OK);
    }

    err = sqlite3_finalize(statement);
    assert(err == SQLITE_OK);
    err = sqlite3_close(dataDB);
    assert(err == SQLITE_OK);

    return 0;

}

int load_data(std::string inPath, std::vector<dataBlock> &dB, int flag) {

    switch (flag)
    {

    case 0:{
        load_data_from_txt(inPath, dB);
        break;
    }

    case 1:{
        load_data_from_DB(inPath, dB);
        break;
    }
    default:
        break;
    }
    return 0;
}

