#pragma once
#define DECLARE_G_OBJ(CLASS, BASE)\
public:\
using CLASS##Ptr = std::shared_ptr<CLASS>;\
using base = BASE;\
template <class ...Args>\
static CLASS##Ptr Make##CLASS##Ptr(Args&&... args){ return std::make_shared<CLASS>(args...);}

#define G_PTR(CLASS) CLASS::CLASS##Ptr

#define G_MAKE(CLASS) CLASS::Make##CLASS##Ptr