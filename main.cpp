#include "BTree.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int M;
    cout<<"Please enter the max degree(>=3):";
    cin>>M;
    M=M<3?3:M;
    BTreeMap<int,int> bt(M);
    int choice;
    int key, data;
    int keyNum=0;
    vector<int> keys;
    KeyValue<int,int> *kv;
    unsigned int seed=0;
    cout<<"Please enter the random seed(0 for random):";
    cin>>seed;
    if(seed==0)
        seed=(unsigned int)time(NULL);
    srand(seed);
    cout<<"Current Seed: "<<seed<<endl;
    cout<<"1.Insert key and data."<<endl;
    cout<<"2.Remove key and data."<<endl;
    cout<<"3.Find data by key."<<endl;
    cout<<"4.Randomization insert test."<<endl;
    cout<<"5.Randomization delete test."<<endl;
    cout<<"6.Reset the max degree."<<endl;
    cout<<"7.Print BTree."<<endl;
    cout<<"8.Clean BTree."<<endl;
    cout<<"Choice:";
    while(cin>>choice){
        switch(choice){
            case 1:
                cout<<"key:";
                cin>>key;
                cout<<"data:";
                cin>>data;
                if(bt.insert(key,data)){
                    keys.push_back(key);
                    cout<<"Insert successfully!"<<endl;
                }else
                    cout<<"Failed to insert!"<<endl;
                bt.printTree();
                break;
            case 2:
                cout<<"key:";
                cin>>key;
                if(bt.erase(key)){
                    for(typename std::vector<int>::iterator it_key=keys.begin();it_key!=keys.end();it_key++){
                        if(*it_key==keys[key]){
                            keys.erase(it_key);
                            break;
                        }
                    }
                    cout<<"Remove successfully!"<<endl;
                }else
                    cout<<"Failed to remove!"<<endl;
                bt.printTree();
                break;
            case 3:
                cout<<"key:";
                cin>>key;
                kv=bt.find(key);
                if(kv){
                    cout<<"data:"<<kv->value<<endl;
                    delete kv;
                }
                else
                    cout<<"No such a key!"<<endl;
                break;
            case 5:
                cout<<"The number of keys to delete:";
                cin>>keyNum;
                keyNum=keyNum<(int)keys.size()?keyNum:keys.size();
                while(keyNum>0){
                    key=rand()%(keys.size());
                    if(bt.erase(keys[key])){
                        cout<<"Delete key:"<<keys[key]<<endl;
                        for(typename std::vector<int>::iterator it_key=keys.begin();it_key!=keys.end();it_key++){
                            if(*it_key==keys[key]){
                                keys.erase(it_key);
                                break;
                            }
                        }
                        bt.printTree();
                        keyNum--;
                    }
                }
                break;
            case 4:
                cout<<"The number of keys:";
                cin>>keyNum;
                while(keyNum>0){
                    key=rand()%(keyNum*10);
                    data=key;
                    if(bt.insert(key,data)){
                        cout<<"Insert key:"<<key<<endl;
                        keys.push_back(key);
                        bt.printTree();
                        keyNum--;
                    }
                }
                break;
            case 6:
                break;
            case 7:
                bt.printTree();
                break;
            case 8:
                bt.clear();
                keys.clear();
                break;
            default:
                break;
        }
    }
    return 0;
}
