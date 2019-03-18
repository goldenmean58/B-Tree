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
    KeyValue<int,int> *kv;
    srand((unsigned int)time(NULL));
    cout<<"1.Insert key and data."<<endl;
    cout<<"2.Remove key and data."<<endl;
    cout<<"3.Find data by key."<<endl;
    cout<<"4.Randomization insert test."<<endl;
    cout<<"Choice:";
    while(cin>>choice){
        switch(choice){
            case 1:
                cout<<"key:";
                cin>>key;
                cout<<"data:";
                cin>>data;
                if(bt.insert(key,data))
                    cout<<"Insert successfully!"<<endl;
                else
                    cout<<"Failed to insert!"<<endl;
                break;
            case 2:
                cout<<"key:";
                cin>>key;
                if(bt.erase(key))
                    cout<<"Remove successfully!"<<endl;
                else
                    cout<<"Failed to remove!"<<endl;
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
            case 4:
                cout<<"The number of keys:";
                cin>>keyNum;
                bt.clear();
                while(keyNum>0){
                    key=rand()%(keyNum*10);
                    data=key;
                    if(bt.insert(key,data)){
                        cout<<"Insert key:"<<key<<endl;
                        bt.printTree();
                        keyNum--;
                    }
                }
                break;
            default:
                break;
        }
        bt.printTree();
    }
    return 0;
}
