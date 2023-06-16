#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

constexpr short int SIZE = 16;

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
        void OutTriad();
};

struct Rule
{
    char left;
    string right;
    Rule(char S, const std::string src);
};

struct Attitude
{
public:
    char CAttitude;
    char C;
    int Constant;
    std::string I;
    int triadid;
public:
    Attitude() { }
    Attitude(const char CAttitude,const char C):CAttitude(CAttitude),
        C(C){}
};



/*

Исходная грамматика:
S → I=E;
E → E “|” T | T
T → T & M | M
M → ~M | (E) | I | C
I → AK|A
K → AK|DK|A|D
C → DC | D
A → а | b | c | d | e | f | g | h | I | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z | _ 
D → 0 | 1

грамматика слабого предшествия:
S' → ꞱSꞱ
S → I=E;
E → E “|” T | T
T → T & M | M
M → ~M | (E) | I | C

*/

const Rule Rules[12] =
    {
        {'W',"#L#"},
        {'L',"LS"},
        {'L',"S"},
        {'S',"I=E;"},
        {'E',"E|T"},
        {'E',"T"},
        {'T',"T&M"},
        {'T',"M"},
        {'M',"~M"},
        {'M',"(E)"},
        {'M',"I"},
        {'M',"C"}
    };

const int rulesCount = sizeof(Rules) / sizeof(*Rules) - 1;

