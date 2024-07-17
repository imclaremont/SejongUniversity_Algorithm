// AVL 트리 구현
// 문제를 단순화하기 위해, 키만 존재하고 원소를 키라 가정
// 외부노드는 실제 키를 가지고 있지 않고, 새로운 노드를 삽입할 위치를 나타내는 역할이다
#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
    int key;
    int height;
    struct NODE* parent;
    struct NODE* lChild;
    struct NODE* rChild;
} NODE;

NODE* g_root = NULL;

// 기본 메서드
int abs(int n);
int getMax(int n, int m);
int isExternal(NODE* node);
int isRoot(NODE* node);
NODE* createEmptyNode();
void printPreorder(NODE* node);
void freeNode(NODE* node);

// 로직 함수들
NODE* treeSearch(int k); // 트리에서 키를 저장한 내부 노드를 반환하는 함수, 혹은 그런 노드가 없다면 삽입될 위치의 외부 노드를 반환
int getKey(int k);
void insertKey(int k); // 외부노드에 키값을 삽입하는 함수
NODE* expandExternal(NODE* node); // 외부노드를 추가해주는 함수
int isBalanced(NODE* node);
int updateHeight(NODE* node);
void searchAndFixAfterInsertion(NODE* node);
void restructure(NODE* tallestGrandchild, NODE* tallerChild, NODE* unbalancedNode);

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

int abs(int n) {
    return n > 0 ? n : -n;
}

int getMax(int n, int m) {
    return n > m ? n : m;
}

int isExternal(NODE* node) {
    return node->lChild == NULL && node->rChild == NULL;
}

int isRoot(NODE* node) {
    return node->parent == NULL;
}

NODE* createEmptyNode() {
    NODE* node = (NODE*)malloc(1 * sizeof(NODE));
    node->parent = NULL;
    node->lChild = NULL;
    node->rChild = NULL;
    return node;
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
        searchAndFixAfterInsertion(targetNode);
    }
}

// 외부노드를 추가해주는 함수
// 외부 노드는 삽입될 위치를 나타내며, 실제로 키가 존재하지 않는 상태
NODE* expandExternal(NODE* node) {
    NODE* leftChild = createEmptyNode();
    NODE* rightChild = createEmptyNode();

    node->height = 1;
    node->lChild = leftChild;
    leftChild->parent = node;
    node->rChild = rightChild;
    rightChild->parent = node;

    return node;
}

int isBalanced(NODE* node) {
    NODE* left = node->lChild; 
    NODE* right = node->rChild;

    return abs(left->height - right->height) <= 1;
}

int updateHeight(NODE* node) {
    int h;
    NODE* leftChild = node->lChild;
    NODE* rightChild = node->rChild;

    h = getMax(leftChild->height, rightChild->height) + 1;
    if (h != node->height) {
        node->height = h;
        return 1;
    }
    else {
        return 0;
    }
}

void searchAndFixAfterInsertion(NODE* node) {
    if (isRoot(node)) {
        return;
    }

    node->lChild->height = 0;
    node->rChild->height = 0;
    node->height = 1;

    NODE* unbalancedNode = node->parent; // 불균형한 부트리의 루트 노드
    NODE* tallerChild; // unbalancedNode의 자식 중 더 높은 부트리를 가진 노드
    NODE* tallestGrandchild; // tallerChild의 자식 중 더 높은 부트리를 가진 노드

    // unbalancedNode가 루트가 아니고, 균형이 잡혀있을 때까지 부모로 이동
    while (updateHeight(unbalancedNode) && isBalanced(unbalancedNode)) {
        if (isRoot(unbalancedNode)) {
            return;
        }
        unbalancedNode = unbalancedNode->parent;
    }
    if (isBalanced(unbalancedNode)) {
        return;
    }

    // unbalancedNode의 자식 중 더 높은 부트리를 가진 노드 찾기
    if (unbalancedNode->lChild->height > unbalancedNode->rChild->height) 
        tallerChild = unbalancedNode->lChild;
    else 
        tallerChild = unbalancedNode->rChild;
    
    // tallerChild의 자식 중 더 높은 부트리를 가진 노드 찾기
    if (tallerChild->lChild->height > tallerChild->rChild->height) 
        tallestGrandchild = tallerChild->lChild;
    else 
        tallestGrandchild = tallerChild->rChild;

    restructure(unbalancedNode, tallerChild, tallestGrandchild);
}

