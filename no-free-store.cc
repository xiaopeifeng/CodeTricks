#include <new>

class NoFreeStore {
  void* operator new(std::size_t sz);
};

NoFreeStore global;

int main()
{
  NoFreeStore local;
  NoFreeStore* freestore = new NoFreeStore;
}
