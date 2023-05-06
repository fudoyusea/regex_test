#include <iostream>
#include <fstream>
#include <string>
#include <memory.h>
#include <stdio.h>
#include <sys/time.h>
#include <vector>

#define MAXN 1000
#define MAXLENGTH 1000000

std::vector<std::string> regex;

typedef struct trie_node_t {
    uint8_t ch;
    trie_node_t * fail;
    bool tail_node;
    trie_node_t * next[MAXN];
};

int load_regex(std::string regexPath, bool isRegexFormat) {
    std::ifstream regexIn(regexPath);
    std::string s;
    int num = 0;

    if(!isRegexFormat) {
        while(getline(regexIn, s)) {
            regex.emplace_back(s);
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
            regex.emplace_back(s);
            // printf("%s\n", rp.regex);
            ++num;
            getline(regexIn, s, ':');
        }
    }

    return num;
}

trie_node_t *create_trie_node(uint8_t ch) {
    trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
    memset(node, 0, sizeof(trie_node_t));

    node->ch = ch;
    node->fail = NULL;
    node->tail_node = false;

    return node;
}

int trie_node_insert(trie_node_t *root, std::string cstr, size_t slen) {
    trie_node_t *cur = root;

    for (size_t i = 0; i < slen; i++) {
        uint8_t ch = cstr[i];
        trie_node_t *child = cur->next[ch];
        if (child == NULL) {
            child = create_trie_node(ch);
            cur->next[ch] = child;
        }
        cur = child;
    }

    cur->tail_node = true;
    //success
    return 0;
}

trie_node_t *fail_ptr_find(trie_node_t *cur, uint8_t ch, trie_node_t *default_ptr) {
    trie_node_t *res = default_ptr;
    trie_node_t *failptr = cur->fail;
    while (failptr != NULL) {
        // same name child
        if (failptr->next[ch] != NULL) {
            res = failptr->next[ch];
            break;
        }
        failptr = failptr->fail;
    }

    return res;
}

void fail_ptr_build(trie_node_t *root) {
    // the failure pointer of the root node points to nil
    root->fail = NULL;

    std::vector<trie_node_t*> queue;
    int head = 0;
    int tail = 0;
    queue.emplace_back(root);
    ++head;

    while (head != tail) {
        trie_node_t *cur = queue[tail++];
        for (int i = 0; i < MAXN; i++) {
            if (cur->next[i] != NULL) {
                trie_node_t *child = cur->next[i];
                child->fail = fail_ptr_find(cur, child->ch, root);
                queue.emplace_back(child);
                ++head;
            }
        }
    }

}

int tail_node_check(trie_node_t *cur) {
    if (cur->tail_node) {
        return 0;
    }
    return -1;
}

int search(trie_node_t *root, char *match_string, size_t slen) {
    trie_node_t *cur = root;
    int cnt = 0;

    for (size_t i = 0; i < slen; i++) {
        uint8_t ch = match_string[i];
        while(cur->next[ch] == NULL && cur != root) {
            cur = cur->fail;
        }
        cur = cur->next[ch];
        if (cur == NULL) {
            cur = root;
        }

        trie_node_t *tmp = cur;
        while (tmp != root) {
            if (tail_node_check(tmp) == 0) {
                cnt += 1;
            }
            tmp = tmp->fail;
        }
    }

    return cnt;
}


int main(int argc, char *argv[]) {

    std::string regexPath = argv[1];
    bool flag;
    if(regexPath.find("format") != std::string::npos)
        flag = true;
    else
        flag = false;
    int regexNum = load_regex(regexPath,flag);

    trie_node_t *root = create_trie_node('r');
    for (int i = 0; i < regexNum; i++) {
        trie_node_insert(root, regex[i], regex[i].size());
    }
    fail_ptr_build(root);

    char dataArray[MAXLENGTH];
    double totalMatchTime = 0.0;
    struct timeval matchingBeginTime, matchingEndTime;
    int totalMatchedPos = 0;
    int testNum = 100;
    for (int i = 0; i < testNum; i++) {
        FILE *fptr = fopen(argv[2], "r");
        int singleTestMatchedPos = 0;
        while(fgets(dataArray, sizeof(dataArray), fptr) != NULL) {
            gettimeofday(&matchingBeginTime, NULL);
            singleTestMatchedPos += search(root, dataArray, strlen(dataArray));
            gettimeofday(&matchingEndTime, NULL);
            totalMatchTime += matchingEndTime.tv_sec-matchingBeginTime.tv_sec 
                                        + (double)(matchingEndTime.tv_usec-matchingBeginTime.tv_usec)/1000000.0;
        }
        totalMatchedPos = singleTestMatchedPos;
    }
    std::cout << "matching total pos:" << totalMatchedPos << std::endl;
    std::cout << "matched average time: " << totalMatchTime / testNum  << "ms;" << std::endl;

    return 0;
}