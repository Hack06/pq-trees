
#ifndef __NODE_H
#define __NODE_H

//dependencies
#include <iostream>
#include <cstdlib>
#include <list>
#include <string>
#include <vector>

enum nodetype {pnode , qnode, leafnode};
enum marking {empty, partial, full};

class Node{
    protected:
        Node* parent; //pointer to the parent node
        int depth;
        nodetype type;
        marking mark;
    public:
        Node();
        virtual ~Node(){} //destructor. use this to get rid of crap "new"'d inside the constructor
        virtual void print() = 0; //now this is pure virtual
        Node* get_parent();
        int get_depth();
        nodetype get_type();
        marking get_mark();
};

#endif