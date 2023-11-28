#include <iostream>
#include <assert.h>
#include "lab4_hash_table.h"


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