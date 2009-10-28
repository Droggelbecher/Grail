
#include "registry.h"

using std::string;

void Registry::registerChapter(Registrable& registrable, string name) {
  registerObject(name, RegistrableInfo(registrable, RegistrableInfo::SCOPE_CHAPTER));
}

void Registry::registerApplication(Registrable& registrable, string name) {
  registerObject(name, RegistrableInfo(registrable, RegistrableInfo::SCOPE_APPLICATION));
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

Registrable& Registry::get(string name) {
  return *(registrables[name].registrable);
}


