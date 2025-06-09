#include "hash_table.h"
#include "prime.h"

#include <string>
#include <math.h>

const int PRIME_1 = 1009;
const int PRIME_2 = 2999;
const int HT_INITIAL_BASE_SIZE = 50;

static Item DELETED_ITEM = {"DELETED", ""};

using namespace std;

static int HT_hash(const string& s, const int a, const int m)
{
    long hash = 0;
    int n = s.size();

    for(int i = 0; i < n; i++)
    {
        hash += (long) pow(a, n - (i + 1)) * s[i];
        hash = hash % m;
    }

    return (int) hash;
}

static int getHash(const string& s, const int numOfBuckets, const int attempts)
{
    const int hashA = HT_hash(s, PRIME_1, numOfBuckets);
    const int hashB = HT_hash(s, PRIME_2, numOfBuckets);

    return (hashA + (attempts * (hashB + 1)) % numOfBuckets);
}

void HashTable::deleteTable()
{
    this -> getItems().clear();
    this -> setCount(0);
}

void HashTable::insert(HashTable& ht, const string& key, const string& value)
{
    const int load = ht.getCount() * 100 / ht.getSize();
    if(load > 70) {ht.resizeUp();}
    
    Item temp {key, value};
    int index = getHash(temp.getKey(), ht.getSize(), 0);
    Item& curr = ht.getItem(index);
    int i = 1;

    int max_attempts = ht.getSize();

    while(i < max_attempts and !curr.isEmpty())
    {
        
        if(!curr.isDeleted() and curr.getKey() == key)
        {
            ht.getItem(index) = temp;
            return;
        }
        
        index = getHash(temp.getKey(), ht.getSize(), i);
        curr = ht.getItem(index);
        i++;
    }

    ht.updateItems(temp, index);
    ht.updateCount(1);
}

string HashTable::search(HashTable& ht, const string& key)
{
    int index = getHash(key, ht.getSize(), 0);
    Item& temp = ht.getItem(index);
    int i = 1;

    while(!temp.isEmpty())
    {
        if(!temp.isDeleted() and temp.getKey() == key)
        {
            return temp.getValue();
        }
        
        index = getHash(key, ht.getSize(), i);
        temp = ht.getItem(index);
        i++;
    }

    return "Not Found";
}

void HashTable::deleteItem(HashTable& ht, const string& key)
{
    bool deletedSuccessfully = false;
    
    const int load = ht.getCount() * 100 / ht.getSize();
    if(load < 10) {ht.resizeDown();}
    
    int index = getHash(key, ht.getSize(), 0);
    Item& temp = ht.getItem(index);
    int i = 1;

    int max_attempts = ht.getSize();

    while(i < max_attempts and !temp.isEmpty())
    {
        if(!temp.isDeleted() and temp.getKey() == key)
        {
            ht.getItem(index) = DELETED_ITEM;
            deletedSuccessfully = true;
            break;
        }

        index = getHash(key, ht.getSize(), i);
        temp = ht.getItem(index);
        i++;
    }

    if(deletedSuccessfully) {ht.updateCount(-1);}
}

HashTable* newSized(const int base_size)
{
    HashTable* ht = new HashTable();

    ht -> setSize(nextPrime(base_size));

    ht -> resizeItems(ht -> getSize());

    return ht;
}

HashTable* newHT() 
{
    return newSized(HT_INITIAL_BASE_SIZE);
}

// hash_table.c
void resizeHT(HashTable* ht, const int base_size) 
{
    if(base_size < HT_INITIAL_BASE_SIZE) {return;}

    HashTable* new_ht = newSized(base_size);
    
    for(int i = 0; i < ht -> getSize(); i++) 
    {
        Item* item = &(ht -> getItem(i));
        if(!item -> isDeleted())
        {
            new_ht -> insert(*new_ht, item -> getKey(), item -> getValue());
        }
    }

    ht -> setCount(new_ht -> getCount());

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht -> getSize();
    ht -> setSize(new_ht -> getSize());
    new_ht -> setSize(tmp_size);

    vector<Item>& tmp_items = ht -> getItems();
    ht -> setItems(new_ht -> getItems());
    new_ht -> setItems(tmp_items);

    new_ht -> deleteTable();
}

void HashTable::resizeUp() 
{
    const int new_size = this -> getSize() * 2;
    resizeHT(this, new_size);
}


void HashTable::resizeDown() 
{
    const int new_size = this -> getSize() / 2;
    resizeHT(this, new_size);
}