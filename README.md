# regex_test
## 环境搭建
### HYPSERSCAN
下载：

https://github.com/intel/hyperscan.git；

依赖：

Cmake, python, ragel, boost

编译安装：

cmake $path;

make && make install;

### PCRE
下载：

https://www.pcre.org;

依赖：

Cmake;

编译安装：

tar zxvf pcre-8.45.tar.gz;

./configure;

make && make install;

pcre-config --version;

### re2
下载：

https://github.com/google/re2;

依赖：

C++11, Cmake, Bazel;

编译安装：

make;

make test;

make install;

make testinstall;

### ragel
sudo apt install ragel;

## 快速开始
### 正则表达式与语义库存储形式

#### regex
1.格式化存储：\<no\>:/\<regex\>/\<flag\>;

文件名中注明format(eg.regex_format.data);

2.简单存储：\<regex\>;

#### corpurs
1.数据库存储：参考gutenberg.db语义库;

2.简单存储：以文本形式存储即可;

### hyperscan性能测试
compile:g++ hyperscan_test.cpp /usr/local/lib/libhs.a /usr/local/lib/libhs_runtime.a -o hyperscan_test -l sqlite3;

run:./hyperscan_test \<regexPath\> \<dataPath\>;

### pcre性能测试
compile:g++ pcre_test.cpp /usr/local/lib/libpcre.a -o pcre_test -l sqlite3;

run:./pcre_test \<regexPath\> \<dataPath\>;

### re2性能测试
compile:g++ re2_test.cpp /usr/local/lib/libre2.a -lpthread -o re2_test -l sqlite3;

run:./re2_test \<regexPath\> \<dataPath\>;

### ragel性能测试
compile and run: sh ragel_run_g2compile.sh  ||  sh ragel_run_ocompile.sh;

You are supposed to change the path of regex and data in the shell file;


## 随机语义库生成
说明：

使用python的xeger库，根据正则表达式反向随机产生数据；

使用：

python data_gene.py;

调整dataLen和dataNum控制单条字符串长度和总字符串个数；
