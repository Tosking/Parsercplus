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
        bool CheckD();
    public:
        inline void GetC();
        long GetVarValue(string& str);
        long& GetVarAdress(string name);

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


void Error(string msg){
    cout << "Error!:" << msg << endl;
    exit(1);
}

bool Parser::CheckD(){
    return (c>='a' && c<='z') || c=='_';
}

inline void Parser::GetC(){
    c = in.get();
}

long Parser::GetVarValue(string& str){
    if(str.empty() || str.size()==0){
        Error("Invalid Syntax!");
    }
    for(int i = 0; i < varlist.length(); i++){
        if(varlist[i] == str){
            return varlist[i].val;
        }
    }
    Error("Unvalid Indentifer!");
    return 0;
}
