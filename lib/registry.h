
#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <list>
#include <map>

class Registrable {
  public:
    std::string classname;
    //Registrable() { }
    Registrable(std::string cn) : classname(cn) { }
    virtual ~Registrable() { }

    /**
     * Needs to have at least one virtual method, otherwise
     * derived objects cant be casted to this and back
     * (german visual c++ 2008 page 290 bottom)
     */
    virtual void dummy() { }
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

    /**
     * Return registered names.
     * This method has a horribly dump implementation performance-wise. Use
     * only for debugging.
     */
    std::list<std::string> getRegisteredNames() {
      std::map<std::string, RegistrableInfo>::const_iterator iter;
      std::list<std::string> r;
      for(iter = registrables.begin(); iter != registrables.end(); iter++) {
        r.push_back(iter->first);
      }
      return r;
    }
};

#endif // REGISTRY_H

