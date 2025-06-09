#include <vector>
#include <optional>
using namespace std;

class Item
{
    private:
        
        string itemKey {""};
        string itemValue {""};

    public:
        
        Item(string key, string value) : itemKey {key}, itemValue {value} {}

        const string& getKey() {return this -> itemKey;}

        const string& getValue() {return this -> itemValue;}

        bool isEmpty() const {return itemKey.empty();}

        bool isDeleted() const {return itemKey == "DELETED";} // Or some other marker
};

class HashTable
{
    private:  
        
        int htSize {};
        int htCount {0};
        vector<Item> htItems {vector<Item>(0)};

    public:
        
        HashTable() : htSize {50}, htCount {0}, htItems {vector<Item>(50)} {}

        const int& getSize() {return this -> htSize;}

        const int& getCount() {return this -> htCount;}

        Item& getItem(int index) {return (this -> htItems) [index];}

        vector<Item>& getItems() {return this -> htItems;}

        void setSize(int size) {this -> htSize = size;}

        void setCount(int cnt) {this -> htCount = cnt;}

        void setItems(vector<Item>& items) {this -> htItems = items;}

        void updateItems(Item& item, int& index) {htItems[index] = item;}

        void updateCount(int val) {this -> htCount += val;}

        void resizeItems(int newSize) {(this -> htItems).resize(newSize);}

        void deleteTable();

        void resizeUp();

        void resizeDown();

        static void insert(HashTable& ht, const string& key, const string& value);
        
        static string search(HashTable& ht, const string& key);
        
        static void deleteItem(HashTable& h, const string& key);
};