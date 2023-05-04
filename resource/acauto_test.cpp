#include <iostream>
#include <string>
#include <memory.h>
#include <stdio.h>
#include "acauto.h"

#define QUEUE_LEN 100
#define MAXN 100

int64_t difftimespec_ns(const struct timespec after, const struct timespec before)
{
      return ((int64_t)after.tv_sec - (int64_t)before.tv_sec) * (int64_t)1000000000
           + ((int64_t)after.tv_nsec - (int64_t)before.tv_nsec);
 }

const uint8_t input[][50] = {
    "a",
    "b",
    "c"
};


trie_node_t *create_trie_node(uint8_t ch) {
    trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
    memset(node, 0, sizeof(trie_node_t));

    node->ch = ch;
    node->fail = NULL;
    node->tail_node = false;

    return node;
}

int trie_node_insert(trie_node_t *root, const uint8_t *cstr, size_t slen) {
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

int fail_ptr_build(trie_node_t *root) {
    // the failure pointer of the root node points to nil
    root->fail = NULL;

    trie_node_t *queue[QUEUE_LEN];
    int head = 0;
    int tail = 0;
    queue[head++] = root;

    while (head != tail) {
        trie_node_t *cur = queue[tail++];
        for (int i = 0; i < MAXN; i++) {
            if (cur->next[i] != NULL) {
                trie_node_t *child = cur->next[i];
                child->fail = fail_ptr_find(cur, child->ch, root);
                queue[head++] = child;
            }
        }
    }

    return 0;
}

int tail_node_check(trie_node_t *cur) {
    if (cur->tail_node) {
        return 0;
    }
    return -1;
}

int search(trie_node_t *root, const uint8_t *match_string, size_t slen) {
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

    trie_node_t *root = create_trie_node('r');
    int len = sizeof(input) / sizeof(input[0]);

    for (int i = 0; i < len; i++) {
        trie_node_insert(root, input[i], strlen((const char *)input[i]));
    }
    
    fail_ptr_build(root);
    std::cout << "success" << std::endl;
    const uint8_t match_string[] = "abd";
    printf("match string len: %d", strlen((const char *)match_string));

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC,&start);

    int cnt = 0;
    for (int i = 0; i < 100000; i++) {
        cnt += search(root, match_string, strlen((const char *)match_string));
    }
    printf("cnt: %d", cnt);

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC,&end);

    uint64_t interval = difftimespec_ns(end, start);
    printf("start_seconds = %ld, start_nanoseconds = %ld", start.tv_sec, start.tv_nsec);
    printf("end_seconds = %ld, end_nanoseconds = %ld", end.tv_sec, end.tv_nsec);
    printf("interval = %ld", interval);

    return 0;
}