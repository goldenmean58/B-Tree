#include "BTree.h"
#include <string.h>

class TestClass{
public:
    int id;
    char name[20];
    int age;
    void print(){
        cout<<"Id:"<<id<<'\t'<<"Name:"<<name<<'\t'<<"Age:"<<age<<endl;
    }
    TestClass(){}
    TestClass(int id, const char* name,int age):id(id),age(age){
        strcpy(this->name,name);
    }
};

void printMenu(){
    cout<<"Menu:"<<endl;
    cout<<"1.Add a new person"<<endl;
    cout<<"2.Delete a person"<<endl;
    cout<<"3.Find a person"<<endl;
    cout<<"4.Print BTree with keys"<<endl;
    cout<<"5.Exit"<<endl;
    cout<<"Choice:"<<endl;
}

int main()
{
    int choice;
    char name[20];
    int age;
    int key;
    BTreeMap<int,TestClass> bt(4);
    TestClass *tc=NULL;
    KeyValue<int,TestClass> *kv=NULL;
    while(1){
        printMenu();
        cin>>choice;
        switch(choice){
            case 1: //add
                cout<<"id:"; 
                cin>>key;
                cout<<"name:"; 
                cin>>name;
                cout<<"age:"; 
                cin>>age;
                tc=new TestClass(key,name,age);
                if(bt.insert(key,*tc)){
                    cout<<"ok"<<endl;
                }
                delete tc;
                break;
            case 2: //delete
                cout<<"id:"; 
                cin>>key;
                if(bt.erase(key)){
                    cout<<"ok"<<endl;
                }
                break;
            case 3: //find
                cout<<"id:"; 
                cin>>key;
                kv=bt.find(key);
                if(kv){
                    kv->value.print();
                    kv->value.age=100;
                }else{
                    cout<<"No such a person"<<endl;
                }
                break;
            case 4: //print
                bt.printTree();
                break;
            case 5:
                return 0;
                break;
        }
    }
    return 0;
}
