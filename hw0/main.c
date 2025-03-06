#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    struct Node *next;
} Node;

int main(void) {
    Node *head = NULL, *tail = NULL;
    FILE *file = fopen("main.c", "r"); 
    if (file == NULL) {
        perror("fail to open main.c");
        return EXIT_FAILURE;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->data = (char)ch;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file); 

    int freq[256] = {0};
    Node *current = head;
    while (current != NULL) {
        freq[(unsigned char)current->data]++;
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        unsigned char c = (unsigned char)current->data;
        if (freq[c] > 0) {
            printf("%c: %d\n", c, freq[c]);
            freq[c] = 0;
        }
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        Node *tmp = current;
        current = current->next;
        free(tmp);
    }
    return EXIT_SUCCESS;
}
