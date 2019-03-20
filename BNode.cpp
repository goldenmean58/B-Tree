#include "BNode.h"

template<class K, class V>
bool BNode<K,V>::isHasKey(K){
    return true;
}

template<class K, class V>
bool BNode<K,V>::isLeaf(){
    if(this->childNode.size()==0) return true;
    return this->childNode.front()==NULL;
}

template<class K, class V>
V BNode<K,V>::getValueOfKey(K key){
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<V>::iterator it_data=this->datas.begin();
    for(;it_key!=this->keys.end();it_key++,it_data++){
        if(*it_key==key){
            return *it_data;
        }
    }
    return NULL;
}

template<class K, class V>
BNode<K,V>::BNode(BNode<K,V> * parentNode):parentNode(parentNode){

}

template<class K, class V>
void BNode<K,V>::insert(K key, V value){ //结点插入kv
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<V>::iterator it_data=this->datas.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_data++,it_child++){
        if(*it_key>key){
            break;
        }
    }
    keys.insert(it_key,key);
    datas.insert(it_data,value);
}


template<class K, class V>
void BNode<K,V>::create(K key, V value){ //结点插入kv
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<V>::iterator it_data=this->datas.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_data++,it_child++){
        if(*it_key>key){
            break;
        }
    }
    if(this->keys.size()==0){
        this->keys.push_back(key);
        this->datas.push_back(value);
        this->childNode.push_back(NULL);
        this->childNode.push_back(NULL);
    }else{
        keys.insert(it_key,key);
        datas.insert(it_data,value);
        childNode.insert(it_child,NULL);
        return;
    }
}

template<class K, class V>
BNode<K,V> *BNode<K,V>::getPreNode(K key){
    if(this->isLeaf()) return NULL;
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_child++){
        if(*it_key==key){
            return *it_child;
        }
    }
    return NULL;
}

template<class K, class V>
BNode<K,V> *BNode<K,V>::getNextNode(K key){
    if(this->isLeaf()) return NULL;
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_child++){
        if(*it_key==key){
            it_child++;
            return *it_child;
        }
    }
    return NULL;
}

template<class K, class V>
bool BNode<K,V>::setPreNode(K key, BNode<K,V> *newSubNode){
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_child++){
        if(*it_key==key){
            *it_child=newSubNode;
            return true;
        }
    }
    return false;
}

template<class K, class V>
bool BNode<K,V>::setNextNode(K key, BNode<K,V> *newSubNode){
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_child++){
        if(*it_key==key){
            it_child++;
            *it_child=newSubNode;
            return true;
        }
    }
    return false;
}

template<class K, class V>
bool BNode<K,V>::removeKeyAndLeftChild(K key){
    //移除结点key data 以及childNode中对应的左孩子
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<V>::iterator it_data=this->datas.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_data++,it_child++){
        if(*it_key==key){
            this->keys.erase(it_key);
            this->datas.erase(it_data);
            this->childNode.erase(it_child);
            return true;
        }
    }
    return false;
}

template<class K, class V>
bool BNode<K,V>::removeKeyAndRightChild(K key){
    //移除结点key data 以及childNode中对应的左孩子
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<V>::iterator it_data=this->datas.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->childNode.begin();
    for(;it_key!=this->keys.end();it_key++,it_data++,it_child++){
        if(*it_key==key){
            this->keys.erase(it_key);
            this->datas.erase(it_data);
            it_child++;
            this->childNode.erase(it_child);
            return true;
        }
    }
    return false;
}

template<class K, class V>
bool BNode<K,V>::replace(K old_key, K new_key,V new_data){
    typename std::list<K>::iterator it_key=this->keys.begin();
    typename std::list<V>::iterator it_data=this->datas.begin();
    for(;it_key!=this->keys.end();it_key++,it_data++){
        if(*it_key==old_key){
            *it_key=new_key;
            *it_data=new_data;
            return true;
        }
    }
    return false;

}

template<class K, class V>
BNode<K,V>* BNode<K,V>::getRightSibling(){
    if(this->parentNode==NULL) return NULL;
    typename std::list<BNode<K,V>*>::iterator it_child=this->parentNode->childNode.begin();
    for(;it_child!=this->parentNode->childNode.end();it_child++){
        if(*it_child==this){
            it_child++;
            if(it_child==this->parentNode->childNode.end()) return NULL;
            else return *it_child;
        }
    }
    return NULL;
}


template<class K, class V>
BNode<K,V> *BNode<K,V>::getLeftSibling(){
    if(this->parentNode==NULL) return NULL;
    if(*(this->parentNode->childNode.begin())==this) return NULL;
    typename std::list<BNode<K,V>*>::iterator it_child=this->parentNode->childNode.begin();
    for(;it_child!=this->parentNode->childNode.end();it_child++){
        if(*it_child==this){
            if(it_child==this->parentNode->childNode.begin()) return NULL;
            it_child--;
            return *it_child;
        }
    }
    return NULL;
}

template<class K, class V>
K BNode<K,V>::getLeftParentKey(){
    if(this->parentNode==NULL) return NULL;
    typename std::list<K>::iterator it_key=this->parentNode->keys.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->parentNode->childNode.begin();
    for(;it_child!=this->parentNode->childNode.end();it_key++,it_child++){
        if(*it_child==this){
            if(it_child==this->parentNode->childNode.begin()) return NULL;
            it_key--;
            return *it_key;
        }
    }
    return NULL;
}

template<class K, class V>
K BNode<K,V>::getRightParentKey(){
    if(this->parentNode==NULL) return NULL;
    typename std::list<K>::iterator it_key=this->parentNode->keys.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=this->parentNode->childNode.begin();
    for(;it_child!=this->parentNode->childNode.end();it_key++,it_child++){
        if(*it_child==this){
            if(it_key==this->parentNode->keys.end()) return NULL;
            return *it_key;
        }
    }
    return NULL;
}
