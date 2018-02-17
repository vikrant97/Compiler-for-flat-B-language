#include <bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>
using namespace std;
using namespace llvm;
class codegen{
     public:
          void visit (class ASTProgram *);
          void visit (class ASTIdComma *);
          void visit (class ASTStatement *);
          void visit (class ASTFieldDeclaration *);
          void visit (class ASTVarIdentifier *,int flag);
          void visit (class ASTNumIdentifier *,int flag);
          void visit (class ASTArrayIdentifier *,int flag);
          void visit (class ASTArrayIdentifier2 *,int flag);
          void visit (class ASTPrintString *,int flag);
          void visit (class ASTNewLineStatement *);
          void visit (class ASTReadStatement *);
          void visit (class ASTPrintStatement *);
          void visit (class ASTAssignmentStatement *);
          void visit (class ASTInitial *);
          void visit (class ASTVarInitial *,Value* val,string oper);
          void visit (class ASTArrayInitial *, Value* val,string oper);
          void visit (class ASTLocation *);
          Value* visit (class ASTVarLocation *);
          Value* visit (class ASTArrayLocation *);
          void visit (class ASTExpression *);
          Value* visit (class ASTBinaryExpression *);
          void visit (class ASTLiteral *);
          Value* visit (class ASTIntLiteral *);
          void visit (class ASTIfStatement *);
          void visit (class ASTIfElseStatement *);
          void visit (class ASTWhileStatement *);
          void visit (class ASTForStatement1 *);
          void visit (class ASTForStatement2 *);
          void visit (class ASTLabelStatement *);
          void visit (class ASTCondGotoStatement *);
          void visit (class ASTUncondGotoStatement *);
};