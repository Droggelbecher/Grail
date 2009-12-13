
#include <iomanip>
#include "unittest.h"

using std::string;
using std::cerr;
using std::endl;
using std::list;
using std::map;
using std::setw;
using std::left;

std::map<std::string, std::list<Unittest*> > Unittest::tests;

Unittest::Unittest(string group, string name) : group(group), name(name), out(cerr) {
  if(!tests.count(group)) {
    tests[group] = list<Unittest*>();
  }
  tests[group].push_back(this);
}

void Unittest::runAll() {
  map<string, list<Unittest*> >::const_iterator group_iter;
  list<Unittest*>::const_iterator test_iter;

  TestResult result;

  cerr << "Running unit tests." << endl;
  for(group_iter = tests.begin(); group_iter != tests.end(); group_iter++) {
    //cerr << "\x1b[01;33mGroup \"" << group_iter->first << "\"\x1b[m" << endl;
    cerr << "\x1b[01;33m" << group_iter->first << "\x1b[m" << endl;
    for(test_iter = group_iter->second.begin(); test_iter != group_iter->second.end(); test_iter++) {
      result.clear();
      cerr <<  "  Test \"" << left << setw(30) << ((*test_iter)->name +"\"... ");
      (*test_iter)->run(result);
      if(result.failed) {
        cerr << endl << "  \x1b[01;31m" << result.failed << " of " << (result.failed + result.passed) << " failed! :(\x1b[m" << endl;
      }
      else {
        cerr << "  \x1b[01;32mAll " << result.passed << " checks passed. :D\x1b[m" << endl;
      }
    } // for test
  } // for group
} // runAll

void Unittest::checkEqual(bool r, string v1, string s1, string s2, TestResult& result) {
  if(!r) {
    result.failed++;
    out << endl << "    \x1b[00;31mFAIL:\x1b[m \"" << s2 << "\" expected, but got \"" << v1 << "\" from \"" << s1 << "\"";
  }
  else {
    result.passed++;
  }
}

