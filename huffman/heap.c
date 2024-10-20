#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *str = "preved medved";
char input[255];

typedef struct MinHeap MinHeap;
typedef struct Node Node;

struct Node {
    int weight;
    unsigned char character;
    char *repr;
    int isLeaf;
    Node *left;
    Node *right;
};

struct MinHeap {
    Node **arr;
    int size;
    int capacity;
    int (*compare)(void *e1, void *e2);
    void (*setWeight)(void *e, int weight);
    int (*getWeight)(void *e);
};

void fill_array(char *str)
{
    for (int i = 0; i < strlen(str); i++) {
        input[(unsigned char)str[i]] += 1;
    }
}

int parent(int i) {
    // Get the index of the parent
    return (i - 1) / 2;
}

int left_child(int i) {
    return (2*i + 1);
}

int right_child(int i) {
    return (2*i + 2);
}

void setWeight(void *e, int weight)
{
    ((Node *) e)->weight = weight;
}

int getWeight(void *e)
{
    return ((Node *) e)->weight;
}


int compare(void *e1, void *e2)
{
    int w1 = ((Node *) e1)->weight, w2 = ((Node *) e2)->weight;
    if (w1 > w2) return 1;
    else if (w1 == w2) return 0;
    else return -1;
}


MinHeap* init_minheap(int capacity, int (*compare)(void *e1, void *e2),
                      void (*setWeight)(void *e, int weight), int (*getWeight)(void *e))
{
    MinHeap* minheap = (MinHeap*) calloc (1, sizeof(MinHeap));
    minheap->arr = (Node **) calloc (capacity, sizeof(void *));
    minheap->capacity = capacity;
    minheap->size = 0;
    minheap->compare = compare;
    minheap->setWeight = setWeight;
    minheap->getWeight = getWeight;
    return minheap;
}

