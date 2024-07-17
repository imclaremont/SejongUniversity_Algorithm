// BST 구현
// 문제를 단순화하기 위해, 키만 존재하고 원소를 키라 가정
// 외부노드는 실제 키를 가지고 있지 않고, 새로운 노드를 삽입할 위치를 나타내는 역할이다
#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
    int key;
    struct NODE* parent;
    struct NODE* lChild;
    struct NODE* rChild;
} NODE;

NODE* g_root = NULL;

// 기본 메서드
int isExternal(NODE* node);
NODE* createEmptyNode();
NODE* getSibling(NODE* node);
NODE* inorderSucc(NODE* node); // 중위순회 계승자를 반환하는 함수
void printPreorder(NODE* node);
void freeNode(NODE* node);

// 로직 함수들
NODE* treeSearch(int k); // 트리에서 키를 저장한 내부 노드를 반환하는 함수, 혹은 그런 노드가 없다면 삽입될 위치의 외부 노드를 반환
int getKey(int k);
void insertKey(int k); // 외부노드에 키값을 삽입하는 함수
NODE* expandExternal(NODE* node); // 외부노드를 추가해주는 함수
int deleteKey(int k);
void reduceExternal(NODE* externalNode); // 외부 노드를 제거하고 트리를 재구성하는 함수

int main() {
    char cmd;
    int key;
    int res;

    // 초기화: 외부 노드만 있는 빈 트리 생성
    g_root = createEmptyNode();
    while (1) {
        scanf(" %c", &cmd);
        if (cmd == 'i') { // insert
            scanf(" %d", &key);
            insertKey(key);
        }
        else if (cmd == 'd') { // delete
            scanf(" %d", &key);
            res = deleteKey(key);
            if (res == -1) 
                printf("X\n"); // 키가 존재하지 않으면 X 출력
            else 
                printf("%d\n", res);
        }
        else if (cmd == 's') { // search
            scanf(" %d", &key);
            res = getKey(key);
            if (res == -1) 
                printf("X\n"); // 키가 존재하지 않으면 X 출력
            else 
                printf("%d\n", res);
        }
        else if (cmd == 'p') { // print
            printPreorder(g_root);
            printf("\n");
        }
        else if (cmd == 'q') { // quit
            break;
        }
    }

    return 0;
}

int isExternal(NODE* node) {
    return node->lChild == NULL && node->rChild == NULL;
}

NODE* createEmptyNode() {
    NODE* node = (NODE*)malloc(1 * sizeof(NODE));
    node->parent = NULL;
    node->lChild = NULL;
    node->rChild = NULL;
    return node;
}

NODE* getSibling(NODE* node) {
    if (node->parent->lChild == node) return node->parent->rChild;
    else return node->parent->lChild;
}

// 중위순회 계승자를 반환하는 함수
NODE* inorderSucc(NODE* node) {
    NODE* nextNode = node->rChild;

    if (isExternal(nextNode)) return NULL;
    while (!isExternal(nextNode->lChild)) {
        nextNode = nextNode->lChild;
    }
    return nextNode;
}

void printPreorder(NODE* node) {
    if (isExternal(node)) return;

    printf(" %d", node->key); // 노드 출력
    printPreorder(node->lChild); // 왼쪽 자식으로 이동
    printPreorder(node->rChild); // 오른쪽 자식으로 이동
}

void freeNode(NODE* node) {
    if (isExternal(node)) return;

    freeNode(node->lChild);
    freeNode(node->rChild);

    free(node);
}



// 트리에서 키를 저장한 내부 노드를 반환하는 함수, 혹은 그런 노드가 없다면 삽입될 위치의 외부 노드를 반환
NODE* treeSearch(int k) {
    NODE* node = g_root;
    while (!isExternal(node)) {
        if (k == node->key) return node; // 키를 찾으면 노드 반환
        else if (k < node->key) node = node->lChild; // 키가 작으면 왼쪽으로 이동
        else node = node->rChild; // 키가 크면 오른쪽으로 이동
    }
    return node; // 삽입될 위치의 외부 노드 반환
}

int getKey(int k) {
    NODE* targetNode = treeSearch(k);
    if (isExternal(targetNode)) {
        return -1;
    }
    return targetNode->key;
}

// 외부노드에 키값을 삽입하는 함수
void insertKey(int k) {
    NODE* targetNode = treeSearch(k);
    if (isExternal(targetNode)) {
        targetNode->key = k;
        targetNode = expandExternal(targetNode);
    }
}

// 외부노드를 추가해주는 함수
// 외부 노드는 삽입될 위치를 나타내며, 실제로 키가 존재하지 않는 상태
NODE* expandExternal(NODE* node) {
    NODE* leftChild = createEmptyNode();
    NODE* rightChild = createEmptyNode();

    node->lChild = leftChild;
    leftChild->parent = node;
    node->rChild = rightChild;
    rightChild->parent = node;

    return node;
}

int deleteKey(int k) {
    NODE* node = treeSearch(k);
    if (isExternal(node)) return -1; // 삭제할 노드가 존재 x (외부노드는 실제 키를 가지고 있지 않고, 새로운 노드를 삽입할 위치를 나타내는 역할이다)

    NODE* nodeToDelete;
    if (isExternal(node->lChild)) // 왼쪽 자식이 외부 노드인 경우
        nodeToDelete = node->lChild; 
    else if (isExternal(node->rChild)) // 오른쪽 자식이 외부 노드인 경우
        nodeToDelete = node->rChild; 
    else { // 두 자식이 모두 내부 노드인 경우
        NODE* succNode = inorderSucc(node); 
        node->key = succNode->key; // 중위 순회 후계자의 키를 복사
        nodeToDelete = succNode->lChild; // succNode의 왼쪽 자식을 nodeToDelete로 설정
    }

    reduceExternal(nodeToDelete); // 외부 노드를 제거하고 트리를 재구성
    return k;
}

// 외부 노드를 제거하고 트리를 재구성하는 함수
void reduceExternal(NODE* externalNode) {
    NODE* parentNode = externalNode->parent;
    NODE* siblingNode = getSibling(externalNode);

    if (parentNode == g_root) {
        g_root = siblingNode;
        siblingNode->parent = NULL;
    }
    else {
        NODE* grandparentNode = parentNode->parent;
        siblingNode->parent = grandparentNode;
        if (parentNode == grandparentNode->lChild) 
            grandparentNode->lChild = siblingNode; // parentNode가 왼쪽 자식인 경우
        else 
            grandparentNode->rChild = siblingNode; // parentNode가 오른쪽 자식인 경우
    }

    free(externalNode);
    free(parentNode);
}

/*
i 9
i 2
i 15
i 1
i 7
i 11
i 5
i 8
i 3
i 4
p
*/
