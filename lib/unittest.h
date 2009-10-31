
#ifndef UNITTEST_H
#define UNITTEST_H

#include <iostream>
#include <sstream>
#include <cassert>
#include <string>
#include <list>
#include <map>

class Unittest {
    static std::map<std::string, std::list<Unittest*> > tests;


  protected:
    std::string group, name;
    std::ostream& out;

    struct TestResult {
      size_t passed;
      size_t failed;
      TestResult() : passed(0), failed(0) { };
      void clear() { passed = failed = 0; }
    };

  public:


    Unittest(std::string group, std::string name);
    virtual void run(TestResult& result) = 0;
    void checkEqual(bool r, std::string v1, std::string s1, std::string s2, TestResult& result);

    static void runAll();

};

template<typename T> std::string toString(T t) {
  std::ostringstream ss;
  ss << t;
  return ss.str();
}

#define TEST(GROUP, TEST) class __unittest__ ## GROUP ## __ ## TEST : public Unittest { \
  public: \
  __unittest__ ## GROUP ## __ ## TEST () : Unittest("GROUP", "TEST") { }; \
  void run(TestResult& result); \
  }  unittest ## GROUP ## TEST ## Instance; \
  void __unittest__ ## GROUP ## __ ## TEST :: run(Unittest::TestResult& result)

#define CHECK_EQUAL(x, y) checkEqual((x) == (y), toString((x)), #x, #y, result)

#endif // UNITTEST_H

