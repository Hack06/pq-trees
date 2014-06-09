
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class PQTree: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Note: the leaflist list is referenced by other variables in the program. list nodes should
 * have the same ref throughout the program until they are destroyed
 * however if for some reason thier addr changes this would be a problem
 * but i really don't think this would happen
 */

#include "PQTree.h"
static bool follow = false; //use this to find bugs. prints out function names when a function is executed

bool contains(std::vector<int> vec, int v);

PQTree::PQTree()
{
    root = NULL;
}

PQTree::PQTree(std::vector<int> leaves)
{
    root = new PQnode(leaves, leaflist);
}

PQTree::PQTree(std::string const expr)
{
    size_t i=0;
    if(PQnode *tmp = dynamic_cast<PQnode*>(build_from_expr(expr, i)))
    {
        root = tmp;
    }else
    {
        root = NULL;
        fprintf(stderr, "cannot initialize a pq tree with an invalid expression\n");
        exit(1);
    }
}

PQTree::~PQTree()
{
    delete root;
    root = NULL;
}

//iterates recusively through the tree and prints out leaves and nodes
void PQTree::print()
{
    if(root==NULL)
    {
        printf("Empty tree\n");
    }else
    {
        root->print();
    }
}

//use for planatiry testing
//input vector of ints represents the leaves of the new universal tree
bool PQTree::reduce_and_replace(int v, std::vector<int> tree_in)
{
    if(follow){ printf("PQTree::reduce_and_replace(int value, std::vector<int> tree_in)\n"); }
    
    //put the value inside a vector
    std::vector<int> values;
    values.push_back(v);
    
    PQnode* subroot = reduce(values); //pertinent subroot
    if(subroot==NULL){ return false; }
    
    Node *child;
    if(tree_in.size()==1)
    {
        child = new Leaf(tree_in[0], leaflist);
    }else
    {
        child = new PQnode(tree_in, leaflist);
    }
    if(!replace_full_with(child)){ return false; }
    
    subroot->unmark();
    
    return true;
}

PQnode* PQTree::reduce(std::vector<int> values)
{
    if(follow){ printf("PQTree::reduce(int value)\n"); }
    
    PQnode* subroot = mark(values); //pertinent subroot
    if(follow){ printf("PQTree::reduce(int value). marked as "); print_expression(true); }
    if(subroot!=NULL)
    {
        if(subroot->reduce())
        {
            return subroot;
        }
    }else
    {
        fprintf(stderr, "pertinent subroot not found, could be invalid leaf values\n");
    }
    return NULL;
}

bool PQTree::set_consecutive(std::vector<int> values)
{
    if(follow){ printf("PQTree::set_consecutive(std::vector<int> values)\n"); }
    PQnode *subroot = reduce(values);
    if(subroot==NULL){ return false; }
    
    subroot->unmark();
    return true;
}

bool PQTree::replace_full_with(Node *child)
{
    if(follow){ printf("PQTree::replace_full_with(Node *child)\n"); }
    
    Node* parent = NULL;
    if(child==NULL){ return false; }
    std::list<Leaf*> fulls = get_pertinent();
    
    for(std::list<Leaf*>::iterator it=fulls.begin(); it!=fulls.end(); ++it)
    {
        if(parent)
        {
            if((*it)->get_parent()!=parent)
            {
                fprintf(stderr, "not all full nodes have the same parent\n");
                return false;
            }
        }else
        {
            parent = (*it)->get_parent();
        }
    }
    
    if(PQnode* temp = dynamic_cast<PQnode*>(parent))
    {
        if(!(temp->condense_and_replace(child)))
        {
            fprintf(stderr, "condense failed\n");
            return false;
        }
        if(temp->count_children()<3)
        {
            temp->set_type(pnode);
        }
        
    }
    return true;
}

/*******************************************************************************
 * Function PQTree::mark(int value)
 * purpose: marks the pertinent subtree
 * returns the subroot of the pertinent subtree, otherwise NULL if an error occurs
 ********************************************************************************/
PQnode* PQTree::mark(std::vector<int> v)
{   
    if(follow){ printf("PQTree::mark(int v)\n"); }
    
    std::list<Leaf*> fulls = mark_pertinent(v); //mark the full leaves based on the input values
    std::list<PQnode*> partials; 
    
    for(std::list<Leaf*>::iterator k = fulls.begin(); k!=fulls.end(); ++k)
    {
        PQnode *p = dynamic_cast<PQnode*>((*k)->get_parent());
        if(p==NULL) //this is the parent we want to add to the list of potential partials
        {
            return NULL;
        }
        
        //add it into the partials list by inserting it at the correct position based on decreasing depth
        add_unique_by_depth(p, partials);
    }
    fulls.clear();
    
    //at this point we have a list of potential partials sorted by depth with no duplicates
    //now we need to mark these nodes.... and then their parents until there is only one node left in the partials list
    while(partials.size()>1)
    {
        PQnode *curr = partials.front();//always deal with the front element first
        curr->mark(); //mark the node
        PQnode *p = (PQnode*)curr->get_parent(); //any parent in the tree will never be a leaf since they cannot have children. therefore this casting is safe
        partials.pop_front(); //remove the curr node and destroy the reference
        
        add_unique_by_depth(p, partials);
    }
    if(!partials.empty())
    {
        partials.front()->mark();
        return partials.front();
    }
    return NULL;
    
}

