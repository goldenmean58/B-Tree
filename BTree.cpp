#include "BTree.h"

template<class K, class V>
BNode<K,V> *BTreeMap<K,V>::getRoot(){
    return this->_root;
}

template<class K, class V>
BTreeMap<K,V>::BTreeMap(int M):_M(M),_root(NULL),_size(0){
    _root = new BNode<K,V>(NULL);
}

template<class K, class V>
bool BTreeMap<K,V>::isEmpty(){
    return _root==NULL;
}

template<class K, class V>
int BTreeMap<K,V>::size(){
    return _size;
}

template<class K, class V>
void BTreeMap<K,V>::clear(){
    //TODO
    _removeNode(_root);
    delete _root;
    _root = new BNode<K,V>(NULL);
    _size=0;
}

template<class K, class V>
bool BTreeMap<K,V>::insert(const KeyValue<K,V> kv){
    return this->insert(kv.key,kv.value);
}

template<class K, class V>
bool BTreeMap<K,V>::insert(K key, V value){
    return this->insert(_root,key,value);
}

template<class K, class V>
bool BTreeMap<K,V>::insert(BNode<K,V> *root, const KeyValue<K,V> kv){
    return this->insert(root,kv.key,kv.value);
}

template<class K, class V>
bool BTreeMap<K,V>::insert(BNode<K,V> *root, K key, V value){
    //先找到应该插到哪个叶子结点
    if(!root) return false;
    typename std::list<K>::iterator it_key=root->keys.begin();
    typename std::list<V>::iterator it_data=root->datas.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=root->childNode.begin();
    for(;it_key!=root->keys.end();it_key++,it_data++,it_child++){
        if(*it_key==key) return false; //key冲突
        if(*it_key>key){
            break;
        }
    }
    if(root->isLeaf()){
        root->create(key,value);
        _splitNode(root);
        return true;
    }else{
        return insert(*it_child,key,value);
    }
}

template<class K, class V>
bool BTreeMap<K,V>::erase(K key){
    return _erase(_root,key);
}

template<class K, class V>
bool BTreeMap<K,V>::_erase(BNode<K,V> *root, K key){
    if(!root) return false;
    //先找到key的位置
    BNode<K,V> *keyNode=_find(root,key);
    if(keyNode==NULL) return false;
    if(keyNode->isLeaf()){ //leaf node
        keyNode->removeKeyAndLeftChild(key); //直接移除
        //检测delete后是否下溢
        if(keyNode->keys.size()<ceil(_M/2.0)-1)
            _reBalance(keyNode);
    }else{ //internal node
        BNode<K,V> *subTree;
        K candidateKey;
        V candidateData;
        //找直接前驱替换并删除直接前驱
        BNode<K,V> *leftSubNode=keyNode->getPreNode(key);
        int tmp=keyNode->childNode.size();
        subTree=leftSubNode;
        while(!subTree->isLeaf()){
            subTree=subTree->childNode.back();
        }
        candidateKey=subTree->keys.back();
        candidateData=subTree->getValueOfKey(candidateKey);
        keyNode->replace(key,candidateKey,candidateData);
        subTree->removeKeyAndLeftChild(candidateKey);
        //上面一步可能会导致子树结点的下溢
        if(subTree->keys.size()<ceil(_M/2.0)-1)
            _reBalance(subTree);
    }
    return true;
}

template<class K, class V>
KeyValue<K,V> *BTreeMap<K,V>::find(K key){
    BNode<K,V> *node=_find(_root,key);
    if(!node) return NULL;
    KeyValue<K,V> *ret=new KeyValue<K,V>(key,node->getValueOfKey(key));
    return ret;
}

template<class K, class V>
BNode<K,V> *BTreeMap<K,V>::_find(BNode<K,V> *root, K key){
    if(!root) return NULL;
    typename std::list<K>::iterator it_key=root->keys.begin();
    typename std::list<V>::iterator it_data=root->datas.begin();
    typename std::list<BNode<K,V>*>::iterator it_child=root->childNode.begin();
    for(;it_key!=root->keys.end();it_key++,it_data++,it_child++){
        if(*it_key==key){
            return root;
        }
        if(*it_key>key) return _find(*it_child,key);
    }
    return _find(*it_child,key);
}

template<class K, class V>
BTreeMap<K,V>::~BTreeMap(){
}

template<class K, class V>
void BTreeMap<K,V>::_mergeNode(BNode<K,V> *node){

}

