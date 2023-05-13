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
        string GetOp() {return value;}
        Constant(string value){ this->value = value; }
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
        bool isDel = false;
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
        void Optimize();
        int MethodC();
        void MethodS();
        int MethodL();
        int MethodE();
        int MethodT();
        int MethodM();
        int MethodI();

        void Print();
        void PrintAll();
        void PrintAllB();
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
            Triad triad('V', new Variable {str}, new Base{});
            triadlist.push_back(triad);
            return TriadCount++;
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
            Triad triad('V', new Variable {str}, new Base{});
            triadlist.push_back(triad);
            return TriadCount++;
        }
    }
    Triad triad('V', new Variable (str), new Base{});
    triadlist.push_back(triad);
    Variable var = Variable(str);
    varlist.push_back(var);
    return TriadCount++;
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
   TriadCount++;
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
        x = TriadCount - 1;
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
        x = TriadCount - 1;
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
        return TriadCount - 1;
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
    Triad triad('C', new Constant{to_string(c - 48)}, new Base{});
    triadlist.push_back(triad);
    GetC();
    return TriadCount++;
}

void Parser::Optimize(){
    int x;
    int y;
    for(int i = 0; i < triadlist.size(); i++){
       x = atoi(triadlist[i].op1->GetOp().c_str() + 1);
       y = atoi(triadlist[i].op2->GetOp().c_str() + 1);
       x--;
       y--;
       if (triadlist[i].op == '|' || triadlist[i].op == '&' )
       {
           if (triadlist[y].op == 'C')
           {
               triadlist[y].isDel = true;
               delete triadlist[i].op2;
               triadlist[i].op2 = new Constant(triadlist[y].op1->GetOp());
               if (triadlist[x].op == 'C')
               {
                   triadlist[x].isDel = true;
                   triadlist[i].op1 = new Constant(triadlist[x].op1->GetOp());
                   if (triadlist[i].op == '|')
                   {
                       triadlist[i].op = 'C';
                       triadlist[i].op1 = new Constant(to_string(atoi(triadlist[i].op1->GetOp().c_str()) |
                                                                 (atoi(triadlist[i].op2->GetOp().c_str()))));
                       triadlist[i].op2 = new Base();
                   }
                   else
                   {
                       triadlist[i].op = 'C';
                       triadlist[i].op1 = new Constant(to_string(atoi(triadlist[i].op1->GetOp().c_str()) &
                                                                 (atoi(triadlist[i].op2->GetOp().c_str()))));
                       triadlist[i].op2 = new Base();
                   }
               }
           }
       }
       else if (triadlist[i].op == '~')
       {
           if (triadlist[x].op == 'C')
           {
               triadlist[x].isDel = true;
               triadlist[i].op1 = new Constant(to_string(~(atoi(triadlist[y].op1->GetOp().c_str()))));
               triadlist[i].op = 'C';
           }
       }
       else if (triadlist[i].op == '=')
       {
           if (triadlist[x].op == 'V')
           {
               triadlist[x].isDel = true;
               triadlist[i].op1 =new Variable(triadlist[x].op1->GetOp());
           }
       }
    }
}

void Parser::PrintAll()
{
    if (!varlist.size())
        cout << "No variables defined yet." << endl;
    else{
        cout << "After:" << endl;
        for (int i = 0; i < triadlist.size(); i++)
            if(!triadlist[i].isDel)
                cout << i+1 << ": " << triadlist[i].op << "(" << triadlist[i].op1->GetOp() << ", " << triadlist[i].op2->GetOp() << ")" << endl;
    }
}
void Parser::PrintAllB(){
    cout << "Before:" << endl;
    for (int i = 0; i < triadlist.size(); i++)
        cout << i+1 << ": " << triadlist[i].op << "(" << triadlist[i].op1->GetOp() << ", " << triadlist[i].op2->GetOp() << ")" << endl;
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
    PrintAllB();
    Optimize();
    PrintAll();
}

