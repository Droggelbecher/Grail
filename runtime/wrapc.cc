
#include <iostream>
#include <cassert>
#include <fstream>
#include <deque>
#include <list>
#include <map>

using std::istream;
using std::deque;
using std::string;
using std::cout;
using std::endl;
using std::list;
using std::ifstream;
using std::ostream;
using std::map;

class Tokenizer {
    istream& stream;
    string singles;
    deque<string> results;
    enum CommentState { CODE, SLASH, SLASHSTAR, SLASHSTARSTAR, END };

  public:
    Tokenizer(istream& stream) : stream(stream), singles("{}()*;,") {
    }

    bool isWhiteSpace(char c) {
      return string(" \t\n\a").find(c) != string::npos;
    }

    bool isSingle(char c) {
      return singles.find(c) != string::npos;
    }

    bool parseToken() {
      if(!stream.good()) { return false; }

      char c;
      string token;
      bool isWhiteSpaceOrComment = true;

      while(isWhiteSpaceOrComment) {
        // Eat white space
        while(stream.good()) {
          c = stream.get();
          if(!stream.good()) { return false; }
          if(!isWhiteSpace(c)) {
            stream.unget();
            break;
          }
        }
        if(!stream.good()) { return false; }

        CommentState commentState = CODE;
        // Eat comments
        while(stream.good() && commentState != END) {
          c = stream.get();
          if(!stream.good()) { return false; }
          switch(commentState) {
            case CODE:
              if(c == '/') {
                commentState = SLASH;
                isWhiteSpaceOrComment = true;
              }
              else {
                isWhiteSpaceOrComment = false;
                commentState = END;
              }
              break;

            case SLASH:
              if(c == '*') { commentState = SLASHSTAR; }
              else { commentState = CODE; }
              break;

            case SLASHSTAR:
              if(c == '*') { commentState = SLASHSTARSTAR; }
              else { }
              break;

            case SLASHSTARSTAR:
              if(c == '/') {
                commentState = END;
              }
              else {
                commentState = SLASHSTAR;
              }
              break;
            case END:
              break;
          }
        }
      }

      if(isSingle(c)) {
        token = string(1, c);
        results.push_back(token);
        return true;
      }

      while(stream.good() && !isWhiteSpace(c) && !isSingle(c)) {
        token.push_back(c);
        c = stream.get();
      }
      stream.unget();
      results.push_back(token);
      return true;
    }

    string getToken() {
      if(results.size() > 0) {
        string r = results.front();
        results.pop_front();
        return r;
      }
      return "";
    }

    void parseAll() {
      while(parseToken()) ;
    }

};

class Parser {
  public:
    struct MethodDefinition {
      string returnType;
      string name;
      list<string> parameters;
      bool isStatic;

      void clear() {
        returnType = "";
        name = "";
        parameters.clear();
        isStatic = false;
      }
    };

    void (Parser::*parseToken)(string);
    MethodDefinition currentMethod;

    Parser() {
      parseToken = &Parser::parseInitial;
    }

    virtual void onStartClass(string name) { }
    virtual void onDefineMethod(MethodDefinition info) { }
    virtual void onClassEnd() { }

    bool isSpecial(string token) {
      return token == "&" || token == "*";
    }

    void parseInitial(string token) {
      //if(token == "/") { parseToken = &Parser::parseComment1; }
      if(token == "class") { parseToken = &Parser::parseClassName; }
      else { assert(false); }
    }

    /*
    void parseComment1(string token) {
      assert(token == "*");
      parseToken = &Parser::parseComment2;
    }

    void parseComment2(string token) {
      if(token == "*") {
        parseToken = &Parser::parseComment3;
      }
    }

    void parseComment3(string token) {
      if(token == "/") { parseToken = &Parser::parseInitial; }
      else { parseToken = &Parser::parseComment2; }
    }
    */

    void parseClassName(string token) { 
      onStartClass(token);
      parseToken = &Parser::parseClassStart;
    }

    void parseClassStart(string token) {
      assert(token == "{");
      parseToken = &Parser::parseMethodReturnType;
      currentMethod.clear();
    }

    void parseMethodReturnType(string token) {
      if(token == "}") {
        onClassEnd();
        parseToken = &Parser::parseInitial;
      }
      else if(token == "static") {
        currentMethod.isStatic = true;
      }
      else {
        currentMethod.returnType = token;
        parseToken = &Parser::parseMethodName;
      }
    }

    void parseMethodName(string token) {
      if(isSpecial(token)) {
        currentMethod.returnType += token;
      }
      else {
        currentMethod.name = token;
        parseToken = &Parser::parseParameterListBegin;
      }
    }

    void parseParameterListBegin(string token) {
      assert(token == "(");
      parseToken = &Parser::parseParameter;
    }

    void parseParameter(string token) {
      if(token == ")") {
        onDefineMethod(currentMethod);
        parseToken = &Parser::parseMethodReturnType;
        currentMethod.clear();
      }
      else {
        currentMethod.parameters.push_back(token);
        parseToken = &Parser::parseCommaOrParameterListEnd;
      }
    }

