#ifndef _CSLIST_H_
#define _CSLIST_H_

#include <unistd.h>
#include <inttypes.h>

/*
 *  author:  李刚
 *  time:  2017-06-14 08:50:53
 *  qq:   865853453
 */

typedef struct CSlistNode{

    struct CSlistNode *prve;

    struct CSlistNode *next;

    void *value;// 数据节点;

}CSlistNode;

// 链表结构体;
typedef struct CSlist{

    size_t size; // list 存放的个数; -- 进行不断 --即可实现遍历;

    size_t byte; // 偏移量;  tail = head + size * byte;

    // 保留指着head tail 指针 表头 表尾的插入时间 空间复杂度会t1;
    struct CSlistNode *head;// 指向结构的头;

    struct CSlistNode *tail;// 指向list列表的末;

    void (* free) (void);

    void (* alloc) (void);


    void (* dup) (void);

} CSlist;

typedef struct String{

    unsigned encode:4;

    unsigned len:12;

    char content[]; // 存放数据, len 长度

}String;

typedef struct Zlplist{

    uint8_t type;

    uint8_t len;

    CSlistNode nodeset[];

} Zlplist;

#endif // cslist.h
