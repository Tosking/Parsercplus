#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <format>

using namespace std;

struct var{
    public:
        string name;
        long val;
        var(string& name, long val);
};

ifstream in;

class Parser{
    private:
        vector<var> varlist;
        int c = EOF;
        int last = -1;
        bool CheckA();
    public:
        inline void GetC();
        long GetVarValue(string& str);
        long& GetVarAdress(string str);

        long MethodC();
        void MethodS();
        long& MethodL();
        long MethodE();
        long MethodT();
        long MethodM();
        long MethodI();

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

long Parser::GetVarValue(string& str){
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

long& Parser::GetVarAdress(string str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].name == str){
            return varlist[i].val;
        }
    }

    varlist.push_back(var{str, 0});
    return varlist[varlist.size() - 1].val;
}

void Parser::MethodS(){
   long x = MethodL();
   if(c != '='){
       Error("Missing symbol '='!");
   }
   GetC();
   x = MethodE();
   GetC();
   if(c != ';'){
       Error("Missing symbol ';'!");
   }
   GetC();
}

long& Parser::MethodL(){
    string str;
    bool flag = 0;
    while(CheckA() || (c == '0' || c == '1')){
        str += c;
        GetC();
    }
    return GetVarAdress(str);
}

long Parser::MethodE(){
    long x = MethodT();
    while(c == '|'){
        GetC();
        x |= MethodT();
    }
    return x;
}

long Parser::MethodT(){
    long x = MethodM();
    while(c == '&'){
        GetC();
        x &= MethodM();
    }
    return x;
}

long Parser::MethodM(){
    if(c == '~'){
        GetC();
        return ~MethodM();
    }
    if(c == '('){
        GetC();
        long x = MethodE();
        GetC();
        if(c != ')'){
            Error("Invalid Syntax!");
            return 0;
        }
        else{
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

long Parser::MethodI(){
    string str;
    bool flag = 0;
    while(CheckA() || (c == '0' || c == '1')){
        str += c;
        GetC();
    }
    return GetVarValue(str);
}

long Parser::MethodC(){
    return c;
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
         cout << "Operator:" << i << endl;
         MethodS();
         Print();
    }
}

var::var(string& name, long val){
    this->name = name;
    this->val = val;
}
