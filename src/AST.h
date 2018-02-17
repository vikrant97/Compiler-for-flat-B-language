#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Visitor.h"
#include <bits/stdc++.h>
// #include "global.h"
using namespace std;
using namespace llvm;
// #include "SymbolTable.h"
// extern "C" ofstream outputFile;
#define TAB 1
extern map <string,ASTStatement*> labelMap;
union Node{
    int ival;
    bool bval;
    char cval;
    char *sval;
    ASTProgram* program;
    vector<ASTFieldDeclaration*>* field_declarations;
    vector<ASTIdComma*>* id_comma;
    ASTFieldDeclaration* field_declaration;
    vector<ASTStatement*>* statements;
    ASTStatement* statement;
    ASTLocation* location;
    ASTInitial* initial;
    ASTExpression* expr;
    ASTLiteral* literal;
};
typedef union Node YYSTYPE;

// // arithematic operations
enum class arithematic_op {
    PLUS,  
    MINUS, 
    MULT,
    DIV,  
    MOD,
    LT,
    GT,
    LTE,
    GTE,
    EQUAL,
    NE
};

// // assignment orperators
enum class assign_op {
    ASSIGN_OP,
    PLUSE,
    MINUSE,
    DIVE,
    MULTE
};

class ASTProgram {
    vector<ASTFieldDeclaration*> *fieldDecls;
    vector<ASTStatement*> *statements;
    public:
        ASTProgram(vector<ASTFieldDeclaration*>* fieldDecls, vector<ASTStatement*>* statements) {
            this->fieldDecls = fieldDecls;
            this->statements = statements;
            // cout<<this->statements<<endl;
        }
        
        ~ASTProgram() {
        }
        
        vector<ASTFieldDeclaration*>* getFieldDecls() {
            return this->fieldDecls;
        }
        
        vector<ASTStatement*>* getStatements() {
            return this->statements;
        }
        void accept(codegen * v) { 
                return v -> visit(this); 
        }
};


class ASTFieldDeclaration {
    string type;
    vector < ASTIdComma *> * idList;
        public:
            ASTFieldDeclaration(string type, vector < ASTIdComma *> * idList){
                this -> type = type;
                this -> idList = idList;
            }
            ~ASTFieldDeclaration(){}
            string getType(){ return this -> type; }
            vector < ASTIdComma *> * getIdList(){ return this -> idList; }
            void  accept(codegen * v) { 
                return v -> visit(this); 
            }
};

class ASTIdComma {
    public:
        ASTIdComma(){};
        ~ASTIdComma(){};
        virtual void accept(codegen * v,int flag) = 0;
};

class ASTVarIdentifier : public ASTIdComma{
    string id;
    public:
        ASTVarIdentifier(string id) { 
            this -> id = id;
        };
        ~ASTVarIdentifier(){};

        string getId(){ return this -> id; }
        void accept(codegen * v,int flag) { 
            return v -> visit(this,flag);
        }
};

class ASTNumIdentifier : public ASTIdComma{
    int num;
    public:
        ASTNumIdentifier(int num) { 
            this -> num = num;
        };
        ~ASTNumIdentifier(){};

        int getNum(){ return this -> num; }
        void accept(codegen * v, int flag) { 
            return v -> visit(this,flag);
        }
};

class ASTArrayIdentifier : public ASTIdComma{
    string id;
    int size;
    public:
        ASTArrayIdentifier(string id, int size){
            this -> id = id;
            this -> size = size;
        }
        ~ASTArrayIdentifier(){};

        string getArrayId1(){ return this -> id; }
        int getArraySize1(){ return this -> size; }
        void accept(codegen * v,int flag) { 
            return v -> visit(this,flag);
        }
};

class ASTArrayIdentifier2 : public ASTIdComma{
    string id;
    string size;
    public:
        ASTArrayIdentifier2(string id, string size){
            this -> id = id;
            this -> size = size;
        }
        ~ASTArrayIdentifier2(){};

        string getArrayId2(){ return this -> id; }
        string getArraySize2(){ return this -> size; }
        void accept(codegen * v,int flag) { 
            return v -> visit(this,flag);
        }
};

class ASTPrintString : public ASTIdComma{
    string value;
public:
    ASTPrintString(string value){
        this->value=value;
    }
    ~ASTPrintString(){};
    string getValue(){ return this->value; }
    void accept(codegen * v,int flag) { 
            return v -> visit(this,flag);
        }
};

class ASTStatement {
    public:
        ASTStatement(){};
        ~ASTStatement(){};
        virtual void accept(codegen * v) = 0;      
};

