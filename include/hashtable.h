#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>

class Node {
public:
    std::string key;
    std::string value;
    Node* next;

    Node(const std::string& key, const std::string& value);
};

class HashTable {
public:
    HashTable();
    ~HashTable();

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;
    bool exists(const std::string& key) const;
    void remove(const std::string& key);
    void print() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

private:
    std::vector<Node*> buckets;
    size_t capacity;
    size_t currentSize;

    size_t hash(const std::string& key, size_t tableCapacity) const;
    void rehash();
};

#endif