
#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <map>

class Registrable {
  public:
    std::string classname;
    //Registrable() { }
    Registrable(std::string cn) : classname(cn) { }
};

class Registry {
    struct RegistrableInfo {
      Registrable* registrable;
      enum Scope { SCOPE_CHAPTER, SCOPE_APPLICATION };
      Scope scope;

      RegistrableInfo() : registrable(0), scope(SCOPE_APPLICATION) { }
      RegistrableInfo(Registrable* registrable, Scope scope) : registrable(registrable), scope(scope) { }
      RegistrableInfo(Registrable& registrable, Scope scope) : registrable(&registrable), scope(scope) { }
    };

    std::map<std::string, RegistrableInfo> registrables;

    void registerObject(std::string name, RegistrableInfo info);

  public:
    class RegistryException {
      public:
        std::string what;
        RegistryException(std::string what) : what(what) { }
    };

    void registerChapter(Registrable&, std::string name);
    void registerApplication(Registrable&, std::string name);

    Registrable& get(std::string name);
};

#endif // REGISTRY_H

