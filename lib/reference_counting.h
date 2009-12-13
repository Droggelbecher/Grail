
#ifndef REFERENCE_COUNTING_H
#define REFERENCE_COUNTING_H

#include <stdint.h>

class ReferenceCounted {
    uint32_t referenceCount;

  public:
    ReferenceCounted();
    virtual ~ReferenceCounted();
    void incrementReferences();
    void decrementReferences();
};

template <typename T>
class Reference {
    T* target;

  public:
    Reference();
    Reference(T* target);
    Reference(T& target);

    Reference(const Reference<T>& other);
    Reference<T>& operator=(const Reference<T>& other);

    virtual ~Reference();

    void reference(T* t);

    T& operator*();
    T* operator->();
};

struct ReferenceCountingTest : public ReferenceCounted {
  static int instances;

  ReferenceCountingTest() { instances++; }
  ~ReferenceCountingTest() { instances--; }
};



#endif // REFERENCE_COUNTING_H

