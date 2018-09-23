/*
//	Reflect.h C++������
*/
#pragma once
#include <map>
#include <iostream>
#include <string>
using namespace std;

typedef void* (*PTRCreateObject)(void);  

class ClassFactory{
private:  
    map<string, PTRCreateObject> m_classMap ;  
    ClassFactory(){}; //���캯��˽�л�

public:   
    void* getClassByName(string className);  
    void registClass(string name, PTRCreateObject method) ;  
    static ClassFactory& getInstance() ;  
};

class RegisterAction{
public:
    RegisterAction(string className,PTRCreateObject ptrCreateFn){
        ClassFactory::getInstance().registClass(className,ptrCreateFn);
    }
};

#define REGISTER(className)                                             \
    className* objectCreator##className(){                              \
        return new className;                                           \
    }                                                                   \
    RegisterAction g_creatorRegister##className(                        \
        #className,(PTRCreateObject)objectCreator##className);
