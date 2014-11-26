#ifndef CACHE_H
#define CACHE_H 
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

template<class K, class T>
struct Node
{
    K key;
    T data;
    Node *prev, *next;
};

template<class K, class T>
class Cache
{
    public:
        Cache(){}
        void setCache(size_t size);
        ~Cache();
        void put(K key, T data);
        T get(K key);
        void print();
    private:
        void detach(Node<K, T>* node);
        void attach(Node<K, T>* node);

        unordered_map<K, Node<K, T>* > hashmap_;
        vector<Node<K,T>* > free_entries_; // 存储可用结点的地址
        Node<K,T> *head_, *tail_;
        Node<K,T> *entries_; // 双向链表中的结点
};

template<class K, class T>
void Cache<K, T>::print()
{
    for(auto &item: hashmap_){
        //cout << item.first << endl;
        std::cout << item.second->key<< " " << item.second->data << std::endl;
            
    }
}


template<class K, class T>
void Cache<K, T>::setCache(size_t size)
{
    entries_ = new Node<K,T>[size];
    for(int i=0; i<size; ++i)// 存储可用结点的地址
        free_entries_.push_back(entries_+i);
    head_ = new Node<K,T>;
    tail_ = new Node<K,T>;
    head_->prev = NULL;
    head_->next = tail_;
    tail_->prev = head_;
    tail_->next = NULL;
}

template<class K, class T>
Cache<K, T>::~Cache()
{
    delete head_;
    delete tail_;
    delete[] entries_;
}


template<class K, class T>
void Cache<K, T>::put(K key, T data)
{
    Node<K,T> *node = hashmap_[key];
    if(node){ // node exists
        detach(node);
        node->data = data;
        attach(node);
    }
    else{
        if(free_entries_.empty()){// 可用结点为空，即cache已满
            node = tail_->prev;
            detach(node);
            hashmap_.erase(node->key);
        }
        else{
            node = free_entries_.back();
            free_entries_.pop_back();
        }
        node->key = key;
        node->data = data;
        hashmap_[key] = node;
        attach(node);
    }
}

template<class K, class T>
T Cache<K, T>::get(K key)
{
    Node<K,T> *node = hashmap_[key];
    if(node){
        detach(node);
        attach(node);
        return node->data;
    }
    else{// 如果cache中没有，返回T的默认值。与hashmap行为一致
        return T();
    }
}

template<class K, class T>
void Cache<K, T>::detach(Node<K,T>* node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

template<class K, class T>
void Cache<K, T>::attach(Node<K, T>* node)
{
    node->prev = head_;
    node->next = head_->next;
    head_->next = node;
    node->next->prev = node;
}

#endif  /*CACHE_H*/  
