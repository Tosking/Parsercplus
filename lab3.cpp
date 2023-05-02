#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct var{
    public:
        string name;
        bool val;
        var(string& name, bool val);
};

ifstream in;

class Parser{
    private:
        vector<var> varlist;
        int c = EOF;
        int last = -1;
        bool CheckA();
        int TriadCount = 0;
    public:
        inline void GetC();
        bool GetVarValue(string& str);
        bool& GetVarAdress(string str);

        bool MethodC();
        void MethodS();
        bool& MethodL();
        bool MethodE();
        bool MethodT();
        bool MethodM();
        bool MethodI();

        void Print();
        void PrintAll();
        void Run();
};

int main(){
    in.open("test.txt", ios::binary);
    Parser parser;
    parser.Run();
    in.close();
    return 0;
}

void Error(string msg){
    cout << "Error!:" << msg << endl;
    exit(1);
}

bool Parser::CheckA(){
    return (c>='a' && c<='z') || c=='_';
}

inline void Parser::GetC(){
    c = in.get();
}

bool Parser::GetVarValue(string& str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].name == str){
            return varlist[i].val;
        }
    }
    Error("Invalid Indentifer!");
    return 0;
}

bool& Parser::GetVarAdress(string str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].name == str){
            return varlist[i].val;
        }
    }
    cout << TriadCount++ <<": V(" << str << ", @)" << endl;
    varlist.push_back(var{str, 0});
    return varlist[varlist.size() - 1].val;
}

void Parser::MethodS(){
   int x = MethodL();
   if(c != '='){
       Error("Missing symbol '='!");
   }
   GetC();
   int y = MethodE();
   varlist[varlist.size()-1].val = x;
   if(c != ';'){
       Error("Missing symbol ';'!");
   }
   cout << TriadCount++ << ": =(^" << x << ", ^" << y << ")" << endl;
   GetC();
}

bool& Parser::MethodL(){
    string str;
    bool flag = 0;
    while(CheckA() || (c == '0' || c == '1')){
        str += c;
        GetC();
    }
    return GetVarAdress(str);
}

bool Parser::MethodE(){
    int x = MethodT();
    int y;
    while(c == '|'){
        GetC();
        y = MethodT();
        cout << TriadCount++ << ": |(^" << x << ", ^" << y << ")" << endl;
        x = TriadCount;
    }
    return x;
}

bool Parser::MethodT(){
    int x = MethodM();
    int y;
    while(c == '&'){
        GetC();
        y = MethodM();
        cout << TriadCount++ << ": &(^" << x << ", ^" << y << ")" << endl;
        x = TriadCount;
    }
    return x;
}

bool Parser::MethodM(){
    if(c == '~'){
        GetC();
        int x = MethodM();
        cout << TriadCount++ << ": ~(^" << x << ", @)" << endl;
        return TriadCount;
    }
    if(c == '('){
        GetC();
        bool x = MethodE();
        if(c != ')'){
            Error("Invalid Syntax!");
            return 0;
        }
        else{
            GetC();
            return x;
        }
    }
    if(c == '1' || c == '0'){
        return MethodC();
    }
    if(CheckA()){
        return MethodI();
    }
    Error("Syntax Error!");
    return 0;
}

bool Parser::MethodI(){
    string str;
    while(CheckA() || (c == '0' || c == '1')){
        str += c;
        GetC();
    }
    return GetVarValue(str);
}

bool Parser::MethodC(){
    bool prev = (char)c - '0';
    GetC();
    cout << TriadCount++ << ": C(" << prev << ", @)" << endl;
    return TriadCount;
}
void Parser::Print()
{
    if (varlist.size() == 0)
        cout << "No varibales defined yet.\n";
    else
        cout << varlist[varlist.size()-1].name << " = " << varlist[varlist.size()-1].val << endl;
}

void Parser::PrintAll()
{
    if (!varlist.size())
        cout << "No variables defined yet." << endl;
    else
        for (int i = 0; i < varlist.size(); i++)
            cout<<varlist[i].name<< " = " << varlist[i].val<<endl;
}

void Parser::Run(){
    GetC();
    for(int i = 0;;i++){
        while(c >=0 && c <=' ')
             GetC();
         if (c == EOF)
             break;
         MethodS();
    }
    PrintAll();
}

var::var(string& name, bool val){
    this->name = name;
    this->val = val;
}
