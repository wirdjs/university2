#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

class SearchTree
{
public:
    class Node
    {
    private:
        int   m_key = 0;
        Node *m_leftChild  = nullptr;
        Node *m_rightChild = nullptr;
        
    public:
        Node() = default;

        explicit Node(int key)
            : m_key(key) {}

        Node(int key, Node *leftChild, Node *rightChild)
            : m_key(key), m_leftChild(leftChild), m_rightChild(rightChild) {}

        Node(const Node &other)
            : m_key(other.m_key) {}

        ~Node() = default; // Поддеревья удаляются классом SearchTree, а не самим узлом

        int  key()  const { return m_key; }
        void setKey(int k) { m_key = k; }

        Node *leftChild()  const { return m_leftChild; }
        Node *rightChild() const { return m_rightChild; }

        void setLeftChild (Node *node) { m_leftChild = node; }
        void setRightChild(Node *node) { m_rightChild = node; }

        friend std::ostream &operator<<(std::ostream &out, const Node &node)
        {
            out << node.m_key;
            return out;
        }

        friend class SearchTree;
    };

    SearchTree() = default;

    explicit SearchTree(int rootKey)
        : m_root(new Node(rootKey)) {}

    // Конструктор 3: Из списка инициализации
    SearchTree(initializer_list<int> keys)
    {
        for (int k : keys)
            addNode(k);
    }

    // Конструктор 4: Копирующий конструктор
    SearchTree(const SearchTree &other)
    {
        m_root = copySubtree(other.m_root);
    }

    // Деструктор
    ~SearchTree()
    {
        deleteSubtree(m_root);
    }

    // Оператор присваивания
    SearchTree &operator=(const SearchTree &other)
    {
        if (this != &other) {
            deleteSubtree(m_root);
            m_root = copySubtree(other.m_root);
        }
        return *this;
    }
    
private:
    Node *m_root = nullptr;

    void addNodeIter(int key)
    {
        if (!m_root) {
            m_root = new Node(key);
            return;
        }
        
        Node *current = m_root;
        while (true) {
            if (key < current->key()) {
                if (!current->leftChild()) {
                    current->setLeftChild(new Node(key));
                    break;
                }
                current = current->leftChild();
            } else {
                if (!current->rightChild()) {
                    current->setRightChild(new Node(key));
                    break;
                }
                current = current->rightChild();
            }
        }
    }

    Node *findNodeIter(int key) const
    {
        Node *current = m_root;
        while (current) {
            if (current->key() == key)
                return current;
            if (key < current->key())
                current = current->leftChild();
            else
                current = current->rightChild();
        }
        return nullptr;
    }

    Node *minNode(Node *root) const
    {
        if (!root) return nullptr;
        while (root->m_leftChild) root = root->m_leftChild;
        return root;
    }

    Node *maxNode(Node *root) const
    {
        if (!root) return nullptr;
        while (root->m_rightChild) root = root->m_rightChild;
        return root;
    }
  
    Node *removeNode(Node *node)
    {
        // Случай 1: Узел-лист
        if (!node->leftChild() && !node->rightChild()) {
            delete node;
            return nullptr;
        }
        // Случай 2а: Есть только левый потомок
        if (!node->rightChild()) {
            Node *rep = node->leftChild();
            delete node;
            return rep;
        }
        // Случай 2б: Есть только правый потомок
        if (!node->leftChild()) {
            Node *rep = node->rightChild();
            delete node;
            return rep;
        }
        // Случай 3: Есть оба потомка (Алгоритм со Слайда 19)
        Node *rn = node->rightChild();
        if (!rn->leftChild()) {
            rn->setLeftChild(node->leftChild());
            delete node;
            return rn;
        }
        Node *rp = rn;
        rn = rn->leftChild();
        while (rn->leftChild()) {
            rp = rn;
            rn = rn->leftChild();
        }
        rp->setLeftChild(rn->rightChild());   
        rn->setLeftChild (node->leftChild()); 
        rn->setRightChild(node->rightChild());
        delete node;
        return rn;
    }

    Node *removeNode(Node *root, int key, bool &found)
    {
        if (!root) { found = false; return nullptr; }
        if (key < root->key()) {
            root->setLeftChild(removeNode(root->leftChild(), key, found));
        } else if (key > root->key()) {
            root->setRightChild(removeNode(root->rightChild(), key, found));
        } else {
            found = true;
            root = removeNode(root);
        }
        return root;
    }

    Node *copySubtree(const Node *src)
    {
        if (!src) return nullptr;
        Node *copy = new Node(src->key());
        copy->setLeftChild (copySubtree(src->leftChild()));
        copy->setRightChild(copySubtree(src->rightChild()));
        return copy;
    }

    void deleteSubtree(Node *root)
    {
        if (!root) return;
        deleteSubtree(root->leftChild());
        deleteSubtree(root->rightChild());
        delete root;
    }

    // Рекурсивный подсчет узлов для выделения памяти под ручную очередь BFS
    int countNodes(Node *root) const
    {
        if (!root) return 0;
        return 1 + countNodes(root->leftChild()) + countNodes(root->rightChild());
    }

