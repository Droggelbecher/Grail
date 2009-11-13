
#include "registry.h"
#include "utils.h"
#include <cassert>

using std::string;
using std::map;
using std::list;

Registry::~Registry() {
  clearData(SCOPE_ALL);
}

void Registry::registerChapter(Object& registrable, string name) {
  assert(registrable.isRegistrable());
  registerObject(name, RegistrableInfo(registrable, SCOPE_CHAPTER));
}

void Registry::registerApplication(Object& registrable, string name) {
  assert(registrable.isRegistrable());
  registerObject(name, RegistrableInfo(registrable, SCOPE_APPLICATION));
}

void Registry::registerObject(string name, RegistrableInfo info) {
  if(registrables.count(name)) {
    throw RegistryException(
        string("Could not register object '") + name +
        string("' because an object with the same name already exists.")
    );
  }

  registrables[name] = info;
} // registerObject

Object& Registry::get(string name) {
  Object* r = registrables[name].registrable;
  if(!r)
    throw Exception(string("Could not find object named '") + name + string("' in registry."));
  return *r;
}

void Registry::clearData(Scope scope) {
  map<string, RegistrableInfo>::iterator iter;
  list<string> l;
  for(iter=registrables.begin(); iter != registrables.end(); iter++) {
    if(iter->second.scope == scope || scope == SCOPE_ALL) {
      delete iter->second.registrable;
      l.push_back(iter->first);
    }
  }
  list<string>::iterator li;
  for(li=l.begin(); li!=l.end(); li++) {
    registrables.erase(*li);
  }
}

