#ifndef BNODE_H
#define BNODE_H

#include<iostream>
#include<list>
#include<queue>
#include<math.h>
#include<vector>
using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::queue;
using std::vector;

template<class K, class V>
class KeyValue{
public:
    K key;
    V &value;
    KeyValue(K key,V &value):key(key),value(value){}
    KeyValue();
};

template<class K, class V>
class BNode{
public:
    list<K> keys; //结点中关键字
    list<V> datas; //结点内数据
    list<BNode<K,V>*> childNode; //结点内孩子结点指针
    BNode<K,V> *parentNode; //父结点
    BNode(BNode<K,V>*);
    bool isLeaf(); //是否为叶结点
    V& getValueOfKey(K); //取key的数据
    BNode<K,V> *getPreNode(K); //取关键字的左子结点
    BNode<K,V> *getNextNode(K); //取关键字的右子结点
    bool setPreNode(K, BNode<K,V>*); //设置关键字的左子结点
    bool setNextNode(K, BNode<K,V>*); //设置关键字的右子结点
    void insert(K key,V value); //在当前结点插入key与value
    void create(K key,V value); //在当前结点插入key,value与对应位置生成新NULL->childNode
    bool removeKeyAndLeftChild(K key);
    bool removeKeyAndRightChild(K key);
    bool replace(K,K,V);
    BNode<K,V> *getRightSibling();
    BNode<K,V> *getLeftSibling();
    K getLeftParentKey();
    K getRightParentKey();
private:
};

#include "BNode.cpp"
#endif
