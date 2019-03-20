all: BTree test

BTree: main.cpp BTree.h BTree.cpp BNode.h BNode.cpp
	g++ main.cpp -o BTree -g

test: test*.cpp BTree.h BTree.cpp BNode.h BNode.cpp
	make test_class_map

test_class_map: test_class_map.cpp BTree.h BTree.cpp BNode.h BNode.cpp
	g++ test_class_map.cpp -o test_class_map -g

clean:
	rm BTree test_class_map