template<class K, class V>
void BTreeMap<K,V>::_reBalance(BNode<K,V> *node){
    //case 1:If there are keys that can be borrowed from its sibling then rotation
    //case 2:If there has been no keys to be borrowed from left or right sibling node then merge node, its sibling and its parent key
    //right sibling
    BNode<K,V> *rightSiblingNode=node->getRightSibling();
    BNode<K,V> *leftSiblingNode=node->getLeftSibling();
    BNode<K,V> *siblingNode=NULL;
    K parentKey;
    V parentData;
    if(rightSiblingNode!=NULL && rightSiblingNode->keys.size()>ceil(_M/2.0)-1){
        //borrow from right sibling
        parentKey=node->getRightParentKey();
        parentData=node->parentNode->getValueOfKey(parentKey);
        siblingNode=rightSiblingNode;
        node->insert(parentKey,parentData);
        node->parentNode->replace(parentKey,siblingNode->keys.front(),siblingNode->datas.front());
        node->childNode.insert(node->childNode.end(),siblingNode->childNode.front());
        if(siblingNode->childNode.back()!=NULL) // reset child's parent
            siblingNode->childNode.front()->parentNode=node;
        siblingNode->removeKeyAndLeftChild(siblingNode->keys.front());
    }else if(leftSiblingNode!=NULL && leftSiblingNode->keys.size()>ceil(_M/2.0)-1){
        //borrow from left sibling
        parentKey=node->getLeftParentKey();
        parentData=node->parentNode->getValueOfKey(parentKey);
        siblingNode=leftSiblingNode;
        node->insert(parentKey,parentData);
        node->parentNode->replace(parentKey,siblingNode->keys.back(),siblingNode->datas.back());
        node->childNode.insert(node->childNode.begin(),siblingNode->childNode.back());
        if(siblingNode->childNode.back()!=NULL)
            siblingNode->childNode.back()->parentNode=node;
        siblingNode->removeKeyAndRightChild(siblingNode->keys.back());
    }else{
        //merge
        //move sep to the end of the left node and move all elements in the right node into the left node
        //remove the sep from the parent node and the empty right node
        BNode<K,V> *parentNode=node->parentNode;
        if(parentNode==NULL) return;
        BNode<K,V> *newRoot=_root;
        if(rightSiblingNode==NULL){
            //合并node,左兄弟结点与parent sep
            //move sep to the end of the left node
            leftSiblingNode->keys.push_back(node->getLeftParentKey());
            leftSiblingNode->datas.push_back(parentNode->getValueOfKey(node->getLeftParentKey()));
            //move all elements in the right node into the left node
            leftSiblingNode->keys.insert(leftSiblingNode->keys.end(),node->keys.begin(),node->keys.end());
            leftSiblingNode->datas.insert(leftSiblingNode->datas.end(),node->datas.begin(),node->datas.end());
            leftSiblingNode->childNode.insert(leftSiblingNode->childNode.end(),node->childNode.begin(),node->childNode.end());
            //remove the sep from the parent node and the empty right node
            parentNode->removeKeyAndRightChild(node->getLeftParentKey());
            newRoot=leftSiblingNode;
            //reset children's parentNode to the new node
            for(auto child:newRoot->childNode){
                if(child!=NULL)
                    child->parentNode=newRoot;
            }
            delete node;
        }else{
            //合并node,右兄弟结点与parent sep
            node->keys.push_back(node->getRightParentKey());
            node->datas.push_back(parentNode->getValueOfKey(node->getRightParentKey()));
            node->keys.insert(node->keys.end(),rightSiblingNode->keys.begin(),rightSiblingNode->keys.end());
            node->datas.insert(node->datas.end(),rightSiblingNode->datas.begin(),rightSiblingNode->datas.end());
            node->childNode.insert(node->childNode.end(),rightSiblingNode->childNode.begin(),rightSiblingNode->childNode.end());
            parentNode->removeKeyAndRightChild(node->getRightParentKey());
            newRoot=node;
            for(auto child:newRoot->childNode){
                if(child!=NULL)
                   child->parentNode=newRoot;
            }
            delete rightSiblingNode;
        }
        if(parentNode==_root &&  parentNode->keys.size()==0){ //根为空则合并结点作为新根
            delete _root;
            _root=newRoot;
            _root->parentNode=NULL;
            return;
        }
        if(parentNode->keys.size()<ceil(_M/2.0)-1)
            _reBalance(parentNode);
    }
}