MinHeap* insert_minheap(MinHeap* heap, Node* element) {
    // Inserts an element to the min heap
    // We first add it to the bottom (last level)
    // of the tree, and keep swapping with it's parent
    // if it is lesser than it. We keep doing that until
    // we reach the root node. So, we will have inserted the
    // element in it's proper position to preserve the min heap property
    if (heap->size == heap->capacity) {
        fprintf(stderr, "\nCannot insert. Heap is already full!\n");
        return heap;
    }
    // We can add it. Increase the size and add it to the end
    heap->size++;
    heap->arr[heap->size - 1] = element;

    // Keep swapping until we reach the root
    int curr = heap->size - 1;
    // As long as you aren't in the root node, and while the 
    // parent of the last element is greater than it
    while (curr > 0 && heap->compare(heap->arr[parent(curr)], heap->arr[curr]) > 0) {
        // Swap
        Node *temp = heap->arr[parent(curr)];
        heap->arr[parent(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        // Update the current index of element
        curr = parent(curr);
    }
    return heap; 
}

MinHeap* heapify(MinHeap* heap, int index) {
    // Rearranges the heap as to maintain
    // the min-heap property
    if (heap->size <= 1)
        return heap;
    
    int left = left_child(index); 
    int right = right_child(index); 

    // Variable to get the smallest element of the subtree
    // of an element an index
    int smallest = index; 
    
    // If the left child is smaller than this element, it is
    // the smallest
    if (left < heap->size && heap->compare(heap->arr[left], heap->arr[index]) < 0)
        smallest = left; 
    
    // Similarly for the right, but we are updating the smallest element
    // so that it will definitely give the least element of the subtree
    if (right < heap->size && heap->compare(heap->arr[right], heap->arr[smallest]) < 0) 
        smallest = right; 

    // Now if the current element is not the smallest,
    // swap with the current element. The min heap property
    // is now satisfied for this subtree. We now need to
    // recursively keep doing this until we reach the root node,
    // the point at which there will be no change!
    if (smallest != index) 
    { 
        Node *temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;
        heap = heapify(heap, smallest); 
    }

    return heap;
}

Node* delete_minimum(MinHeap* heap) {
    // Deletes the minimum element, at the root
    if (!heap || heap->size == 0)
        return (Node *) {0};

    int size = heap->size;
    Node *min_element = heap->arr[0];
    Node *last_element = heap->arr[size-1];
    
    // Update root value with the last element
    heap->arr[0] = last_element;

    // Now remove the last element, by decreasing the size
    heap->size--;
    size--;

    // We need to call heapify(), to maintain the min-heap property
    heapify(heap, 0);
    
    return min_element;
}


Node* delete_element(MinHeap* heap, int index) {
    // Deletes an element, indexed by index
    // Ensure that it's lesser than the current root
    //Node* ret = heap->arr[index];
    
    heap->setWeight(heap->arr[index], heap->getWeight(heap->arr[0]) - 1);
    
    // Now keep swapping, until we update the tree
    int curr = index;
    while (curr > 0 && heap->compare(heap->arr[parent(curr)], heap->arr[curr]) > 0) {
        Node *temp = heap->arr[parent(curr)];
        heap->arr[parent(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        curr = parent(curr);
    }

    // Now simply delete the minimum element
    delete_minimum(heap);
    
    return ret;
}


void print_heap(MinHeap* heap) {
    // Simply print the array. This is an
    // inorder traversal of the tree
    printf("Min Heap:\n");
    for (int i = 0; i < heap->size; i++) {
        printf("(%d, %c)-> ", heap->arr[i]->weight, heap->arr[i]->character);
    }
    printf("\n");
}


void free_minheap(MinHeap* heap) {
    if (!heap)
        return;
    free(heap->arr);
    free(heap);
}

Node* join(Node *node1, Node *node2)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = node1->weight + node2->weight;
    node->isLeaf = 0;
    node->left = node1;
    node->right = node2;
    
    return node;
}

// Compute the "height" of a tree -- 
int height(Node *node) {
    if (node == NULL)
        return 0;
    else {
        int lheight = height(node->left);
        int rheight = height(node->right);
        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}

// Print nodes at a current level
void printCurrentLevel(Node *root, int level) {
    if (root == NULL)
        return;
    if (level == 1) {
        printf("(%d, %c) -> ", root->weight, root->character);
    }
    else if (level > 1) {
        printCurrentLevel(root->left, level - 1);
        printCurrentLevel(root->right, level - 1);
    }
}

void printLevelOrder(Node *root) {
    int h = height(root);
    
    for (int i = 1; i <= h; i++)
        printCurrentLevel(root, i);
}


void make(char *str, Node *node) {
    if (node == NULL) return;
    
    if (node->isLeaf) {
        node->repr = (char *) malloc(strlen(str));
        strcpy(node->repr, str);
    }
    
    char *s1 = (char *) malloc(strlen(str) + 1);
    strcpy(s1, str);
    strcat(s1, "0");
    make(s1, node->left);
    
    char *s2 = (char *) malloc(strlen(str) + 1);
    strcpy(s2, str);
    strcat(s2, "1");
    make(s2, node->right);
}

void flatten(Node *node, Node *array, int *node2Cnt)
{
    if (node == NULL) return;
    
    if (node->isLeaf) {      
        printf("\n(%c, %s)", node->character, node->repr);
        array[(*node2Cnt)++] = *node;
    }
    
    flatten(node->left, array, node2Cnt);
    flatten(node->right, array, node2Cnt);
}


int compare_by_char(const void* a, const void* b) {
    return ((Node *)a)->character - ((Node *)b)->character;
}


int compare_by_binary(const void* a, const void* b) {   
    return strcmp(((Node *)a)->repr, ((Node *)b)->repr);
}


int find_code(char c, Node *array, int len)
{
    int position = 0, left = 0, right = len - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (array[middle].character >= c) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (array[position].character != c)
        return -1;
    return position;
}


char find_key(char *key, Node *array, int len)
{
    int position = 0, left = 0, right = len - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        int cmp = strcmp(array[middle].repr, key);
        if (cmp >= 0) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (strcmp(array[position].repr, key) != 0)
        return -1;
    return array[position].character;
}




int main() {
    // Capacity of 100 elements
    MinHeap* heap = init_minheap(100, compare, setWeight, getWeight);
    
    fill_array(str);
    
    for (int i = 0; i < 254; i++) {
        if (!input[i]) continue;
        
        Node *node = (Node *) calloc(1, sizeof(Node));
        node->weight = input[i];
        node->character = i;
        node->isLeaf = 1;
        insert_minheap(heap, node);
    }
    
    printf("Heap before folding:\n");
    print_heap(heap);
    
    Node *arr[2];
    while(heap->size > 1) {
        arr[0] = delete_element(heap, 0);
        arr[1] = delete_element(heap, 0);
        printf("\narr[0]->weight = %d, arr[1]->weight = %d", arr[0]->weight, arr[1]->weight);
        printf("\narr[0]->character = %c, arr[1]->character = %c\n", arr[0]->character, arr[1]->character);
        insert_minheap(heap, join(arr[0], arr[1]));
        
        //insert_minheap(heap, join(delete_element(heap, 0), delete_element(heap, 0)));
        print_heap(heap);
    }

    
    Node *node = delete_element(heap, 0);
    
    make("", node); 
    
    Node array[255];
    int node2Cnt = 0;
    
    flatten(node, array, &node2Cnt);
    printf("\nnode2Cnt = %d", node2Cnt);
    
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) ", array[i].character, array[i].repr);
    }
    
    // Sort the array using qsort
    qsort(array, node2Cnt, sizeof(Node), compare_by_char);
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) -> ", array[i].character, array[i].repr);
    }
    
    
    // Convert input string to binary code
    printf("\n");
    char *s3 = (char *) malloc(100);
    for (int i = 0 ; i < strlen(str); i++) {
        int v = find_code(str[i], array, node2Cnt);
        strcat(s3, array[v].repr);
    }
    
    printf("\nBinary output:\n%s\n", s3);
    
    
    qsort(array, node2Cnt, sizeof(Node), compare_by_binary);
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) -> ", array[i].character, array[i].repr);
    }
    
    
    // decode

    // 1110111110110100100000110011101001
    
    char *decoded_string = (char *) calloc(1, 100);
    char *tmp = (char *) calloc(1, 100);
    int total_decoded = 0;
    int start = 0, end = 1;
    char c;
    
    while(end <= strlen(s3)) {
        strncpy(tmp, s3 + start, end - start);
        if ((c = find_key(tmp, array, node2Cnt)) != -1) {
            decoded_string[total_decoded++] = c;
            start = end;
            memset(tmp, '\0', strlen(tmp));
        }
        ++end;
        
    }
    
    printf("\nDecoded string = %s\n", decoded_string);
    
    
    return 0;
}

