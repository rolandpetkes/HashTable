#include "HashTable.h"

HashTable::HashTable(int capacity) {
    size = 0;
    this -> capacity = capacity;
    table.resize(capacity);
}

int HashTable::hash(const string &key) const {  // returns the hash value of the given key
    int hash_value = 0;
    for (char ch : key) {
        hash_value = (hash_value * 31 + ch) % capacity;
    }
    return hash_value;
}

int HashTable::probe(int index, int i) const {      // quadratic probing
    return (index + i * i) % capacity;
}

void HashTable::rehash(int new_capacity) {  // inserts all data into a new table, called when the load factor is above or below a given threshold
    vector<Element> old_table = table;      // saving data from previous table
    int old_capacity = capacity;

    capacity = new_capacity;        // resizing
    table.clear();
    table.resize(capacity);
    size = 0;

    for (int i = 0; i < old_capacity; ++i) {        // inserting all data from previous table
        if (old_table[i].occupied) {
            insert(old_table[i].key, old_table[i].data);
        }
    }
}

void HashTable::insert(const string &key, const string &data) {        // inserts data into the HashTable based on the given key
    if (size >= capacity * 0.75) {      // load factor checking
        int new_capacity = capacity * 2;
        rehash(new_capacity);
    }

    int index = hash(key);
    int i = 0;

    while (table[index].occupied) {     // looking for an empty spot
        if (table[index].key == key) {
            throw "Key already exists.\nCannot add data!";
        }

        i++;
        index = probe(index, i);        // quadratic probing
    }

    table[index].key = key;     // inserting
    table[index].data = data;
    table[index].occupied = true;
    size++;
}

string HashTable::retrieve(const string &key) const {       // returns the data associated with given key
    int index = hash(key);
    int i = 0;

    while (table[index].occupied) {     // searching
        if (table[index].key == key) {
            return table[index].data;
        }

        i++;
        index = probe(index, i);
    }

    throw "Key not found!\nUnable to retreive.";        // error handling
}

string HashTable::search(const string &data) const {       // linear search based on stored data (for speed comparison)
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].occupied && table[i].data == data) {
            return table[i].key;
        }
    }
    throw "data not found!\nUnable to retreive.";
}

void HashTable::remove(const string &key) {     // deletes the data associated with a given key
    int index = hash(key);
    int i = 0;

    while (table[index].occupied) {     // searching
        if (table[index].key == key) {
            table[index].occupied = false;
            size--;

            if (size < capacity * 0.25 && capacity > 17) {      // load factor checking
                int new_capacity = capacity / 2;
                rehash(new_capacity);
            }

            return;
        }

        i++;
        index = probe(index, i);
    }

    throw "Key not found!\nUnable to delete.";
}

void HashTable::print() const {     // prints all data on screen
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].occupied) {
            cout << table[i].key << endl << table[i].data << endl;
        }
    }
}

void HashTable::printToFile(const string &file_name) const {    // writes all data into a given text file
    ofstream fout(file_name);
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].occupied) {
            fout << table[i].key << endl << table[i].data << endl;
        }
    }
}

int HashTable::getSize() const {    // returns size
    return size;
}
