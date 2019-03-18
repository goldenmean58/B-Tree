#ifndef BTREE_H
#define BTREE_H
#include "BNode.h"

template<class K, class V>
class BTreeMap{
public:
    BTreeMap(int M);
    BNode<K,V> *getRoot();
    bool isEmpty();
    bool insert(K,V);
    bool insert(const KeyValue<K,V>);
    bool erase(K); //删除
    void printTree();
    KeyValue<K,V> *find(K key);
    void clear();
    int size();
    ~BTreeMap();
private:
    BNode<K,V> *_root;
    int _M; //阶数
    int _size;
    void _mergeNode(BNode<K,V>*); //合并结点
    void _reBalance(BNode<K,V> *leaf); //重新平衡且从叶子?结点向上到根
    void _splitNode(BNode<K,V>*); //分裂结点
    void _removeNode(BNode<K,V>*); //直接删除结点
    bool insert(BNode<K,V>*,K,V); //插入
    bool insert(BNode<K,V>*,const KeyValue<K,V>); 
    void _printTree(BNode<K,V>*);
    bool _erase(BNode<K,V>*,K);
    BNode<K,V> *_find(BNode<K,V>*,K key);
};

#include "BTree.cpp"
#endif
