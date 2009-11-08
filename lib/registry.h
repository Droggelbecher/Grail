
#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <list>
#include <map>

class Registrable {
  public:
    std::string className;
    //Registrable() { }
    Registrable(std::string cn) : className(cn) { }
    virtual ~Registrable() { }

    /**
     * Needs to have at least one virtual method, otherwise
     * derived objects cant be casted to this and back
     * (german visual c++ 2008 page 290 bottom)
     */
    virtual void dummy() { }
};

class Registry {
  #ifdef DEBUG
  public:
  #endif
    enum Scope { SCOPE_CHAPTER, SCOPE_APPLICATION, SCOPE_ALL };

    struct RegistrableInfo {
      Registrable* registrable;
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

    ~Registry();

    void registerChapter(Registrable&, std::string name);
    void registerApplication(Registrable&, std::string name);

    void clearData(Scope scope);

    virtual Registrable& get(std::string name);
};

#endif // REGISTRY_H

