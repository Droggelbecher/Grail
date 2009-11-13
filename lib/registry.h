
#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <list>
#include <map>

class Registrable__ {
  public:
    std::string className;
    //Registrable() { }
    Registrable__(std::string cn) : className(cn) { }
    virtual ~Registrable__() { }

    /**
     * Needs to have at least one virtual method, otherwise
     * derived objects cant be casted to this and back
     * (german visual c++ 2008 page 290 bottom)
     */
    virtual void dummy() { }
};

class Object {
  public:
    std::string className;
    bool registrable;

    Object(std::string cn, bool r = true) : className(cn), registrable(r) { }
    virtual ~Object() { }
    virtual void dummy() { }

    bool isRegistrable() { return registrable; }
    std::string getClassName() { return className; }
};

class Registry {
  #ifdef DEBUG
  public:
  #endif
    enum Scope { SCOPE_CHAPTER, SCOPE_APPLICATION, SCOPE_ALL };

    struct RegistrableInfo {
      Object* registrable;
      Scope scope;

      RegistrableInfo() : registrable(0), scope(SCOPE_APPLICATION) { }
      RegistrableInfo(Object* registrable, Scope scope) : registrable(registrable), scope(scope) { }
      RegistrableInfo(Object& registrable, Scope scope) : registrable(&registrable), scope(scope) { }
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

    void registerChapter(Object&, std::string name);
    void registerApplication(Object&, std::string name);

    void clearData(Scope scope);

    virtual Object& get(std::string name);
};

#endif // REGISTRY_H

