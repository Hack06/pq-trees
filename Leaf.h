
#ifndef __LEAF_H
#define __LEAF_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"


//dependencies
#include "Node.h"
#include <string>

class Leaf: public Node
{
    private:
        int value;
        Leaf **leaf_list_ptr;
        bool less_than(Node&);
        void sort(){}
        
    public:
        //constructors and destructors
        Leaf(Node*, int, std::list<Leaf*>&);
        Leaf(int, std::list<Leaf*>&);
        virtual ~Leaf();
        
        //testing functions
        void print();
        std::string print_expression(bool m = false);
        
        //update and setters
        void mark();
        void update_depth();
        void unmark();
        
        //getters
        int get_value();
};

#endif