    void parseCommaOrParameterListEnd(string token) {
      if(token == ",") {
        parseToken = &Parser::parseParameter;
      }
      else if(token == ")") {
        onDefineMethod(currentMethod);
        parseToken = &Parser::parseMethodReturnType;
        currentMethod.clear();
      }
      else { // parameter addition such as "*"
        string t = currentMethod.parameters.back();
        currentMethod.parameters.pop_back();
        t += string(" ") + token;
        currentMethod.parameters.push_back(t);
      }
    }
};

class LuaWrapperParser : public Parser {
  public:
    ostream& out;
    string currentClass;

    map<string, list<string>* > methods;

    LuaWrapperParser(ostream& out) : out(out) { }
    ~LuaWrapperParser() {
      map<string, list<string>* >::iterator map_iter;

      for(map_iter = methods.begin(); map_iter != methods.end(); map_iter++) {
        delete map_iter->second;
      }
      methods.clear();
    }

    void onStartClass(string name) {
      currentClass = name;
      methods[name] = new list<string>();
    }

    void onDefineMethod(MethodDefinition info) {
      bool hasThis = !info.isStatic;

      methods[currentClass]->push_back(info.name);

      out << "int _" << currentClass << "_" << info.name << "(lua_State* L) {" << endl;
      out << "  assert(lua_gettop(L) == " << info.parameters.size() + hasThis << ");" << endl;
      if(hasThis) {
        out << "  " << currentClass << "* obj = luaGet<" << currentClass << "*>(L, 1);" << endl;
      }

      list<string>::const_iterator iter;
      int i = 2;
      for(iter = info.parameters.begin(); iter != info.parameters.end(); iter++, i++) {
        out << "  " << *iter << " p" << i << " = luaGet<" << *iter << ">(L, " << i << ");" << endl;
      }

      string base;
      if(hasThis) { base = "obj->"; }
      else { base = currentClass + "::"; }

      if(info.returnType == "void") {
        out << "  " << base << info.name << "(";
      }
      else {
        out << "  " << info.returnType << " ret = " << base << info.name << "(";
      }

      i = 2;
      string comma = "";
      for(iter = info.parameters.begin(); iter != info.parameters.end(); iter++, i++) {
        out << comma << "p" << i;
        comma = ", ";
      }
      out << ");" << endl;

      if(info.returnType == "void") {
        out << "  return 0;" << endl;
      }
      else {
        out << "  return luaPush<" << info.returnType << ">(L, ret);" << endl;
        //out << "  return luaPush(L, ret);" << endl;
      }

      out << "}" << endl << endl;
    }

    void registerWrappings() {
      map<string, list<string>* >::iterator map_iter;
      out << "void registerLuaWrappings() {" << endl;
      for(map_iter = methods.begin(); map_iter != methods.end(); map_iter++) {
        out << "  interpreter.registerBase(\"" << map_iter->first << "\");" << endl;
        list<string>::iterator list_iter;
        for(list_iter = map_iter->second->begin(); list_iter != map_iter->second->end(); list_iter++) {
          out << "  interpreter.registerMethod(\"" << map_iter->first << "\", \""
            << *list_iter << "\", &_" << map_iter->first << "_" << *list_iter << ");" << endl;
        }
        out << endl;
      }
      out << "}" << endl << endl;
    }

};





int main(int argc, char** argv) {
  assert(argc > 1);

  ifstream f(argv[1], ifstream::in);
  ostream& out = cout;

  Tokenizer t(f);
  LuaWrapperParser p(out);

  out << "\n//\n// /!\\ WARNING: This is generated code. Changes to it will be lost.\n//\n" << endl;

  out << "#include <cassert>" << endl;
  out << "#include <string>" << endl << endl;
  out << "#include \"lib/actor.h\"" << endl;
  out << "#include \"lib/animation.h\"" << endl;
  out << "#include \"lib/area.h\"" << endl;
  out << "#include \"lib/classes.h\"" << endl;
  out << "#include \"lib/event.h\"" << endl;
  out << "#include \"lib/game.h\"" << endl;
  out << "#include \"lib/image.h\"" << endl;
  out << "#include \"lib/mainloop.h\"" << endl;
  out << "#include \"lib/rect.h\"" << endl;
  out << "#include \"lib/registry.h\"" << endl;
  out << "#include \"lib/resource_manager.h\"" << endl;
  out << "#include \"lib/scene.h\"" << endl;
  out << "#include \"lib/shortcuts.h\"" << endl;
  out << "#include \"lib/sprite.h\"" << endl;
  out << "#include \"lib/surface.h\"" << endl;
  out << "#include \"lib/unittest.h\"" << endl;
  out << "#include \"lib/user_interface_element.h\"" << endl;
  out << "#include \"lib/user_interface.h\"" << endl;
  out << "#include \"lib/utils.h\"" << endl;
  out << "#include \"lib/vector2d.h\"" << endl;
  out << "#include \"lib/viewport.h\"" << endl << endl;
  out << "#include \"lua_utils.h\"" << endl;
  out << "#include \"interpreter.h\"" << endl << endl;

  out << "using std::string;" << endl << endl;


  while(t.parseToken()) {
    string token = t.getToken();
    (p.*p.parseToken)(token);
  }
  p.registerWrappings();

  return 0;
}