    // Честный КЛП (Pre-order) обход: сначала Корень, потом Левый, потом Правый
    void nlrPrint(Node *root, std::ostream &out) const
    {
        if (!root) return;
        out << root->key() << ' '; 
        nlrPrint(root->leftChild(), out);
        nlrPrint(root->rightChild(), out);
    }

    void printHorizontal(Node *root, int marginLeft, int levelSpacing, std::ostream &out) const
    {
        if (!root) return;
        printHorizontal(root->rightChild(), marginLeft + levelSpacing, levelSpacing, out);
        out << std::string(marginLeft, ' ') << root->key() << '\n';
        printHorizontal(root->leftChild(),  marginLeft + levelSpacing, levelSpacing, out);
    }

public:
    void addNode(int key)
    {
        addNodeIter(key);
    }
    
    Node *findNode(int key) const
    {
        return findNodeIter(key);
    }
   
    bool removeNode(int key)
    {
        bool found = false;
        m_root = removeNode(m_root, key, found);
        return found;
    }

    
    int min() const
    {
        Node *n = minNode(m_root);
        if (!n) {
            cerr << "Error: Tree is empty" << endl;
            return -1;
        }
        return n->key();
    }

    
    int max() const
    {
        Node *n = maxNode(m_root);
        if (!n) {
            cerr << "Error: Tree is empty" << endl;
            return -1;
        }
        return n->key();
    }

    bool empty() const { return m_root == nullptr; }

    // Обход в ширину (BFS) по уровням — без использования контейнеров STL
    void bfsPrint(std::ostream &out = std::cout) const
    {
        if (!m_root) return;

        // Вычисляем размер дерева для создания точного плоского массива-очереди
        int totalNodes = countNodes(m_root);
        Node** queueArray = new Node*[totalNodes];
        
        int head = 0; // Индекс извлечения (чтения)
        int tail = 0; // Индекс вставки (записи)

        // Помещаем корень в очередь
        queueArray[tail++] = m_root;

        while (head < tail) {
            Node *node = queueArray[head++]; // Извлекаем элемент
            out << node->key() << ' ';

            if (node->leftChild())  queueArray[tail++] = node->leftChild();
            if (node->rightChild()) queueArray[tail++] = node->rightChild();
        }
        out << '\n';

        delete[] queueArray; // Освобождаем динамическую память массива
    }

    // Вывод КЛП — pre-order
    void nlrPrint(std::ostream &out = std::cout) const
    {
        nlrPrint(m_root, out);
        out << '\n';
    }

    // Горизонтальный вывод дерева
    void printHorizontal(std::ostream &out = std::cout) const
    {
        printHorizontal(m_root, 0, 4, out);
    }
};

int main()
{

    SearchTree tree = {50, 20, 70, 10, 30, 60, 90, 5, 15, 25, 40, 35, 45, 40, 55, 65, 80, 100, 57};

    cout << "Horizontal view (root on the left):\n";
    tree.printHorizontal();

    cout << "\nNLR (pre-order):  ";
    tree.nlrPrint();
    cout << "Level-order (BFS): ";
    tree.bfsPrint();

    cout << "\n--- Search ---\n";
    cout << "findNode(40): " << (tree.findNode(40) ? "found" : "not found") << '\n';
    cout << "findNode(99): " << (tree.findNode(99) ? "found" : "not found") << '\n';

    cout << "\n--- Minimum / Maximum ---\n";
    cout << "min = " << tree.min() << ",  max = " << tree.max() << '\n';

    cout << "\n--- Interactive Deletion ---\n";
    cout << "Enter keys to delete (enter -1 to stop):\n";
    int key;
    while (cin >> key && key != -1) {
        if (tree.removeNode(key)) {
            cout << "Deleted " << key << ". Tree:\n"; 
            tree.printHorizontal();
        } else {
            cout << "Key " << key << " not found in tree.\n";
        }
    }

    cout << "\n--- Final state ---\n";
    cout << "Final tree (NLR):  ";
    tree.nlrPrint();
    cout << "Horizontal view:\n";
    tree.printHorizontal();

    cout << "\n--- Copy constructor ---\n";
    SearchTree copy(tree);
    int n = 0;
    cout << "Enter a key to add to the copy: ";
    cin >> n;
    copy.addNode(n);

    cout << "Copy (+ " << n << "):     "; copy.nlrPrint();
    cout << "Original:        "; tree.nlrPrint();
    cout << "\nHorizontal view after copy:\n";
    copy.printHorizontal();
    

    cout << "--- Assignment operator = ---\n";
    SearchTree assigned;
    assigned = tree;
    int m = 0;
    cout << "Enter a key to add to the assigned tree: ";
    cin >> m;
    assigned.addNode(m);
    cout << "Assigned (+ " << m << "):  "; assigned.nlrPrint();
    cout << "Original:        "; tree.nlrPrint();
    cout << "\nHorizontal view after assignment:\n";
    assigned.printHorizontal();

    return 0;
}