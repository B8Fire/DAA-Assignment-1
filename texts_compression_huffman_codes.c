/*GROUP MEMBERS.
BONFACE GEKARA MORARA - SCT221-0282/2022
BENSON MAINGI KIOKO   - SCT221-0730/2022
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node {
    int size;
    char value;
    struct Node* next;
    struct Node* left;
    struct Node* right;
};

struct PriorityQueue {
    struct Node* nodes[256];//256 is the representation of all ASCII characters
    int size;
};

struct Node* createNode(int size, char value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->size = size;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

struct PriorityQueue* createPriorityQueue() {
    struct PriorityQueue* priorityQueue = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue));
    priorityQueue->size = 0;
    return priorityQueue;
};

void insert(struct PriorityQueue* pq, struct Node* newNode);
struct Node* removeMin(struct PriorityQueue* pq);
struct Node* buildHuffmanTree(int freq[256]);
void generateCodes(struct Node* root, char* code, int depth, char codes[256][256]);
int encodeText(char* text, char codes[256][256], char* encodedText);

int main() {
    char text_size[1000];
    int no_of_characters[256] = {0};
    int count = 0;

    printf("\nEnter text size that needs compression:");
    fgets(text_size, sizeof(text_size), stdin);

    int length = strlen(text_size) - 1;

    printf("The size of the text before compression is %d bits. \n",length*8);

    for(int i = 0;i < length ;i++) {
        no_of_characters[text_size[i]]++;
    }

    struct PriorityQueue* priorityQueue = createPriorityQueue();

    for (int i = 0; i < 256; i++) {
        if (no_of_characters[i] > 0) {
            struct Node* newNode = createNode(no_of_characters[i], (char)i);
            insert(priorityQueue, newNode);
        }
    }
    //build the huffman tree
    struct Node* root = buildHuffmanTree(no_of_characters);

    //generate huffman codes
    char codes[256][256] = {0};
    char code[256];
    generateCodes(root, code, 0, codes);

    //compress the text by encoding
    char encodedText[1000];
    int encodedLength = encodeText(text_size, codes, encodedText);

    printf("\nThe size of the text after compression is %d bits.\n", encodedLength);

    return 0;
}

//insert a node into the priority queue (min-heap)
void insert(struct PriorityQueue* pq, struct Node* newNode) {
    int i = pq->size++;
    pq->nodes[i] = newNode;

    while (i > 0 && pq->nodes[i]->size < pq->nodes[(i - 1) / 2]->size) {
        struct Node* temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        pq->nodes[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

//to remove the node with highest priority
struct Node* removeMin(struct PriorityQueue* pq) {
    if (pq->size == 0) return NULL;

    struct Node* minNode = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];

    int i = 0;
    while (2 * i + 1 < pq->size) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int smallest = i;

        if (leftChild < pq->size && pq->nodes[leftChild]->size < pq->nodes[smallest]->size) {
            smallest = leftChild;
        }
        if (rightChild < pq->size && pq->nodes[rightChild]->size < pq->nodes[smallest]->size) {
            smallest = rightChild;
        }
        if (smallest == i) break;

        struct Node* temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[smallest];
        pq->nodes[smallest] = temp;

        i = smallest;
    }

    return minNode;
}

//build the huffman tree
struct Node* buildHuffmanTree(int freq[256]) {
    struct PriorityQueue* pq = createPriorityQueue();

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            insert(pq, createNode(freq[i], i));
        }
    }

    while (pq->size > 1) {
        struct Node* left = removeMin(pq);
        struct Node* right = removeMin(pq);

        struct Node* internalNode = createNode(left->size + right->size, '\0');
        internalNode->left = left;
        internalNode->right = right;

        insert(pq, internalNode);
    }

    return removeMin(pq);
}

void generateCodes(struct Node* root, char* code, int depth, char codes[256][256]) {
    if (root == NULL) return;

    //check if it is a leaf node, if it is store the code
    if (root->left == NULL && root->right == NULL) {
        code[depth] = '\0';
        strcpy(codes[(unsigned char)root->value], code);
        return;
    }

    //traverse left then right
    code[depth] = '0';
    generateCodes(root->left, code, depth + 1, codes);

    code[depth] = '1';
    generateCodes(root->right, code, depth + 1, codes);
}

//now compress the text using the huffman codes
int encodeText(char* text, char codes[256][256], char* encodedText) {
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        char* code = codes[(unsigned char)text[i]];
        for (int j = 0; code[j] != '\0'; j++) {
            encodedText[count++] = code[j];
        }
    }
    encodedText[count] = '\0';
    return count;
}