void restructure(NODE* unbalancedNode, NODE* tallerChild, NODE* tallestGrandchild) {
    // 재구성될 서브트리의 노드들
    NODE* smallestNode;
    NODE* newRoot;
    NODE* largestNode;

    // 재구성될 서브트리의 서브트리들
    NODE* leftSubtree;
    NODE* leftMiddleSubtree;
    NODE* rightMiddleSubtree;
    NODE* rightSubtree;
 
    // RR 회전: unbalancedNode 키 < tallerChild 키 < tallestGrandchild 키
    if (unbalancedNode->key < tallerChild->key && tallerChild->key < tallestGrandchild->key) { 
        smallestNode = unbalancedNode; 
        newRoot = tallerChild; 
        largestNode = tallestGrandchild;

        leftSubtree = smallestNode->lChild; 
        leftMiddleSubtree = newRoot->lChild; 
        rightMiddleSubtree = largestNode->lChild; 
        rightSubtree = largestNode->rChild;
    }
    // LL 회전: tallestGrandchild 키 < tallerChild 키 < unbalancedNode 키
    else if (tallestGrandchild->key < tallerChild->key && tallerChild->key < unbalancedNode->key) { 
        smallestNode = tallestGrandchild; 
        newRoot = tallerChild; 
        largestNode = unbalancedNode;

        leftSubtree = smallestNode->lChild; 
        leftMiddleSubtree = smallestNode->rChild; 
        rightMiddleSubtree = newRoot->rChild; 
        rightSubtree = largestNode->rChild;
    }
    // RL 회전: unbalancedNode 키 < tallestGrandchild 키 < tallerChild 키
    else if (unbalancedNode->key < tallestGrandchild->key && tallestGrandchild->key < tallerChild->key) { 
        smallestNode = unbalancedNode; 
        newRoot = tallestGrandchild; 
        largestNode = tallerChild;

        leftSubtree = smallestNode->lChild; 
        leftMiddleSubtree = newRoot->lChild; 
        rightMiddleSubtree = newRoot->rChild; 
        rightSubtree = largestNode->rChild;
    }
    // LR 회전: tallerChild 키 < tallestGrandchild 키 < unbalancedNode 키
    else { 
        smallestNode = tallerChild; 
        newRoot = tallestGrandchild;
        largestNode = unbalancedNode;

        leftSubtree = smallestNode->lChild; 
        leftMiddleSubtree = newRoot->lChild; 
        rightMiddleSubtree = newRoot->rChild; 
        rightSubtree = largestNode->rChild;
    }

    // 새로운 루트가 최상위 노드라면 g_root 업데이트
    // 그렇지 않다면 unbalancedNode의 부모 노드와 연결
    if (isRoot(unbalancedNode)) {
        g_root = newRoot;
        newRoot->parent = NULL;
    }
    // unbalancedNode가 부모 노드의 왼쪽 자식인 경우
    else if (unbalancedNode->parent->lChild == unbalancedNode) {
        unbalancedNode->parent->lChild = newRoot;
        newRoot->parent = unbalancedNode->parent;
    }
    // unbalancedNode가 부모 노드의 오른쪽 자식인 경우
    else { 
        unbalancedNode->parent->rChild = newRoot;
        newRoot->parent = unbalancedNode->parent;
    }

    // 재구성된 서브트리의 왼쪽 노드 연결 (smallestNode)
    smallestNode->lChild = leftSubtree;
    leftSubtree->parent = smallestNode;
    smallestNode->rChild = leftMiddleSubtree;
    leftMiddleSubtree->parent = smallestNode;
    updateHeight(smallestNode);

    // 재구성된 서브트리의 오른쪽 노드 연결 (largestNode)
    largestNode->lChild = rightMiddleSubtree;
    rightMiddleSubtree->parent = largestNode;
    largestNode->rChild = rightSubtree;
    rightSubtree->parent = largestNode;
    updateHeight(largestNode);

    // 새로운 루트의 자식 노드 연결 (newRoot)
    newRoot->lChild = smallestNode;
    smallestNode->parent = newRoot;
    newRoot->rChild = largestNode;
    largestNode->parent = newRoot;
    updateHeight(newRoot);
}

/*
i 44
i 17
i 78
i 32
i 50
i 88
i 48
i 62
s 88
*/

/*
i 9
i 31
i 66
i 30
i 1
i 24
p
*/
