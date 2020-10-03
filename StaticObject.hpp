//
// Created by JunMin Kim on 2020/10/03.
//

#ifndef STATEMACHINE_STATICOBJECT_HPP
#define STATEMACHINE_STATICOBJECT_HPP

template <class Type>
class RegisterStaticObject
{
private:
    int64_t ClassID;
public:
    static RegisterStaticObject& GetStaticObject()
    {
        static RegisterStaticObject Object;
        return Object;
    }

    static void SetStaticObject(const int64_t Object)
    {
        GetStaticObject().ClassID = Object;
    }

    static uint64_t StaticClassID()
    {
       return GetStaticObject().ClassID;
    }
};

#define SET_OBJECT(ObjectName)                                          \
    ObjectName::GetStaticObject().SetStaticObject(std::hash<std::string>()(#ObjectName))

#endif //STATEMACHINE_STATICOBJECT_HPP
