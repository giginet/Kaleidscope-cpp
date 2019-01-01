#include "GlobalVariables.h"
#include "ExprAST.hpp"
#include <vector>
#include "llvm.h"
#include "Utility.h"

llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(theContext, llvm::APFloat(value));
}

llvm::Value *VariableExprAST::codegen() {
    auto v = namedValues[name];
    if (v == nullptr) {
        logError<VariableExprAST>("Unknown variable name");
    }
    return v;
}

llvm::Value *BinaryExprAST::codegen() {
    auto l = lhs->codegen();
    auto r = rhs->codegen();
    
    if (l == nullptr || r == nullptr) {
        return nullptr;
    }
    
    switch (op) {
        case '+':
            return builder.CreateFAdd(l, r, "addtmp");
        case '-':
            return builder.CreateFAdd(l, r, "subtmp");
        case '*':
            return builder.CreateFAdd(l, r, "multmp");
        case '<':
            l = builder.CreateFCmpULT(l, r, "cmptmp");
            return builder.CreateUIToFP(l, llvm::Type::getDoubleTy(theContext), "booltmp");
        default:
            logError<VariableExprAST>("invalid binary operator");
            return nullptr;
    }
}

llvm::Value *CallExprAST::codegen() {
    auto calleeF = theModule->getFunction(callee);
    if (calleeF == nullptr) {
        logError<CallExprAST>("Unknown function referenced");
        return nullptr;
    }
    
    if (calleeF->arg_size() != args.size()) {
        logError<CallExprAST>("Incorrect # argument passed");
        return nullptr;
    }
    
    std::vector<llvm::Value *> argsv;
    for (unsigned i = 0, e = (unsigned)args.size(); i != e; ++i) {
        argsv.push_back(args[i]->codegen());
        if (argsv.back() == nullptr) {
            return nullptr;
        }
    }
    
    return builder.CreateCall(calleeF, argsv, "calltmp");
}

llvm::Function *PrototypeAST::codegen() {
    std::vector<llvm::Type *> doubles(0, llvm::Type::getDoubleTy(theContext));
    auto ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(theContext), doubles, false);
    auto f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, theModule.get());
    
    unsigned index = 0;
    for (auto &arg : f->args()) {
        arg.setName(args[++index]);
    }
    return f;
}

llvm::Function *FunctionAST::codegen() {
    auto theFunction = theModule->getFunction(prototype->getName());
    
    if (theFunction == nullptr) {
        theFunction = prototype->codegen();
    }
    
    if (theFunction == nullptr) {
        return nullptr;
    }
    
    if (!theFunction->empty()) {
        logError<llvm::Function>("Function cannot be redefined");
        return nullptr;
    }
    
    auto bb = llvm::BasicBlock::Create(theContext, "entry", theFunction);
    builder.SetInsertPoint(bb);
    
    namedValues.clear();
    for (auto &args : theFunction->args()) {
        namedValues[args.getName()] = &args;
    }
    
    if (auto retval = body->codegen()) {
        builder.CreateRet(retval);
        
        llvm::verifyFunction(*theFunction);
        
        return theFunction;
    }
    
    theFunction->eraseFromParent();
    return nullptr;
}