char Matrix[SIZE][SIZE] =
{
    {' ','L', 'S', 'E', 'T', 'M', '=', ';', '|', '&', '~', '(', ')', 'I', 'C','#'},
    {'L',' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<',' ', '='},
    {'S',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',' ', '>'},
    {'E',' ', ' ', ' ', ' ', ' ', ' ', '=', '=', ' ', ' ', ' ', '=', ' ',' ', ' '},
    {'T',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '=', ' ', ' ', '>', ' ',' ', ' '},
    {'M',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ',' ', ' '},
    {'=',' ', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<','<', ' '},
    {';',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',' ', '>'},
    {'|',' ', ' ', ' ', '$', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'&',' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'~',' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'(',' ', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {')',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'I',' ', ' ', ' ', ' ', ' ', '=', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'C',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'#','$', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', ' ',' '},
};

ifstream in;

class Parser{
    private:
        vector<Variable> varlist;
        vector<Triad> triadlist;
        vector<Attitude> attlist;
        int c = EOF;
        string name;
        int lex = EOF;
        int TriadCount = 0;
        int value;
        int CheckA();
        string operands = "=;~|&()";
    public:
        inline void GetC();
        int GetVarValue(string& src);
        int GetVarAdress(string src);
        char FindRules(string src);
        inline void GetLex();
        char FindElement(char Y);
        void Analize();
        char Convolution();
        int FindAttitude(const char C);
        void Check(string src,size_t i);

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

void Triad::OutTriad(){
    cout << op << '(' << op1->GetOp() << ", " << op2->GetOp() << ')' << std::endl;
}

int Parser::CheckA(){
    return (c>='a' && c<='z') || c=='_';
}

inline void Parser::GetC(){
    c = in.get();
}

int Parser::GetVarValue(string& src){
    if(src.empty() || src.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].GetOp() == src){
            Triad triad('V', new Variable {src}, new Base{});
            triadlist.push_back(triad);
            return TriadCount++;
        }
    }
    Error("Invalid Indentifer!");
    return 0;
}

int Parser::GetVarAdress(string src){
    if(src.empty() || src.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.size(); i++){
        if(varlist[i].GetOp() == src){
            Triad triad('V', new Variable {src}, new Base{});
            triadlist.push_back(triad);
            return TriadCount++;
        }
    }
    Triad triad('V', new Variable (src), new Base{});
    triadlist.push_back(triad);
    Variable var = Variable(src);
    varlist.push_back(var);
    return TriadCount++;
}

inline void Parser::GetLex(){
     while (isspace(c)) GetC();

     if (CheckA())
     {
         std::string temp;
         temp += c;
         GetC();
         while (CheckA() || isdigit(c))
         {
             temp += c;
             GetC();

         }
         name = temp;
        lex = 'I';
     
     }
     else if (c >= '0' && c <= '1')
     {
         long x = 0;
         while (c >='0' && c <='1')
         {
             x *= 2;
             x += c - '0';
             GetC();
         }
         value = x;
         lex = 'C';
     }
     else if (strchr(operands.c_str(), c))
     {
        lex = c;
        GetC();
     }
     else if (c == EOF)
        lex = '#';
}

char Parser::FindElement(char Y){
    char X = attlist.back().C;
     int i = 0, j = 0;
     while (Matrix[i][0] != X)
         i++;
     while (Matrix[0][j] != Y)
         j++;
     return Matrix[i][j];
}
void Parser::Analize(){
    char C = FindElement(lex);
     if (C != '>')
     {
         Attitude attitude(C, lex);
         if (lex == 'C')
         {
             attitude.Constant = value;
             Triad triad('C', new Constant(to_string(value)), new Base());
             triadlist.push_back(triad);
             TriadCount++;
             attitude.triadid = TriadCount;
         }
         else if (lex == 'I')
         {
             attitude.I = name;
             
             Triad triad('V', new Variable(name), new Base());
             triadlist.push_back(triad);
             TriadCount++;
             attitude.triadid = TriadCount;
         }
         attlist.push_back(attitude);
     }
         
     else
     {
         while (C == '>')
         {
             char result = Convolution();
             Attitude attitude(FindElement(result), result);
             attitude.triadid = TriadCount;
             attlist.push_back(attitude);
             C = FindElement(lex);
         }
         Attitude attitude(C, lex);
         if (lex == 'I')
         {
             attitude.I = name;

             Triad triad('V', new Variable(name), new Base());
             triadlist.push_back(triad);
             TriadCount++;
             attitude.triadid = TriadCount;
         }
         attlist.push_back(attitude);
     }
}

inline char Parser::FindRules(string src){

     for (int i = 0; i <= rulesCount; i++)
         if (src == Rules[i].right)
             return Rules[i].left;
     return NULL;
}

char Parser::Convolution(){
    std::string base;
     char result;
     int posL = 0,tempposL=0;
     size_t _sizeattlist = attlist.size();

     posL = FindAttitude('<');      
     if (posL == 0)
         posL = FindAttitude('$');

     while (true)
     {
         tempposL = posL;
         
         for(;posL< _sizeattlist;posL++)
            base.push_back(attlist[posL].C);
         
         Check(base,tempposL);
         result = FindRules(base);
         posL = tempposL;
         if (result != NULL)
         {
             for (posL; posL < _sizeattlist; posL++)
                 attlist.pop_back();
             return result;
         }

         else
         {
             base.clear();
             for (; posL < _sizeattlist; posL++)
                 if (attlist[posL].CAttitude == '$')
                     break;
            if (posL == attlist.size())
                Error("Couldn't find symbol < and <=");
         }

     }
}

 inline int Parser::FindAttitude(const char C){
     for (int j = attlist.size() - 1; j >= 1; j--)
         if (attlist[j].CAttitude == C)
             return j;
    Error("Cant Find Attitude");
 }

void Parser::Check(string src,size_t pos){
    
     if (src == "E|T")
     {
        
         Triad triad('|', new Reference(attlist[pos].triadid), new Reference(attlist[pos+2].triadid));
         triadlist.push_back(triad);
         TriadCount++;

     }
     else if (src == "T&M")
     {
         
         Triad triad('&', new Reference(attlist[pos].triadid), new Reference(attlist[pos+2].triadid));
         triadlist.push_back(triad);
         TriadCount++;

     }
     else if (src == "I=E;")
     {
        
       
         GetVarAdress(attlist[pos].I);
         Triad triad('=', new Reference(attlist[pos].triadid), new Reference(attlist[pos+2].triadid));
         triadlist.push_back(triad);
         TriadCount++;
     }
     else if (src == "I")
     {
        
         GetVarValue(attlist[pos].I);
     }
     else if (src == "~M")
     {
        
         Triad triad('~', new Reference(attlist[pos+1].triadid), new Base());
         triadlist.push_back(triad);
         TriadCount++;
     }
     else if (src == "(E)")
     {
        
         TriadCount = attlist[pos + 1].triadid;
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
     attlist.push_back(Attitude(' ', '#'));
     do
     {
         GetLex();
        if(c == -1)
            break;
         Analize();
         if (lex == ';')
         {
             for (auto it = attlist.begin(); it != attlist.end(); ++it)
                 std::cout << it->C;
             std::cout << std::endl;
         }
         
     } while (lex != '#');
    for (int i = 0; i < triadlist.size(); i++)
     {
        triadlist[i].OutTriad();
     }
}
Rule::Rule(char S,const std::string str):left(S),right(str){}