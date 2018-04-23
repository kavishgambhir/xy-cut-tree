#include <iostream>
#include <vector>
#include "math.h"
#include <string>

using namespace std;

class Node {
    Node *root;
    string label;
    Node *parent;
    vector<Node *> child;


    int treeSize;

public:
    std::vector<Node *> allNodes;

    std::vector<Node *> (*pFunctionInit)();

    Node() {
        this->root = NULL;
    }


    Node(string str) {
        label = str;
    }

    vector<Node *> getkid(Node *n);

    int getHeight(Node *node);

    void setLabel(string s) { label = s; }

    void setAllNodes(std::vector<Node *> v) { allNodes = v; }

    void setSize(int x) { treeSize = x; }

    void setRoot(Node *root) { this->root = root; }

    Node * getParent(Node * node){return node->parent;}
    void setParent(Node *node) {
        parent = node;
    }

    void addKid(Node *node) {
        child.push_back(node);
    }

    string getLabel() {
        return this->label;
    }

    vector<int> findLabel(const string &label);

    Node *getRoot() { return root; }

    int getSize() { return treeSize; }

    void print(Node *node, int num);


};


//manual initialisation of a tree from eg in paper.
vector<Node *> init1() {
    const int n = 19;
    vector<Node *> sample(n);
    sample[0] = new Node("hl");
    sample[0]->setParent(nullptr);
    sample[1] = new Node("hL");
    sample[2] = new Node("vs");
    sample[3] = new Node("hL");
    sample[4] = new Node("vl");
    sample[5] = new Node("T");
    sample[6] = new Node("I");
    sample[7] = new Node("hl");
    sample[8] = new Node("vL");
    sample[9] = new Node("hs");
    sample[10] = new Node("T");
    sample[11] = new Node("hL");
    sample[12] = new Node("hs");
    sample[13] = new Node("I");
    sample[14] = new Node("T");
    sample[15] = new Node("T");
    sample[16] = new Node("I");
    sample[17] = new Node("T");
    sample[18] = new Node("I");
    for (int i = 1; i <= 4; i++) {
        sample[i]->setParent(sample[0]);
        sample[0]->addKid(sample[i]);
    }
    for (int i = 5; i <= 6; i++) {
        sample[i]->setParent(sample[2]);
        sample[2]->addKid(sample[i]);
    }
    for (int i = 7; i <= 9; i++) {
        sample[i]->setParent(sample[4]);
        sample[4]->addKid(sample[i]);
    }
    for (int i = 10; i <= 12; i++) {
        sample[i]->setParent(sample[7]);
        sample[7]->addKid(sample[i]);
    }
    for (int i = 13; i <= 15; i++) {
        sample[i]->setParent(sample[9]);
        sample[9]->addKid(sample[i]);
    }
    for (int i = 16; i <= 18; i++) {
        sample[i]->setParent(sample[12]);
        sample[12]->addKid(sample[i]);
    }

    for (auto it = sample.begin(); it != sample.end(); it++) {
        (*it)->pFunctionInit = &init1;
        (*it)->setRoot(sample[0]);
    }
    // vector<Node *> v=(sample[0]->pFunctionInit)();

    return sample;

}

int Node::getHeight(Node *node) {

    int l = static_cast<int>(node->getkid(node).size());
    if (l == 0) return 0;
    else {
        int height = 0;
        for (int i = 0; i < l; i++) height = max(height, getHeight(node->getkid(node)[i]));
        return (1 + height);
    }
}

std::vector<Node *> Node::getkid(Node *n) {
    return n->child;
}

void printSpace(int num) {
    int i;
    for (i = 0; i < num - 3; i++) cout << ' ';
    for (; i < num - 2; i++) cout << '*';
    for (; i < num; i++) cout << '_';
}

vector<int> Node::findLabel(const string &label) {
    std::vector<int> labelOccurences;
    for (int i = 0; i < allNodes.size(); i++) {
        if (allNodes[i]->getLabel() == label) labelOccurences.push_back(i);
    }
    return labelOccurences;

}

void Node::print(Node *node, int num) {
    if (node == nullptr) return;
    else if (node != nullptr) {
        printSpace(num);
        cout << node->getLabel() << endl;
        std::vector<Node *> v = getkid(node);
        for (auto &it : v)
            print(it, num + 4);
        return;

    }
}

class RuleSet {
    friend class Node;

public:
    void apply(Node);
    vector<Node> rule_1(Node);

    vector<Node> rule_2(Node);

    vector<Node> rule_3(Node);

    vector<Node> rule_4(Node);

    vector<Node> rule_5(Node);

    vector<Node> rule_6(Node);

};

vector<Node> RuleSet::rule_1(Node node) // a small I region replaced with T region
{
    Node *currNode = node.getRoot();
    std::vector<int> targetNodes = node.findLabel("I");
    int i, n = static_cast<int>(targetNodes.size());
    if(n>0) {
    int mask = static_cast<int>(pow(2, n));
    vector<Node> T_reduced(mask);
    for (i = 0; i < mask; i++) {
        T_reduced[i] = node;
        T_reduced[i].setAllNodes(node.allNodes[0]->pFunctionInit());
        T_reduced[i].setRoot(T_reduced[i].allNodes[0]); //so that all roots are different
        // cout<<"pointer to function worked.\n";
    }

    for (i = 0; i < pow(2, targetNodes.size()); i++) {
        for (int j = 0; j < targetNodes.size(); j++) {
            if ((i & (1 << j)) > 0) {
                int pos = j;


                T_reduced[i].allNodes[targetNodes[pos]]->setLabel("T");

            }

        }

    }
    cout << endl;

//    for (int i = 0; i < mask; i++) {
//        cout << "\nreduced trees are: \n";
//        T_reduced[i].print(T_reduced[i].getRoot(), 0);
//    }
        return T_reduced;
    } else
    {    vector<Node> T_reduced;
    T_reduced.push_back(node);
    T_reduced[0].setAllNodes(node.allNodes[0]->pFunctionInit());
    T_reduced[0].setRoot(T_reduced[0].allNodes[0]); //so that all roots are different
    return T_reduced;
}}

