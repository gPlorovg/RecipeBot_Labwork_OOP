#include <algorithm>
#include <iostream>
#include <fstream>
#include "lab4_hash_table.h"

template<typename T>
HashTable<T>::HashTable() {
  size = 2;
  count = 0;
  items.resize(size);
}

template<typename T>
HashTable<T>::HashTable(const HashTable &ht) {
  size = ht.size;
  count = ht.count;
  items = ht.items;
}

template<typename T>
HashTable<T>::~HashTable() {
  this->Clear();
}

template<typename T>
void HashTable<T>::Clear() {
  size = 2;
  count = 0;
  items.clear();
  items.resize(size);
}

template<typename T>
int HashTable<T>::GetCount() const {
  return count;
}

template<typename T>
int HashTable<T>::GetId(const string &key) const {
  int id;
  id = hash<string>{}(key) % size;
  id = abs(id);
  return id;
}

template<typename T>
void HashTable<T>::Resize() {
  size *= 2;
  vector<vector<Item<T>*>> tmp;
  tmp.resize(size);

  for (auto i: items) {
    if(!i.empty()) {
      for (auto j: i) {
        tmp[GetId(j->key)].push_back(j);
      }
    }
  }

  items = tmp;
}

template<typename T>
void HashTable<T>::Add(const Item<T> &item) {
  int id;
  id = GetId(item.key);

  if (!items[id].empty()) {
    for (auto j : items[id]) {
      if (j->key == item.key) {
        cout << item.key << " already exists!";
        exit(2);
      }
    }
  }
  auto tmp = new Item<T>;
  memcpy(tmp, &item, sizeof(Item<T>));
  items[id].push_back(tmp);
  count += 1;

  if (count / size > 1 / 2) {
    Resize();
  }
}

template<typename T>
void HashTable<T>::operator<<(const Item<T> &item) {
  this->Add(item);
}

template<typename T>
Item<T>* HashTable<T>::Find(const string &key) const {
  int id;
  id = GetId(key);
  if (!items[id].empty()) {
    for (auto j: items[id]) {
      if (j->key == key) {
        return j;
      }
    }
  }
  return nullptr;
}

template<typename T>
void HashTable<T>::Remove(const string &key) {
  int id;
  id = GetId(key);

  if (!items[id].empty()) {
    for (int j = 0; j < items[id].size(); j++) {
      if (items[id][j]->key == key) {
        items[id].erase(items[id].begin() + j);
        count -= 1;
      }
    }
  }
}

template<typename T>
T HashTable<T>::operator[](const string &key) const {
  Item<T>* tmp;
  tmp = Find(key);
  if (tmp != nullptr) {
    return tmp->value;
  } else {
    cout << "Error! No '" << key <<"' key in table" << endl;
    exit(1);
  }
}

template<typename T>
bool HashTable<T>::IsIn(const string &key) const {
  return Find(key) != nullptr;
}

template<typename T>
HashTable<T>* HashTable<T>::operator&&(const HashTable<T> &ht) const {
  auto tmp = new HashTable<T>();
  if (ht.GetCount() != 0) {
    for (auto i : items) {
      if (!i.empty()) {
        for (auto j : i) {
          if (ht.IsIn(j->key)) {
            tmp->Add(*j);
          }
        }
      }
    }
  }
  return tmp;
}

template<typename T>
void HashTable<T>::Save(const string &path) const {
  ofstream f;
  f.open(path);
  for (auto i: items) {
    if (!i.empty()) {
      f << "{\n";
      for (auto j: i) {
        f << "\t" << j->key << ": " << j->value << "\n";
      }
      f << "}\n\n";
    }
  }
  f.close();
}

// ТОЛЬКО ДЛЯ НАШЕЙ ЗАДАЧИ!!!
template<typename T>
void HashTable<T>::Load(const string &path) {
  string s;
  ifstream f;
  string line;
  f.open(path);
  if (f.is_open()) {
    while (getline(f, line)) {
      int pos;
      pos = line.find(':');
      if (pos != std::string::npos) {
        auto tmp = new Item<int>;
        tmp->key = line.substr(1, pos - 1);
        tmp->value = stoi(line.substr(pos + 2, line.length()));
        this->Add(*tmp);
      }
    }
  }
  f.close();
}