template<class K, class V>
void BTreeMap<K,V>::_removeNode(BNode<K,V> *node){
    //递归removeNode
    return;
}

template<class K, class V>
void BTreeMap<K,V>::_splitNode(BNode<K,V> *node){
    if(node->keys.size()<=_M-1){ //无需分裂
        return;
    }
    typename std::list<K>::iterator it_key_sep=node->keys.begin();
    typename std::list<K>::iterator it_key_all=node->keys.begin();
    typename std::list<V>::iterator it_data_sep=node->datas.begin();
    typename std::list<BNode<K,V>* >::iterator it_child_sep=node->childNode.begin();
    if(node->parentNode==NULL){ //根的分裂
        BNode<K,V> *newRoot=new BNode<K,V>(NULL);
        _root=newRoot;
        node->parentNode=_root;
    }
    //用sep分裂当前结点为两个新结点A,B
    BNode<K,V> *pNewNodeA,*pNewNodeB;
    pNewNodeA=new BNode<K,V>(node->parentNode);
    pNewNodeB=new BNode<K,V>(node->parentNode); 
    while(it_key_all!=node->keys.end()){ //找到seprator key,data
        it_key_all++;
        if(it_key_all==node->keys.end()) break;
        it_key_sep++;
        if(*it_child_sep!=NULL)
            (*it_child_sep)->parentNode=pNewNodeA;
        it_child_sep++;
        it_data_sep++;
        it_key_all++;
    }
    if(*it_child_sep!=NULL)
        (*it_child_sep)->parentNode=pNewNodeA;
    it_child_sep++;
    //复制原结点key child data到新的两个结点中
    pNewNodeA->keys.insert(pNewNodeA->keys.begin(),node->keys.begin(),it_key_sep);
    pNewNodeA->childNode.insert(pNewNodeA->childNode.begin(),node->childNode.begin(),it_child_sep); 
    pNewNodeA->datas.insert(pNewNodeA->datas.begin(),node->datas.begin(),it_data_sep); 
    it_key_sep++;
    it_data_sep++;
    pNewNodeB->keys.insert(pNewNodeB->keys.begin(),it_key_sep,node->keys.end());
    pNewNodeB->childNode.insert(pNewNodeB->childNode.begin(),it_child_sep,node->childNode.end());
    pNewNodeB->datas.insert(pNewNodeB->datas.begin(),it_data_sep,node->datas.end()); 
    it_key_sep--;
    it_data_sep--;
    //重新设置所有子结点的父结点
    while(it_child_sep!=node->childNode.end() && *it_child_sep!=NULL){
        (*it_child_sep)->parentNode=pNewNodeB;
        it_child_sep++;
    }
    //提sep到父结点
    node->parentNode->create(*it_key_sep,*it_data_sep);
    //重新设置父结点的孩子结点
    node->parentNode->setPreNode(*it_key_sep,pNewNodeA);
    node->parentNode->setNextNode(*it_key_sep,pNewNodeB);
    //删除被分裂的原结点How?
    _removeNode(node);
    //判断分裂父结点
    _splitNode(node->parentNode);
}

template<class K, class V>
void BTreeMap<K,V>::_printTree(BNode<K,V> *root){
    if(!root) return;
    set<BNode<K,V>* >subTree;
    queue<BNode<K,V>* >visitQueue;
    int oldSubTreeNum=1, newSubTreeNum=0;
    BNode<K,V> *p=NULL;
    visitQueue.push(root);
    subTree.insert(root);
    auto pre=p;
    while(!visitQueue.empty()){
        pre=p;
        p=visitQueue.front();
        //print a node's keys
        cout<<"| ";
        for(auto it_key:p->keys){
            cout<<it_key<<" ";
        }
        cout<<"|";
        visitQueue.pop();
        if(!p->isLeaf()){
            for(auto it_child:p->childNode){
                if(subTree.find(it_child)==subTree.end() && it_child!=NULL){
                    newSubTreeNum++;
                    visitQueue.push(it_child);
                    subTree.insert(it_child);
                }
            }
        }
        oldSubTreeNum--;
        if(oldSubTreeNum==0){
            oldSubTreeNum=newSubTreeNum;
            newSubTreeNum=0;
            cout<<endl;
        }
    }
}

template<class K, class V>
void BTreeMap<K,V>::printTree(){
    _printTree(_root);
}