vector<Node> RuleSet::rule_2(Node node) {

    cout << "\nrule_2 called\n";
    Node *currNode = node.getRoot();
    std::vector<int> targetNodes = node.findLabel("T");
    int i, n = static_cast<int>(targetNodes.size());
    if(n>0) {
        int mask = static_cast<int>(pow(2, n));
        vector<Node> T_reduced(mask);
        for (i = 0; i < mask; i++) {
            T_reduced[i] = node;
            T_reduced[i].setAllNodes(node.allNodes[0]->pFunctionInit());
            T_reduced[i].setRoot(T_reduced[i].allNodes[0]); //so that all roots are different
            // cout<<"pointer to function worked.\n";
        }

        for (i = 0; i < pow(2, targetNodes.size()); i++) {
            for (int j = 0; j < targetNodes.size(); j++) {
                if ((i & (1 << j)) > 0) {
                    int pos = j;


                    Node * p=T_reduced[i].allNodes[targetNodes[pos]]->getParent(T_reduced[i].allNodes[targetNodes[pos]]);
                    vector<Node *> t= p->getkid(p);
                    t.erase(find(t.begin(),t.begin()+t.size(),T_reduced[i].allNodes[targetNodes[pos]]));
                    T_reduced[i].allNodes.erase(T_reduced[i].allNodes.begin()+targetNodes[pos]);



                }

            }

        }
        cout << endl;

//        for (int i = 0; i < mask; i++) {
//            cout << "\nreduced trees are: \n";
//            T_reduced[i].print(T_reduced[i].getRoot(), 0);
//        }
        return T_reduced;
    } else
    {    vector<Node> T_reduced;
        T_reduced.push_back(node);
        T_reduced[0].setAllNodes(node.allNodes[0]->pFunctionInit());
        T_reduced[0].setRoot(T_reduced[0].allNodes[0]); //so that all roots are different
        return T_reduced;}
}

void RuleSet::apply(Node node) {
    vector<Node > T_set = rule_1(node);int count =0;
    for(auto it=T_set.begin() ; it!=T_set.end();it++)
    {
        cout<<++count<<" :\n";
        (*it).print((*it).getRoot(),0);

    }
    cout<<"Total number of reduced trees : \n";
    cout<<count;



}

int heuristicF( //has bugs
        Node node) //h(t) =  1+(m k=0)(nk i=1) k·i SOH is summation m is height of tree and nk number of nodes at level k
{
    Node *tempNode = node.getRoot();
    int treeHeight = tempNode->getHeight(tempNode);
    cout << "works here , treeHeight found to be :   " << treeHeight << endl << endl;
    int h = 0, nodesAtk = 1;
    vector< Node * > temp;
    temp.push_back(tempNode);
    for (int i = treeHeight; i >= 0; i--) {
        h += (i * nodesAtk);
        nodesAtk = 0;
        cout << "working...value h=" << endl << h << endl << endl;
        auto it = temp.begin();
        auto it_end = temp.end();
        cout << "works 3  " << endl << endl;
        for (; it != it_end; it++) {
            cout << "works 4  " << endl << endl;
            nodesAtk += (*it)->getkid(*it).size();
            cout << nodesAtk << " = nodes at k\n\n";
            cout << temp.size() << endl;
            cout << temp[0]->getLabel() << endl;
            auto it_start=(*it)->getkid(*it).begin();
            auto it_end=(*it)->getkid(*it).end();
            for (auto itt=it_start;itt!=it_end;itt++) {
                temp.push_back(*itt);
            }
            cout << "insert chal gaya.." << "size after insert : " << endl << temp.size() << " ." << endl;
            for (auto it_check = temp.begin(); it_check != temp.end(); it_check++)
                cout << (*it_check)->getLabel() << ' ';

            cout << "print bhi ho gaya" << endl << endl;
//            temp.erase(it);
        }

    }
    return 1 + h;
}

int main() {
    Node treeSet;
    vector<Node> T_set; //to store reduced trees
    treeSet.setAllNodes(init1()); //initialise allNodes vector of tree
    treeSet.setRoot(treeSet.allNodes[0]);
    cout<<"original tree :\n";
    treeSet.print(treeSet.getRoot(), 0);
    cout<<endl<<"enter any key to print reduced trees :\n";
    cin.get();
    RuleSet reduce;
    reduce.apply(treeSet);
//    cout << heuristicF(treeSet) << endl;
//    vector<Node> temp=reduce.rule_2(treeSet);
//     T_set.insert(T_set.end(),temp.begin(),temp.end());
    // for(auto it=T.begin();it!=T.end();it++)
    // {
    //     vector<Node *> temp= *it;
    //     cout<< (temp).size()<<endl;
    //     auto it2=temp.begin();
    //             while(it2!=temp.end()) {
    //                 tree.print(*it2,0);

    //             }
    return 0;
}

