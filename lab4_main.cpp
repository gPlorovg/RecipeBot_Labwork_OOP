#include <algorithm>
#include <iostream>
#include <fstream>
#include <assert.h>
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


int main() {
  auto ht = new HashTable<int>();
  Item<int> i1 = {"xxx", 1};
  Item<int> i2 = {"yyy", 2};
  Item<int> i3 = {"zzz", 3};
  ht->Add(i1);
  ht->Add(i2);
  ht->Add(i3);
  assert(ht->GetCount() == 3);
  assert(ht->IsIn(i1.key));
  ht->Remove(i3.key);
  assert(ht->GetCount() == 2);
  assert(!ht->IsIn(i3.key));
  *ht << i3;
  assert(ht->IsIn(i3.key));
  assert((*ht)[i2.key] == i2.value);
  auto ht0 = new HashTable<int>(*ht);
  assert(ht0->GetCount() == 3);
  assert(ht0->IsIn(i1.key));
  auto htx = *ht && *ht0;
  assert(htx->GetCount() == 3);
  assert(htx->IsIn(i1.key));
  ht0->Clear();
  assert(ht0->GetCount() == 0);
  auto htx0 = *ht && *ht0;
  assert(htx0->GetCount() == 0);
  assert(!htx0->IsIn(i1.key));

  ht->Save("../out");
  ht0->Load("../out");
  assert(ht0->GetCount() == 3);
  assert(ht0->IsIn(i1.key));

  cout << "All test completed!" << endl;
  return 0;
}