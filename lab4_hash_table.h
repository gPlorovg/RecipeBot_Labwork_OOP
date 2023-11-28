#ifndef LAB1__LAB4_HASH_TABLE_H_
#define LAB1__LAB4_HASH_TABLE_H_
#include <string>
#include <vector>
using namespace std;

template<typename T>
struct Item {
  std::string key;
  T value;
};

template<typename T>
class HashTable {
 public:
  HashTable();
  HashTable(const HashTable &ht);
  ~HashTable();

  void Clear();
  [[nodiscard]] int GetCount() const;
  void Save(const string &path) const;
  void Load(const string &path);
  void operator << (const Item<T> &item);
  void Add(const Item<T> &item);
  void Remove(const string &key);
  T operator [] (const string &key) const;
  [[nodiscard]] bool IsIn(const string &key) const;
  HashTable<T>* operator && (const HashTable<T> &ht) const;

 private:
  vector<vector<Item<T>*>> items;
  int size;
  int count;
  [[nodiscard]] int GetId(const string &key) const;
  void Resize();
  Item<T>* Find(const string &key) const;
};

#endif
