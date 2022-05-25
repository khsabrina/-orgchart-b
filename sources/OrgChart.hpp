#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <typeinfo>
#include <string>

using namespace std;

namespace ariel{
    template <class T = string>
    class OrgChart{
        private:
            struct Node {
                public:
                //variables of Node
                T position;
                vector<Node*> children;
                //Constructors 
                Node(T p): position(p), children(){}
                Node(): position(nullptr), children(){}
                Node(Node* new_node):position(new_node->position),children(new_node->children){}
                //function adding a child to the vector of children that the father holds
                void add_child(T child){
                    Node* new_child = new Node(child);
                    children.push_back(new_child);
                    
                }
            };
            class OrgChart_itertaor{
                private:
                    //variables of Node
                    Node* current =nullptr;
                    size_t counter;
                    vector<Node*> current_order;
                public:
                    //Constructors
                    OrgChart_itertaor(Node* new_node , vector<Node*> vector): current(new_node), current_order(vector),counter(0) {}
                    OrgChart_itertaor(Node* new_node = nullptr): current(new_node),counter(0),current_order(){}
                    /**
                    * Operator overriding:
                    * -> return Node pointer
                    * * return the name of current Node
                    * ++ increase to current node
                    * == , != compression operators
                    * @return
                    */
                    T& operator*() const {
                        return current->position;
                    }
                    T* operator->() const {
                        return &(current->position);
                    }
                    OrgChart_itertaor& operator++() {
                        //checking if we reached the end of the order.
                        if(counter+1 == this->current_order.size()){
                            this->current = nullptr;
                            return *this;
                        }
                        this->counter++;
                        this->current = current_order[this->counter];
                        return *this;
                    }
                    OrgChart_itertaor operator++(int) {
                        //checking if we reached the end of the order.
                        if(counter+1 == this->current_order.size()){
                            this->current = nullptr;
                            return *this;
                        }
                        OrgChart_itertaor tmp= ++*this;
                        return tmp;
		            }
                    bool operator==(const OrgChart_itertaor& another_node) const {
                        return this->current == another_node.current;
                    }
                    bool operator!=(const OrgChart_itertaor& another_node) const {
			            return !(*this == another_node);
		            }
		    };
            Node* root =nullptr;
            vector<Node*> order; 
        public:
            OrgChart(): root(nullptr){}
            ~OrgChart(){
                if(this->root == NULL){
                   return;
                }
                queue<Node*> q;
                q.push(this->root);
                while(!q.empty()){
                    Node* current=q.front();
                    q.pop();
                    for(size_t i=0;i<current->children.size();++i){
                        q.push(current->children[i]);
                    }
                    delete current;
                }
            }
            OrgChart& add_root(T new_root){
                if(this->root == NULL){
                    Node* new_node = new Node(new_root);
                    this->root= new_node;
                }
                else{
                    this->root->position = new_root;
                }
                return *this;
            }
            OrgChart& add_sub(T root, T child){
                if(this->root == NULL){
                    throw invalid_argument{"can't add sub before root"};
                }
                queue<Node*> q;
                q.push(this->root);
                while(!q.empty()){
                    Node* current=q.front();
                    q.pop();
                    //found the father, exiting the function
                    if(current->position == root){
                        current->add_child(child);
                        return *this;
                    }
                    for(size_t i=0;i<current->children.size();++i){
                        q.push(current->children[i]);
                    }
                }
                throw std::invalid_argument("can't find the father");;
            }
            void update_level_order(){
                queue<Node*> q;
                this->order.clear();
                q.push(this->root);
                while(!q.empty()){
                    Node* current=q.front();
                    q.pop();
                    this->order.push_back(current);
                    for(size_t i=0;i<current->children.size();i++){
                        q.push(current->children[i]);
                    }
                }
            }
            void update_reverse_order(){
                queue<Node*> q;
                stack<Node*> s;
                this->order.clear();
                q.push(this->root);
                while(!q.empty()){
                    Node* current=q.front();
                    q.pop();
                    s.push(current);
                    if(current->children.size() >0){
                    for(int i= (current->children.size()-1) ; i>=0 ; i-- ){
                        q.push(current->children.at((size_t)i));
                    }
                    }
                }
                while(!s.empty()){
                    this->order.push_back(s.top());
                    s.pop();
                }
            }
            void update_pre_order(){
                this->order.clear();
                this->order.push_back(this->root);
                update_pre_order_help(this->root->children,0);
            }
            void update_pre_order_help(vector<Node*> vector, size_t size){
                for(size_t i=size; i<vector.size(); i++){
                    this->order.push_back(vector.at(i));
                    update_pre_order_help(vector.at(i)->children,size);
                }
            }
            OrgChart_itertaor begin_level_order() {
                error_root_null();
                update_level_order();
                return OrgChart_itertaor{this->order.at(0),this->order};
            }
            OrgChart_itertaor end_level_order() {
                error_root_null();
                return OrgChart_itertaor{};
            }
            OrgChart_itertaor begin_reverse_order() {
                error_root_null();
                update_reverse_order();
                return OrgChart_itertaor{this->order.at(0),this->order};
            }
            OrgChart_itertaor reverse_order() {
                error_root_null();
                return OrgChart_itertaor{};
            }
            OrgChart_itertaor begin_preorder() {
                error_root_null();
                update_pre_order();
                return OrgChart_itertaor{this->order.at(0),this->order};
            }
            OrgChart_itertaor end_preorder() {
                error_root_null();
                return OrgChart_itertaor{};
            }
            OrgChart_itertaor begin() {
                error_root_null();
                update_level_order();
                return OrgChart_itertaor{this->order.at(0),this->order};
            }
            OrgChart_itertaor end() {
                error_root_null();
                return OrgChart_itertaor{};
            }
            void error_root_null(){
                if(this->root == NULL){
                    throw invalid_argument{"chart is empty!"};
                }
            }
            friend ostream& operator<< (ostream& output, OrgChart const &m) {
                return output;
            }
            //make tidy said to do that
            OrgChart (const OrgChart& copy_chart){}
            OrgChart& operator=(const OrgChart& copy_chart)=delete;
            OrgChart(OrgChart && move_chart)noexcept{};
            OrgChart& operator=(OrgChart&& move_chart)=delete;
    };
}