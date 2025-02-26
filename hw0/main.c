#include <stdio.h>
#include <string.h>

#define MAX_LEN 100

// 定義節點結構
typedef struct Node {
    char data;           // 儲存字元
    struct Node *next;   // 指向下一個節點
} Node;

int main(void) {
    char input[MAX_LEN];
    // 讀取一行字串 (最多 MAX_LEN-1 個字元)
    if (fgets(input, MAX_LEN, stdin) == NULL) {
        return 1;  // 讀取失敗就結束
    }
    // 移除結尾的換行符號（若存在）
    input[strcspn(input, "\n")] = '\0';

    // 用陣列模擬節點空間
    Node nodes[MAX_LEN];
    Node *head = NULL;   // 鏈結串列的頭
    Node *tail = NULL;   // 鏈結串列的尾

    // 逐字元建立鏈結串列
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        nodes[i].data = input[i];
        nodes[i].next = NULL;

        if (head == NULL) {
            // 第一個節點
            head = &nodes[i];
            tail = &nodes[i];
        } else {
            // 串到尾巴
            tail->next = &nodes[i];
            tail = &nodes[i];
        }
    }

    // 以兩兩一組的方式輸出
    Node *current = head;
    while (current != NULL && current->next != NULL) {
        printf("%c : %c\n", current->data, current->next->data);
        // 一次跳過兩個節點 (因為我們是字元 : 下一個字元)
        current = current->next->next;
    }

    return 0;
}
