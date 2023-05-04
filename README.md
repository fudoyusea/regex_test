# regex_test
## 快速开始
### hyperscan性能测试
compile:g++ hyperscan_test.cpp /usr/local/lib/libhs.a /usr/local/lib/libhs_runtime.a -o hyperscan_test -l sqlite3

run:./hyperscan_test <regexPath> <dataPath>

### pcre性能测试
compile:g++ pcre_test.cpp /usr/local/lib/libpcre.a -o pcre_test -l sqlite3

run:./pcre_test <regexPath> <dataPath>

### re2性能测试
compile:g++ re2_test.cpp /usr/local/lib/libre2.a -lpthread -o re2_test -l sqlite3

run:./re2_test <regexPath> <dataPath>

### ragel性能测试
compile and run: sh ragel_run_g2compile.sh  ||  sh ragel_run_ocompile.sh

You are supposed to change the path of regex and data in the shell file.

## 测试流程设计
to do

