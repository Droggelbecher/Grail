
#include <cassert>
#include "reference_counting.h"

ReferenceCounted::ReferenceCounted() : referenceCount(0) { }

ReferenceCounted::~ReferenceCounted() { }

void ReferenceCounted::incrementReferences() {
  referenceCount++;
}

void ReferenceCounted::decrementReferences() {
  assert(referenceCount > 0);
  referenceCount--;
  if(referenceCount == 0) {
    delete this;
  }
}


template <typename T>
Reference<T>::Reference() : target(0) { }

template <typename T>
Reference<T>::Reference(T* target) : target(target) {
  target->incrementReferences();
}

template <typename T>
Reference<T>::Reference(T& t) : target(&t) {
  target->incrementReferences();
}

template <typename T>
Reference<T>::Reference(const Reference<T>& other) {
  target = other.target;
  if(target) {
    target->incrementReferences();
  }
}

template <typename T>
Reference<T>::~Reference() {
  if(target) {
    target->decrementReferences();
  }
}

template <typename T>
Reference<T>& Reference<T>::operator=(const Reference<T>& other) {
  target = other.target;
  if(target) {
    target->incrementReferences();
  }
  return *this;
}

template <typename T>
void Reference<T>::reference(T* t) {
  if(target) {
    target->decrementReferences();
  }
  target = t;
  if(target) {
    target->incrementReferences();
  }
}

template <typename T>
T& Reference<T>::operator*() { return *target; }

template <typename T>
T* Reference<T>::operator->() { return target; }

int ReferenceCountingTest::instances = 0;

