#include "../include/hashtable.h"

#include <iostream>

int main()
{
    HashTable table;

    table.set("apple", "red");
    table.set("banana", "yellow");
    table.set("cat", "meow");
    table.set("dog", "bark");
    table.set("elephant", "gray");
    table.set("fish", "blue");
    table.set("goat", "white");
    table.set("horse", "brown");
    table.set("ice", "cold");
    table.set("jaguar", "fast");

    table.print();
}