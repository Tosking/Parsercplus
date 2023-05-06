#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Base {
    public:
        string value;
        virtual string GetType() {return "@";}
};

class Constant : public Base {
    public:
        string GetType() {return "C";}
        Constant(string value){ this->value = value; }
};

class Variable : public Base {
    public:
        string name;
        string GetType() {return "V";}
        Variable(string value){ this->value = value; }
};

class Reference : public Base {
    public:
        int id;
        Reference(int id){this->id = id;}
};

class Triad {
    public:
        char op;
        Base* op1;
        Base* op2;
        bool deleted = false;
        Triad(char op, Base* op1, Base* op2){this->op = op; this->op1 = op1; this->op2 = op2;}
};

ifstream in;

class Parser{
    private:
        vector<Variable> varlist;
        vector<Triad> triadlist;
        int c = EOF;
        int last = -1;
        int CheckA();
        int TriadCount = 1;
    public:
        inline void GetC();
        int GetVarValue(string& str);
        Variable GetVarAdress(string str);

        int MethodC();
        void MethodS();
        Variable MethodL();
        int MethodE();
        int MethodT();
        int MethodM();
        int MethodI();

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

int Parser::CheckA(){
    return (c>='a' && c<='z') || c=='_';
}

inline void Parser::GetC(){
    c = in.get();
}

int Parser::GetVarValue(string& str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].name == str){
            Triad triad("V", varlist[i], new Base);
            return TriadCount - 1;
        }
    }
    Error("Invalid Indentifer!");
    return 0;
}

Variable Parser::GetVarAdress(string str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].name == str){
            return varlist[i];
        }
    }
    Variable var = Variable(NULL);
    var.name = str;
    varlist.push_back(var);
    return varlist[varlist.size() - 1].triad;
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
   x.name = y;
   GetC();
}

Variable Parser::MethodL(){
    string str;
    int flag = 0;
    while(CheckA() || (c == '0' || c == '1')){
        str += c;
        GetC();
    }
    return GetVarAdress(str);
}

int Parser::MethodE(){
    int x = MethodT();
    int y;
    while(c == '|'){
        GetC();
        y = MethodT();
        Triad triad('|', new Reference {x}, new Reference {y});
        triadlist.push_back(triad);
        TriadCount++;
        x = TriadCount;
    }
    return x;
}

int Parser::MethodT(){
    int x = MethodM();
    int y;
    while(c == '&'){
        GetC();
        y = MethodM();
        Triad triad('&', new Reference {x}, new Reference {y});
        triadlist.push_back(triad);
        TriadCount++;
        x = TriadCount;
    }
    return x;
}

int Parser::MethodM(){
    if(c == '~'){
        GetC();
        int x = MethodM();
        Triad triad('~', new Reference {x}, new Base);
        triadlist.push_back(triad);
        TriadCount++;
        return TriadCount;
    }
    if(c == '('){
        GetC();
        int x = MethodE();
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

int Parser::MethodI(){
    string str;
    while(CheckA() || (c == '0' || c == '1')){
        str += c;
        GetC();
    }
    return GetVarValue(str);
}

int Parser::MethodC(){
    Triad triad('C', new Constant(("" + (char)c)), new Base);
    triadlist.push_back(triad);
    TriadCount++;
    GetC();
    return TriadCount;
}
void Parser::Print()
{
    if (varlist.size() == 0)
        cout << "No varibales defined yet.\n";
    else
        cout << varlist[varlist.size()-1].name << " = " << varlist[varlist.size()-1].value << endl;
}

void Parser::PrintAll()
{
    if (!varlist.size())
        cout << "No variables defined yet." << endl;
    else
        for (int i = 0; i < varlist.size(); i++)
            cout<<varlist[i].name<< " = " << varlist[i].value<<endl;
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
}

