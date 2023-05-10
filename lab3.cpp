#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Base {
    public:
        virtual string GetOp() {return "@";}
        string value;
        virtual ~Base();
};

Base::~Base(){}

class Constant : public Base {
    public:
        char value;
        string GetOp() {return to_string(value);}
        Constant(char value){ this->value = value; }
};

class Variable : public Base {
    public:
        string GetType() {return "V";}
        string GetOp() {return value;}
        Variable(string value){ this->value = value; }
};

class Reference : public Base {
    public:
        int id;
        string GetOp() {return "^" + to_string(id);}
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
        int GetVarAdress(string str);

        int MethodC();
        void MethodS();
        int MethodL();
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
        if(varlist[i].GetOp() == str){
            Triad triad('V', new Variable (str), new Base{});
            triadlist.push_back(triad);
            return TriadCount++ - 1;
        }
    }
    Error("Invalid Indentifer!");
    return 0;
}

int Parser::GetVarAdress(string str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].GetOp() == str){
            return TriadCount++ - 1;
        }
    }
    Variable var = Variable(str);
    varlist.push_back(var);
    return TriadCount++ - 1;
}

void Parser::MethodS(){
   int x = MethodL();
   if(c != '='){
       Error("Missing symbol '='!");
   }
   GetC();
   int y = MethodE();
   if(c != ';'){
       Error("Missing symbol ';'!");
   }
   Triad triad('=', new Reference{x}, new Reference{y});
   triadlist.push_back(triad);
   GetC();
}

int Parser::MethodL(){
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
    Triad triad('C', new Constant{(char)c}, new Base{});
    triadlist.push_back(triad);
    TriadCount++;
    GetC();
    return TriadCount;
}

void Parser::PrintAll()
{
    if (!varlist.size())
        cout << "No variables defined yet." << endl;
    else
        for (int i = 0; i < triadlist.size(); i++)
            cout << triadlist[i].op << "(" << triadlist[i].op1->GetOp() << ", " << triadlist[i].op2->GetOp() << ")" << endl;
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

