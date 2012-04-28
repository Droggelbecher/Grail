// vim: set noexpandtab:

#include <iomanip>
#include "unittest.h"

using std::string;
using std::cerr;
using std::endl;
using std::list;
using std::map;
using std::setw;
using std::left;

namespace grail {

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
	info_[0] = '\0';
	
	TestResult result;
	
	cerr << "Running unit tests." << endl;
	for(group_iter = tests.begin(); group_iter != tests.end(); group_iter++) {
		cerr << "\x1b[01;33m" << group_iter->first << "\x1b[m" << endl;
		for(test_iter = group_iter->second.begin(); test_iter != group_iter->second.end(); test_iter++) {
			result.clear();
			cerr <<  "\tTest \"" << left << setw(30) << ((*test_iter)->name +"\"... ");
			(*test_iter)->run(result);
			if(result.failed) {
				cerr << endl << "\t\t\x1b[01;31m=> " << result.failed << " of " << (result.failed + result.passed) << " failed! :(\x1b[m" << endl << endl;
			}
			else {
				cerr << "\t\x1b[01;32mAll " << result.passed << " checks passed. :D\x1b[m" << endl;
			}
		} // for test
	} // for group
} // runAll

void Unittest::checkEqual(bool r, string v1, string s1, string s2, TestResult& result, string file, size_t line) {
	if(!r) {
		result.failed++;
		out << endl << "\t\t\x1b[00;31m" << file << ":" << line << "\x1b[m: \x1b[00;32m" << s2 << "\x1b[m expected, but got >\x1b[00;32m" << v1 << "\x1b[m< from expression: " << s1 << "";
	}
	else {
		result.passed++;
	}
	popInfo(!r);
}

void Unittest::checkGreater(bool r, string v1, string s1, string s2, TestResult& result, string file, size_t line) {
	if(!r) {
		result.failed++;
		out << endl << "\t\t\x1b[00;31m" << file << ":" << line << "\x1b[m: Something greater than \x1b[00;32m" << s2 << "\x1b[m expected, but got >\x1b[00;32m" << v1 << "\x1b[m< from expression: " << s1 << "";
	}
	else {
		result.passed++;
	}
	popInfo(!r);
}

void Unittest::checkLower(bool r, string v1, string s1, string s2, TestResult& result, string file, size_t line) {
	if(!r) {
		result.failed++;
		out << endl << "\t\t\x1b[00;31m" << file << ":" << line << "\x1b[m: Something lower than \x1b[00;32m" << s2 << "\x1b[m expected, but got >\x1b[00;32m" << v1 << "\x1b[m< from expression: " << s1 << "";
	}
	else {
		result.passed++;
	}
	popInfo(!r);
}

void Unittest::popInfo(bool show) {
	if(show && info_[0]) {
		out << endl << "\t\t\x1b[00;33mINFO: " << info_;
	}
	info_[0] = '\0';
}

void Unittest::info(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(info_, INFO_BUFSIZE, fmt, ap);
	va_end(ap);
}


char Unittest::info_[Unittest::INFO_BUFSIZE];

} // namespace grail