//adds only unique nodes
//adds the node so that the partials list maintains the by decreasing depth property
void PQTree::add_unique_by_depth(PQnode *p, std::list<PQnode*> &partials){
    if(partials.empty())
    {
        partials.push_back(p);
    }else
    {
        for(std::list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it) //iterates through our list of partials
        { 
            if((*it)->get_depth()>=p->get_depth())
            {
                if(p==(*it)){ break; }
            }else
            {
                partials.insert(it, p);
            }
        }
    }
}

//prints an expression that represents the current tree. {} are p nodes, [] are qnodes
std::string PQTree::print_expression(bool mark/*false*/)
{
    std::string result = root->print_expression(mark);
    return result;
}


//takes in a string expression of a pq-tree and builds the corresponding tree
Node* PQTree::build_from_expr(std::string const expr, size_t &i)
{
    if(follow){ printf("PQTree::build_from_expr(std::string const expr, int &i)\n"); }
    
    int state = 0;
    bool reading = true;
    bool isqnode = false;
    PQnode *rt = NULL;
    Leaf *lf = NULL;
    
    while(reading&&i<expr.length())
    {
        switch(state)
        {
            case 0:
                if(isspace(expr[i])) //ignore whitespace
                {
                    ++i;
                }else if(expr[i]=='{'||expr[i]=='[')
                {
                    if(expr[i]=='[')
                    {
                        isqnode = true;
                    }
                    state = 1; ++i; rt = new PQnode();
                }else
                {
                    return NULL;
                }
                break;
            case 1: //started a node. linking children
                if(isspace(expr[i])) //ignore whitespace
                { 
                    ++i;
                }else if(expr[i]=='{'||expr[i]=='[') //start the next pnode
                { 
                    Node *child = build_from_expr(expr, i);
                    rt->link_child(child);
                    ++i;
                }else if(expr[i]=='}'||expr[i]==']')
                {
                    reading = false;
                }else if(isdigit(expr[i]))
                {
                    //get the number terminated by a comma
                    std::string num = "";
                    while(expr[i]!=' '&&expr[i]!=']'&&expr[i]!='}'&&i<expr.length())
                    {
                        num += expr[i++];
                    }
                    lf = new Leaf(rt, atoi(num.c_str()), leaflist);
                    rt->link_child(lf);
                }
                break;
            default:
                return NULL;
        }
    }
    if(isqnode&&rt->count_children()>2)
    {
        rt->set_type(qnode);
    }else if(rt->count_children()==1)
    {
        return lf;
    }
    return rt;
}


std::list<Leaf*> PQTree::mark_pertinent(std::vector<int> vec)
{
    if(follow){ printf("PQTree::mark_pertinent(std::vector<int> vec)\n"); }
    std::list<Leaf*> fulls;
    std::list<Leaf*>::iterator it=leaflist.begin();
    while(it!=leaflist.end())
    {
        if((*it)==NULL)
        {
            it = leaflist.erase(it);
        }else
        {
            if(contains(vec, (*it)->get_value())){
                (*it)->mark();
                fulls.push_back((*it));
            }
            ++it;
        }
    }
    return fulls;
}

std::list<Leaf*> PQTree::get_pertinent()
{
    if(follow){ printf("PQTree::get_pertinent()\n"); }
    
    std::list<Leaf*> fulls;
    std::list<Leaf*>::iterator it=leaflist.begin();
    while(it!=leaflist.end())
    {
        if((*it)==NULL)
        {
            it = leaflist.erase(it);
        }else
        {
            if((*it)->get_mark()==full)
            {
                fulls.push_back((*it)); 
            }
            ++it;
        }
    }
    return fulls;
}

size_t PQTree::get_leaflist_size()
{
    return leaflist.size();
}


bool custom::compare(std::string s1, std::string s2)
{
    auto a = s1.begin();
    auto b = s2.begin();
    while(a!=s1.end())
    {
        if((*a)!=' ') //ignore whitespace in s1
        {
            while(b!=s2.end())
            {
                if((*b)!=' ') //ignore whitespace in s2
                {
                    if((*b)==(*a))
                    {
                        ++b;
                        break;
                    }else
                    {
                        return false;
                    }
                }
                ++b;
            }
        }
        ++a;
    }
    
    return true;
}









