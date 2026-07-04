#include "../include/hashtable.h"

#include <iostream>

Node::Node(const std::string& key, const std::string& value) {
    this->key = key;
    this->value = value;
    this->next = nullptr;
}

HashTable::HashTable() {
    capacity = 8;
    currentSize = 0;
    buckets.resize(capacity, nullptr);
}

size_t HashTable::hash(const std::string& key, size_t tableCapacity) const {
    size_t hashValue = 0;

    for(char c : key) {
        hashValue = hashValue * 31 + c;
    }

    return hashValue % tableCapacity;
}

void HashTable::set(const std::string& key, const std::string& value) {
    size_t index = hash(key, capacity);

    Node* current = buckets[index];

    while(current != nullptr) {
        if(current->key == key) {
            current->value = value;
            return;
        }

        current = current->next;
    }

    Node* newNode = new Node(key, value);

    newNode->next = buckets[index];
    buckets[index] = newNode;

    currentSize++;

    double loadFactor = (double) currentSize / capacity;

    if(loadFactor > 0.75) {
        rehash();
    }
}

std::string HashTable::get(const std::string& key) const {
    size_t index = hash(key, capacity);

    Node* current = buckets[index];

    while(current != nullptr) {
        if(current->key == key){
            return current->value;
        }

        current = current->next;
    }

    return "";
}

bool HashTable::exists(const std::string& key) const {
    size_t index = hash(key, capacity);

    Node* current = buckets[index];

    while(current != nullptr) {
        if(current->key == key){
            return true;
        }

        current = current->next;
    }

    return false;
}

void HashTable::remove(const std::string& key) {
    size_t index = hash(key, capacity);

    Node* current = buckets[index];
    Node* previous = nullptr;

    while(current != nullptr) {

        if(current->key == key) {

            if(previous == nullptr) {
                buckets[index] = current->next;
            }
            else {
                previous->next = current->next;
            }

            // Free memory
            delete current;

            // Decrement size
            currentSize--;

            return;
        }

        previous = current;
        current = current->next;
    }

    return;
}

void HashTable::print() const {

    for(size_t i = 0; i < capacity; i++) {
        std::cout << "Bucket " << i << ": ";

        Node* current = buckets[i];

        while(current != nullptr) {
            std::cout << "(" << current->key << ", " << current->value << ") ";
            current = current->next;
        }

        std::cout << std::endl;
    }
}

HashTable::~HashTable() { // destructor function to free memory

    for(size_t i = 0; i < capacity; i++) {
        Node* current = buckets[i];

        while(current != nullptr) {
            Node* next = current->next;

            delete current;

            current = next;
        }
    }
}

void HashTable::rehash() {
    size_t newCapacity = capacity * 2;

    std::vector<Node*> newBuckets(newCapacity, nullptr);

    for(size_t i = 0; i < capacity; i++) {
        Node* current = buckets[i];

        while(current != nullptr) {
            Node* next = current->next;

            size_t newIndex = hash(current->key, newCapacity);

            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;

            current = next;
        }
    }

    std::cout << "\nREHASHING\n";
    std::cout << "Old Capacity: " << capacity << std::endl;
    std::cout << "New Capacity: " << newCapacity << std::endl;
    
    // Replace old bucket array
    buckets = std::move(newBuckets);
    capacity = newCapacity;
}