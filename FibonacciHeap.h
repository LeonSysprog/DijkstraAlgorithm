#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <climits>

struct FibonacciNode {
    int key;
    int weight;
    struct FibonacciNode *parent;
    struct FibonacciNode *child;
    struct FibonacciNode *left;
    struct FibonacciNode *right;
    int degree;
    bool mark;  // был ли удален в процессе изменения ключа ребёнок этой вершины

    struct FibonacciNode& operator=(const FibonacciNode& other) {
        key = other.key;
        weight = other.weight;
        parent = other.parent;
        child = other.child;
        left = other.left;
        right = other.right;
        degree = other.degree;
        mark = other.mark;

        return *this;
    }
};

class FibonacciHeap {
    private:
        int size = 0;
        FibonacciNode *min = nullptr;

    public:
        FibonacciHeap() {
            min = new FibonacciNode();
        }

        // +
        void insert(int w, int k) {
            FibonacciNode *node = new FibonacciNode();
            node->key = k;
            node->weight = w;

            if (!size) {
                min = node;
                min->left = node;
                min->right = node;
            }
            else {
                FibonacciNode *prevRight = min->right;
                min->right = node;
                node->left = min;
                node->right = prevRight;
                prevRight->left = node;
            }

            if (node->weight < min->weight)
                min = node;
            
            ++size;
        }

        // +
        int getMinKey() {
            return min->key;
        }

        // +
        FibonacciNode *getMinNode() {
            return min;
        }

        // +
        void unionLists(FibonacciNode *first, FibonacciNode *second) {
            FibonacciNode *L = first->left;
            FibonacciNode *R = first->right;
            second->right = first;
            first->left = second;
            L->right = R;
            R->left = L;
        }

        // +
        void merge(FibonacciHeap *that) {
            if (!that->size)
                return;
            
            if (!size) {
                min = that->min;
                size = that->size;
            }
            else {
                unionLists(min, that->min);
                size += that->size;
            }

            if (min || (that->min && that->min < min))
                min = that->min;
        }

        void consolidate() {
            FibonacciNode **A = new FibonacciNode*[size];
            for (int i = 0; i < size; ++i)
                A[i] = nullptr;

            A[min->degree] = min;
            FibonacciNode *current = min->right;

            while (A[current->degree]) { //current
                if (A[current->degree]) {
                    A[current->degree] = current;
                    current = current->right;
                }
                else {
                    FibonacciNode *conflict = A[current->degree];
                    FibonacciNode *addTo, *adding;

                    if (conflict->weight < current->weight) {
                        addTo = conflict;
                        adding = current;
                    }
                    else {
                        addTo = current;
                        adding = conflict;
                    }

                    unionLists(addTo->child, adding);
                    adding->parent = addTo;
                    ++addTo->degree;
                    current = addTo;

                    if (min->weight > current->weight)
                        min = current;
                }
            }
        }

        int removeMin() {
            FibonacciNode *prevMin = min;
            unionLists(min, min->child);
            FibonacciNode *L = min->left;
            FibonacciNode *R = min->right;
            L->right = R;
            R->left = L;
            
            if (prevMin->right == prevMin)
                return min->key;    //

            min = min->right;
            consolidate();
            --size;
            return prevMin->key;
        }

        void cut(FibonacciNode *x) {
            FibonacciNode *L = x->left;
            FibonacciNode *R = x->right;

            R->left = L;
            L->right = R;
            --x->parent->degree;

            if (x->parent->child == x) 
                if (x->right == x) //
                    x->parent->child = nullptr;
                else
                    x->parent->child = x->right;

            x->right = x;
            x->left = x;
            // x.parent = nullptr;
            unionLists(min, x);
        }

        // +
        void cascadingCut(FibonacciNode *x) {
            while (x->mark) {
                cut(x);
                x = x->parent;
            }

            x->mark = true;
        }

        // +
        void decreaseKey(FibonacciNode *x, int newValue) {
            if (newValue < x->parent->weight) {
                x->weight = newValue;
                return;
            }

            FibonacciNode *parent = x->parent;
            cut(x);
            cascadingCut(parent);
        }

        // +
        int remove(FibonacciNode *x) {
            decreaseKey(x, INT_MAX);
            return removeMin(); //
        }

        FibonacciNode *find(FibonacciNode *node, int key) {
            FibonacciNode *tmp = node;
            while (tmp->parent != nullptr)
                if (tmp->key == key)
                    return tmp;
                else
                    find(tmp, key);

            while (tmp->child != nullptr)
                if (tmp->key == key)
                    return tmp;
                else
                    find(tmp, key);

            while (tmp->left != nullptr)
                if (tmp->key == key)
                    return tmp;
                else
                    find(tmp, key);

            while (tmp->right != nullptr)
                if (tmp->key == key)
                    return tmp;
                else
                    find(tmp, key);

            return nullptr;
        }
};

#endif