class ASTNewLineStatement: public ASTStatement {
    vector < ASTIdComma *> * idList;
    public:
        ASTNewLineStatement(vector < ASTIdComma *> * idList) {
             this -> idList = idList;
        }
        ~ASTNewLineStatement() {
        }
         vector < ASTIdComma *> * getIdList(){ return this -> idList; }
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTReadStatement: public ASTStatement {
    vector < ASTIdComma *> * idList;
        public:
            ASTReadStatement(vector < ASTIdComma *> * idList){
                this -> idList = idList;
            }
            ~ASTReadStatement(){}
            vector < ASTIdComma *> * getIdList(){ return this -> idList; }
            void accept(codegen * v) { 
                return v -> visit(this); 
            }
};

class ASTPrintStatement: public ASTStatement {
    vector < ASTIdComma *> * idList;
    public:
        ASTPrintStatement(vector < ASTIdComma *> * idList) {
            this -> idList = idList;
        }
        ~ASTPrintStatement() {
        }
         vector < ASTIdComma *> * getIdList(){ return this -> idList; }
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTIfStatement: public ASTStatement {
    ASTExpression* expr;
    vector<ASTStatement*> *statements;
    public:
        ASTIfStatement(ASTExpression* expr, vector<ASTStatement*> *statements) {
            this->expr = expr;
            this->statements = statements;
        }
  
        ~ASTIfStatement() {
        }
  
        ASTExpression* getExpr() {
            return this->expr;
        }
        
        vector<ASTStatement*>*  getStatements() {
            return this->statements;
        }
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTIfElseStatement: public ASTStatement {
    ASTExpression* expr;
    vector<ASTStatement*> *ifstatements;
    vector<ASTStatement*> *elsestatements;
    public:
        ASTIfElseStatement(ASTExpression* expr, vector<ASTStatement*> *ifstatements, vector<ASTStatement*> *elsestatements) {
            this->expr = expr;
            this->ifstatements = ifstatements;
            this->elsestatements = elsestatements;
        }
  
        ~ASTIfElseStatement() {
        }
  
        ASTExpression* getExpr() {
            return this->expr;
        }
        
        vector<ASTStatement*>*  getIfStatements() {
            return this->ifstatements;
        }
        vector<ASTStatement*>*  getElseStatements() {
            return this->elsestatements;
        }
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTWhileStatement: public ASTStatement {
    ASTExpression* expr;
    vector<ASTStatement*> *statements;
    public:
        ASTWhileStatement(ASTExpression* expr, vector<ASTStatement*> *statements) {
            this->expr = expr;
            this->statements = statements;
        }
  
        ~ASTWhileStatement() {
        }
  
        ASTExpression* getExpr() {
            return this->expr;
        }
        
        vector<ASTStatement*>*  getStatements() {
            return this->statements;
        }
        void accept(codegen* v) {
            return v->visit(this);
        }
};
class ASTCondGotoStatement: public ASTStatement {
    string id;
    ASTExpression* expr;
    public:
        ASTCondGotoStatement(string id, ASTExpression* expr) {
            this->id=id;
            this->expr = expr;
        }
  
        ~ASTCondGotoStatement() {
        }
    
        string getId() {
            return this->id;
        }

        ASTExpression* getExpr() {
            return this->expr;
        }
        
        
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTUncondGotoStatement: public ASTStatement {
    string id;
    public:
        ASTUncondGotoStatement(string id) {
            this->id=id;
        }
  
        ~ASTUncondGotoStatement() {
        }
    
        string getId() {
            return this->id;
        }
        
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTForStatement1: public ASTStatement {
    string id;
    int num1;
    int num2;
    vector<ASTStatement*> *statements;
    public:
        ASTForStatement1(string id, int num1, int num2,vector<ASTStatement*> *statements) {
            this->id=id;
            this->num1=num1;
            this->num2=num2;
            this->statements = statements;
        }
  
        ~ASTForStatement1() {
        }
  
        string getId() {
            return this->id;
        }
        int getNum1(){
            return this->num1;
        }
        int getNum2(){
            return this->num2;
        }
        vector<ASTStatement*>*  getStatements() {
            return this->statements;
        }
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTForStatement2: public ASTStatement {
    string id;
    int num1;
    int num2;
    int num3;
    vector<ASTStatement*> *statements;
    public:
        ASTForStatement2(string id, int num1, int num2, int num3, vector<ASTStatement*> *statements) {
            this->id=id;
            this->num1=num1;
            this->num2=num2;
            this->num3=num3;
            this->statements = statements;
        }
  
        ~ASTForStatement2() {
        }
  
        string getId() {
            return this->id;
        }
        int getNum1(){
            return this->num1;
        }
        int getNum2(){
            return this->num2;
        }
        int getNum3(){
            return this->num3;
        }
        vector<ASTStatement*>*  getStatements() {
            return this->statements;
        }
        void accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTLabelStatement: public ASTStatement{
    string id;
    // cout<<"here"<<endl;
    ASTStatement* statement;
public:
    ASTLabelStatement(string id,ASTStatement* statement){
        this->id=id;
        // cout<<"here"<<endl;
        this->statement=statement;
        labelMap[id]=statement;
    }
    ~ASTLabelStatement(){}
    string getId(){
        return this->id;
    }
    ASTStatement* getStatement(){
        return this->statement;
    }
    void accept(codegen* v){
        return v->visit(this);
    }
};

class ASTAssignmentStatement : public ASTStatement {
    ASTInitial* initial;
    string oper;
    ASTExpression* expr;
    public:
        ASTAssignmentStatement(ASTInitial* initial, string oper, ASTExpression* expr) {
            this->initial = initial;
            this->oper = oper;
            this->expr = expr;
        }
        ~ASTAssignmentStatement() {

        }
        ASTInitial* getInitial() {
            return this->initial;
        }
        string getOper() {
            return this->oper;
        }
        ASTExpression* getExpr() {
            return this->expr;
        }
        void accept(codegen* v) {
            return v->visit(this);
        }

};

class ASTInitial{
    public:
        ASTInitial() {
        }
        ~ASTInitial() {
        }
        virtual void accept(codegen* v,Value* val,string oper) = 0;
        // virtual int accept(codegen* v);
};

class ASTVarInitial : public ASTInitial {
    string id;
    public:
        // cout<<"hello"<<<endl;
        ASTVarInitial(string id) {
            // cout<<"I am here"<<endl;
            this->id = id;
            // cout<<id<<endl;
        }
        ~ASTVarInitial(){
        }
        string getId() {
            return this->id;
        }
  
        void accept(codegen* v,Value* val,string oper) {
            return v->visit(this,val,oper);
        }
};

class ASTArrayInitial : public ASTInitial {
    string id;
    ASTExpression* expr;
    public:
        ASTArrayInitial(string id, ASTExpression* expr) {
            this->id = id;
            this->expr = expr;
        }
        ~ASTArrayInitial() {
        }
        string getId() {
            return this->id;
        }
        ASTExpression* getExpr(){
          return this -> expr;
        }
        void accept(codegen* v,Value* val,string oper) {
            return v->visit(this,val,oper);
        }
};


class ASTExpression {
      public:
        ASTExpression() {
        }
        ~ASTExpression() {
        }
        
        virtual Value* accept(codegen* v) = 0;
};


class ASTBinaryExpression : public ASTExpression {
    ASTExpression* lhs, *rhs;
    string oper;
    
    public:
        ASTBinaryExpression(ASTExpression* lhs, string oper, ASTExpression* rhs) {
            this->lhs = lhs;
            this->oper = oper;
            this->rhs = rhs;
        }
        
        ~ASTBinaryExpression() {
        }
        
        ASTExpression* getLhs() {
            return this->lhs;
        }
        
        ASTExpression* getRhs() {
            return this->rhs;
        }
        
        string getOper() {
            return this->oper;
        }
        
        Value* accept(codegen* v) {
            return v->visit(this);
        }
        
};

class ASTLocation : public ASTExpression {
    public:
        ASTLocation() {
        }
        ~ASTLocation() {
        }
        virtual Value* accept(codegen* v) = 0;
        // virtual int accept(codegen* v);
};

class ASTVarLocation : public ASTLocation {
    string id;
    public:
        // cout<<"hello"<<<endl;
        ASTVarLocation(string id) {
            // cout<<"I am here"<<endl;
            this->id = id;
            // cout<<id<<endl;
        }
        ~ASTVarLocation(){
        }
        string getId() {
            return this->id;
        }
  
        Value* accept(codegen* v) {
            return v->visit(this);
        }
};

class ASTArrayLocation : public ASTLocation {
    string id;
    ASTExpression* expr;
    public:
        ASTArrayLocation(string id, ASTExpression* expr) {
            this->id = id;
            this->expr = expr;
        }
        ~ASTArrayLocation() {
        }
        string getId() {
            return this->id;
        }
        ASTExpression* getExpr(){
          return this -> expr;
        }
        Value* accept(codegen* v) {
            return v->visit(this);
        }
};
class ASTLiteral : public ASTExpression {
    public:
        ASTLiteral() {
        }
        ~ASTLiteral() {
        }
        virtual Value* accept(codegen* v) = 0;
};

class ASTIntLiteral : public ASTLiteral {
    int value;
    public:
        ASTIntLiteral(int value) {
            this->value = value;
        }
        ~ASTIntLiteral() {
        }
        int getValue() {
            return this->value;
        }
        Value* accept(codegen* v) {
            return v->visit(this);
        }
};
