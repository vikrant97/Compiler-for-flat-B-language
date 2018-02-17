#include "AST.h"
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
map <string,ASTStatement*> labelMap;
string label="";
int goto_flag=0;

llvm::Module* module=new Module("FlatB", llvm::getGlobalContext());
llvm::LLVMContext& context = getGlobalContext();
llvm::IRBuilder<> Builder(context);
FunctionType *funcType=llvm::FunctionType::get(Builder.getVoidTy(),false);
Function *fooFunc=llvm::Function::Create(funcType,llvm::Function::ExternalLinkage,"main",module);
Function *printF = module->getOrInsertFunction("printf",FunctionType::get(IntegerType::getInt32Ty(getGlobalContext()),true));
Function *scanF = module->getOrInsertFunction("scanf",FunctionType::get(IntegerType::getInt32Ty(getGlobalContext()),true));

void codegen::visit(class ASTProgram * node) {
    //llvm
    
    BasicBlock *entry=BasicBlock::Create(context,"B1",fooFunc);
    Builder.SetInsertPoint(entry);
    //
    if(node->getFieldDecls()!=NULL)
    {
        for(auto it : *(node->getFieldDecls())) {
            it->accept(this);
        }
    }
    if(node->getStatements()!=NULL)
    {
        vector<ASTStatement*> v;
        v=*(node->getStatements());
        std::vector<ASTStatement*>::iterator it;
        std::vector<ASTStatement*>::iterator itr;
        it=v.begin();
        while(it!=v.end()) 
        {
            (*it)->accept(this);
            it++;
        }
    }
    Builder.CreateRetVoid();
    return;
}
void codegen::visit(class ASTIdComma * node){
    return;
}
void codegen::visit(class ASTFieldDeclaration * node){
    if(node->getIdList()){
        for(auto it: *(node->getIdList())){
            it->accept(this,0);
        }
    }
    return;
}
void codegen::visit(class ASTStatement * node){
    return ;
}    
void codegen::visit(class ASTVarIdentifier * node,int flag){
    string var_name=node->getId();
    //from the declaration block
    if (flag==0){
        //llvm code
        llvm::Type* ty=Type::getInt32Ty(context);
        llvm::PointerType* ptrTy=PointerType::get(ty,0);
        module->getOrInsertGlobal(var_name,Builder.getInt32Ty());
        GlobalVariable* gv = module->getNamedGlobal(var_name);
        gv->setLinkage(GlobalValue::CommonLinkage);
        ConstantInt* const_int_val = ConstantInt::get(context, APInt(32,0));
        gv->setInitializer(const_int_val);
        //
    }
    //from the read statement
    else if(flag==1){
        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        Value* addr=module->getGlobalVariable(var_name);
        Value* v=addr;
        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,true);
        // Function *CalleeF = module->getOrInsertFunction("scanf",funcType);
        Builder.CreateCall(scanF, ArgsV);
    }
    //from print statement
    else if(flag==2){
        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        Value* addr=module->getGlobalVariable(var_name);
        Value* v=Builder.CreateLoad(addr);
        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    //from pritln statement
    else if(flag==3){
        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        Value* addr=module->getGlobalVariable(var_name);
        Value* v=Builder.CreateLoad(addr);
        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    // cout<<var<<endl; 
    return;
}
void codegen::visit(class ASTNumIdentifier * node,int flag){
    int value=node->getNum();
    //from print statement
    if(flag==2){
        Value* v = ConstantInt::get(getGlobalContext(), llvm::APInt(32,value));
        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());
        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    //from println statement
    else if(flag==3){
        Value* v = ConstantInt::get(getGlobalContext(), llvm::APInt(32,value));
        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());
        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    return;
}
void codegen::visit(class ASTArrayIdentifier * node,int flag){
    string arr_name=node->getArrayId1();
    int size=node->getArraySize1();
    Value* index=ConstantInt::get(getGlobalContext(), llvm::APInt(32,size));
    //when called from field decl block
    if(flag==0){
        //llvm code
        llvm::Type *ty=Type::getInt32Ty(context);
        llvm::ArrayType *arrType=ArrayType::get(ty,size);
        llvm::PointerType *ptrType=PointerType::get(arrType,0);
        GlobalVariable* gv= new GlobalVariable(*module,arrType,false,GlobalValue::ExternalLinkage,0,arr_name);
        gv->setInitializer(ConstantAggregateZero::get(arrType));
        //
    }
    //when called from read statement
    else if(flag==1){
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");

        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        ArgsV.push_back(V);     
        // argTypes.push_back(V->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,true);
        // Function *CalleeF = module->getOrInsertFunction("scanf",funcType);
        Builder.CreateCall(scanF, ArgsV);
    }
    //when called from print statement
    else if(flag==2){
        Value* index = ConstantInt::get(getGlobalContext(), llvm::APInt(32,size));
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* v=Builder.CreateLoad(V);

        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    //from println statement
    else if(flag==3){
        Value* index = ConstantInt::get(getGlobalContext(), llvm::APInt(32,size));
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* v=Builder.CreateLoad(V);

        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    return;
}
void codegen::visit(class ASTArrayIdentifier2 * node,int flag){
    string arr_name=node->getArrayId2();
    string s=node->getArraySize2();
    Value *addr=module->getGlobalVariable(s);
    Value *index=Builder.CreateLoad(addr);
    //from read statement
    if(flag==1){
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");

        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        ArgsV.push_back(V);     
        // argTypes.push_back(V->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,true);
        // Function *CalleeF = module->getOrInsertFunction("scanf",funcType);
        Builder.CreateCall(scanF, ArgsV);
    }
    //from print statement
    else if(flag==2){
        // Value* index = ConstantInt::get(getGlobalContext(), llvm::APInt(32,size));
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* v=Builder.CreateLoad(V);

        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
        }
    //from println statement
    else if(flag==3){
        // Value* index = ConstantInt::get(getGlobalContext(), llvm::APInt(32,size));
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* v=Builder.CreateLoad(V);

        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%d ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());

        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);
    }
    return; 
}
void codegen::visit(class ASTPrintString * node, int flag){
    string value=node->getValue();
    char c='"';
    value.erase(remove(value.begin(),value.end(),c),value.end());
    //from print statement
    if(flag==2){
        Value* v=Builder.CreateGlobalString(value);
        std::vector<Value *> ArgsV;
        vector<llvm::Type *> argTypes;
        Value* str_val=Builder.CreateGlobalStringPtr("%s ");
        ArgsV.push_back(str_val);
        // argTypes.push_back(str_val->getType());
        ArgsV.push_back(v);     
        // argTypes.push_back(v->getType());
        // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
        // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
        Builder.CreateCall(printF, ArgsV);

    }
    //from println statement
    else if(flag==3){
    Value* v=Builder.CreateGlobalStringPtr(value);
    std::vector<Value *> ArgsV;
    vector<llvm::Type *> argTypes;
    Value* str_val=Builder.CreateGlobalStringPtr("%s ");
    ArgsV.push_back(str_val);
    // argTypes.push_back(str_val->getType());
    ArgsV.push_back(v);     
    // argTypes.push_back(v->getType());
    // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
    // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
    Builder.CreateCall(printF, ArgsV);
    }
}
void codegen::visit(class ASTNewLineStatement * node){
    for(auto it: *(node->getIdList())){
        it->accept(this,3);
    }
    string value="\n";
    Value* v=Builder.CreateGlobalStringPtr(value);
    std::vector<Value *> ArgsV;
    vector<llvm::Type *> argTypes;
    Value* str_val=Builder.CreateGlobalStringPtr("%s");
    ArgsV.push_back(str_val);
    // argTypes.push_back(str_val->getType());
    ArgsV.push_back(v);     
    // argTypes.push_back(v->getType());
    // FunctionType *funcType=llvm::FunctionType::get(Builder.getInt32Ty(),argTypes,false);
    // Function *CalleeF = module->getOrInsertFunction("printf",funcType);
    Builder.CreateCall(printF, ArgsV);
    return;
}
void codegen::visit(class ASTReadStatement * node){
    for(auto it: *(node->getIdList())){
        it->accept(this,1);
    }
    return;
}
void codegen::visit(class ASTPrintStatement * node){
    for(auto it: *(node->getIdList())){
        it->accept(this,2);
    }
    return;
}
void codegen::visit(class ASTAssignmentStatement * node){
    string oper=node->getOper();
    Value* val=node->getExpr()->accept(this);
    // cout<<val<<endl;
    node->getInitial()->accept(this,val,oper);
    return;
}
void codegen::visit(class ASTVarInitial * node,Value* val,string oper){
    string var_name=node->getId();
    if(oper=="=")
    {
        Value* addr=module->getGlobalVariable(var_name);
        Value* cur=Builder.CreateStore(val,addr);
    }
    else if(oper=="+=")
    {
        Value* addr=module->getGlobalVariable(var_name);
        Value* cur=Builder.CreateLoad(addr);
        Value* update=Builder.CreateAdd(cur,val);
        Value* uval=Builder.CreateStore(update,addr);
    }
    else if(oper=="-=")
    {
        Value* addr=module->getGlobalVariable(var_name);
        Value* cur=Builder.CreateLoad(addr);
        Value* update=Builder.CreateSub(cur,val);
        Value* uval=Builder.CreateStore(update,addr);
    }
    else if(oper=="*=")
    {
        Value* addr=module->getGlobalVariable(var_name);
        Value* cur=Builder.CreateLoad(addr);
        Value* update=Builder.CreateMul(cur,val);
        Value* uval=Builder.CreateStore(update,addr);
    }
    else if(oper=="/=")
    {
        Value* addr=module->getGlobalVariable(var_name);
        Value* cur=Builder.CreateLoad(addr);
        Value* update=Builder.CreateUDiv(cur,val);
        Value* uval=Builder.CreateStore(update,addr);
    }
    return;
}
void codegen::visit(class ASTArrayInitial * node,Value* val,string oper){
    string arr_name=node->getId();
    Value* index=node->getExpr()->accept(this);
    if(oper=="=")
    {
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* cur=Builder.CreateStore(val,V);
    }
    else if(oper=="+=")
    {
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* cur=Builder.CreateLoad(V);
        Value* update=Builder.CreateAdd(cur,val);
        Value* uval=Builder.CreateStore(update,V);
    }
    else if(oper=="-=")
    {
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* cur=Builder.CreateLoad(V);
        Value* update=Builder.CreateSub(cur,val);
        Value* uval=Builder.CreateStore(update,V);
    }
    else if(oper=="*=")
    {
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* cur=Builder.CreateLoad(V);
        Value* update=Builder.CreateMul(cur,val);
        Value* uval=Builder.CreateStore(update,V);
    }
    else if(oper=="/=")
    {
        Value* V=module->getGlobalVariable(arr_name);
        vector<Value*> array_index;
        array_index.push_back(Builder.getInt32(0));
        array_index.push_back(index);
        V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
        Value* cur=Builder.CreateLoad(V);
        Value* update=Builder.CreateUDiv(cur,val);
        Value* uval=Builder.CreateStore(update,V);
    }
    return;
}
// void codegen::visit(class ASTLocation * node){
//     return;
// }
Value* codegen::visit(class ASTVarLocation * node){
    string var_name=node->getId();
    Value* addr=module->getGlobalVariable(var_name);
    Value* v=Builder.CreateLoad(addr);
    return v; 
}
Value* codegen::visit(class ASTArrayLocation * node){
    string arr_name=node->getId();
    Value* V=module->getGlobalVariable(arr_name);
    Value* index=node->getExpr()->accept(this);
    vector<Value*> array_index;
    array_index.push_back(Builder.getInt32(0));
    array_index.push_back(index);
    V = Builder.CreateGEP(V, array_index, arr_name+"_Index");
    Value* v=Builder.CreateLoad(V);
    return v;
}
// void codegen::visit(class ASTExpression * node){
//     return;
// }
Value* codegen::visit(class ASTBinaryExpression * node){
    string oper=node->getOper();
    Value* left=node->getLhs()->accept(this);
    Value* right=node->getRhs()->accept(this);
    Value* v;
    if(oper=="+")
        v=Builder.CreateAdd(left,right,"addtmp");
    else if(oper=="-")
        v=Builder.CreateSub(left,right,"subtmp");
    else if(oper=="*")
        v=Builder.CreateMul(left,right,"multmp");
    else if(oper=="/")
        v=Builder.CreateUDiv(left,right,"divtmp");
    else if(oper=="%")
        v=Builder.CreateURem(left,right,"modtmp");
    else if (oper == "<")
        v = Builder.CreateICmpULT(left,right,"ltcomparetmp");
    else if (oper == ">")
        v = Builder.CreateICmpUGT(left,right,"gtcomparetmp");
    else if (oper == "<=")
        v = Builder.CreateICmpULE(left,right,"lecomparetmp");
    else if (oper == ">=")
        v = Builder.CreateICmpUGE(left,right,"gecomparetmp");
    else if (oper == "==")
        v = Builder.CreateICmpEQ(left,right,"equalcomparetmp");
    else if (oper == "!=")
        v = Builder.CreateICmpNE(left,right,"notequalcomparetmp");
    return v;
}
// void codegen::visit(class ASTLiteral * node){
//     return ;
// }
Value* codegen::visit(class ASTIntLiteral * node){
    Value* v = ConstantInt::get(getGlobalContext(), llvm::APInt(32,node->getValue()));
    return v;
    // return node->getValue();
}
void codegen::visit(class ASTIfStatement * node){
    Value* cond=node->getExpr()->accept(this);
    BasicBlock* ifBlock=BasicBlock::Create(context,"if",fooFunc);
    BasicBlock* nextBlock=BasicBlock::Create(context,"next",fooFunc);
    Builder.CreateCondBr(cond,ifBlock,nextBlock);
    Builder.SetInsertPoint(ifBlock);
    for(auto it : *(node->getStatements())) 
        it->accept(this);
    Builder.CreateBr(nextBlock);
    Builder.SetInsertPoint(nextBlock);
    return ;
}
void codegen::visit(class ASTIfElseStatement * node){
    Value* cond=node->getExpr()->accept(this);
    BasicBlock* ifBlock=BasicBlock::Create(context,"if",fooFunc);
    BasicBlock* elseBlock=BasicBlock::Create(context,"else",fooFunc);
    BasicBlock* nextBlock=BasicBlock::Create(context,"next",fooFunc);
    Builder.CreateCondBr(cond,ifBlock,elseBlock);
    Builder.SetInsertPoint(ifBlock);
    for(auto it : *(node->getIfStatements())) 
        it->accept(this);
    Builder.CreateBr(nextBlock);
    Builder.SetInsertPoint(elseBlock);
    for(auto it : *(node->getElseStatements()))
        it->accept(this);
    Builder.CreateBr(nextBlock);
    Builder.SetInsertPoint(nextBlock);
    return ;
}
void codegen::visit(class ASTWhileStatement * node){
    Value* cond=node->getExpr()->accept(this);
    BasicBlock* whileBlock=BasicBlock::Create(context,"while",fooFunc);
    BasicBlock* nextBlock=BasicBlock::Create(context,"next",fooFunc);
    Builder.CreateCondBr(cond,whileBlock,nextBlock);
    Builder.SetInsertPoint(whileBlock);
    for(auto it : *(node->getStatements()))
        it->accept(this); 
    cond=node->getExpr()->accept(this); 
    Builder.CreateCondBr(cond,whileBlock,nextBlock);  
    Builder.SetInsertPoint(nextBlock);
    return ;
}
void codegen::visit(class ASTForStatement1 * node){
    int start=node->getNum1();
    int end=node->getNum2();
    int increment=1;
    string var_name=node->getId();

    BasicBlock* forBlock=BasicBlock::Create(context,"for",fooFunc);
    BasicBlock* nextBlock=BasicBlock::Create(context,"next",fooFunc);

    Value* addr=module->getGlobalVariable(var_name);
    Value* start_val=ConstantInt::get(getGlobalContext(),APInt(32,start));
    Value* end_val=ConstantInt::get(getGlobalContext(),APInt(32,end));
    Value* step_val = ConstantInt::get(getGlobalContext(),APInt(32,increment));
    Value* loc=Builder.CreateStore(start_val,addr);
    
    Value* cur_val=Builder.CreateLoad(addr);
    Value* cond=Builder.CreateICmpUGT(cur_val,end_val,"gtcomparetmp");
    Builder.CreateCondBr(cond,nextBlock,forBlock);
    Builder.SetInsertPoint(forBlock);
    for(auto it : *(node->getStatements()))
        it->accept(this);
    cur_val=Builder.CreateLoad(addr);
    Value* new_val=Builder.CreateAdd(cur_val,step_val);
    loc=Builder.CreateStore(new_val,addr);
    cond=Builder.CreateICmpUGT(new_val,end_val,"gtcomparetmp");;
    Builder.CreateCondBr(cond,nextBlock,forBlock);
    Builder.SetInsertPoint(nextBlock);
    return ;
}
void codegen::visit(class ASTForStatement2 * node){
    int start=node->getNum1();
    int end=node->getNum2();
    int increment=node->getNum3();
    string var_name=node->getId();

    BasicBlock* forBlock=BasicBlock::Create(context,"for",fooFunc);
    BasicBlock* nextBlock=BasicBlock::Create(context,"next",fooFunc);

    Value* addr=module->getGlobalVariable(var_name);
    Value* start_val=ConstantInt::get(getGlobalContext(),APInt(32,start));
    Value* end_val=ConstantInt::get(getGlobalContext(),APInt(32,end));
    Value* step_val = ConstantInt::get(getGlobalContext(),APInt(32,increment));
    Value* loc=Builder.CreateStore(start_val,addr);
    
    Value* cur_val=Builder.CreateLoad(addr);
    Value* cond=Builder.CreateICmpUGT(cur_val,end_val,"gtcomparetmp");
    Builder.CreateCondBr(cond,nextBlock,forBlock);
    Builder.SetInsertPoint(forBlock);
    for(auto it : *(node->getStatements()))
        it->accept(this);
    cur_val=Builder.CreateLoad(addr);
    Value* new_val=Builder.CreateAdd(cur_val,step_val);
    loc=Builder.CreateStore(new_val,addr);
    cond=Builder.CreateICmpUGT(new_val,end_val,"gtcomparetmp");
    Builder.CreateCondBr(cond,nextBlock,forBlock);
    Builder.SetInsertPoint(nextBlock);
    return ;
}
void codegen::visit(class ASTLabelStatement * node){
    // cout<<"reached\n";
    return ;
}
void codegen::visit(class ASTCondGotoStatement * node){
    label=node->getId();
    goto_flag=node->getExpr()->accept(this);
    // ASTStatement* statement=labelMap[label];
    return ;
}
void codegen::visit(class ASTUncondGotoStatement * node){
    //cout<<"reached\n";
    label=node->getId();
    goto_flag=1;
    return ;
}