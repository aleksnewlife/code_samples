
#include <iostream>


class Object
{
public:
    Object()
    {
        std::cout << "Object created: " << int(this) << std::endl;
    }

    // Copy
    Object(const Object& a)
    {
        std::cout << "Object created by copy ctr: " << int(this) << std::endl;   
    }

    Object& operator=(const Object& a)
    {
        std::cout << "Object created by copy operator: " << int(this) << std::endl;   
    }

    // Move
    Object(Object&& a)
    {
        std::cout << "Object created by move ctr: " << int(this) << std::endl;   
    }

    Object& operator=(Object&& a)
    {
        std::cout << "Object created by move operator: " << int(this) << std::endl;   
    }

    ~Object()
    {
        std::cout << "Object destroyed: " << int(this) << std::endl;   
    }

    void printThis() const
    {
        std::cout << "Object this: " << int(this) << " ; val = " << val_ << std::endl;   
    }

public:
    int val_ = 0;
};


class ObjectHolder
{
public:
    ObjectHolder() 
    {
        std::cout << "ObjectHolder object ptr: " << int(&obj_) << std::endl;
    }
    ~ObjectHolder() {}

    void setObjectByValue(const Object obj)  // copy 
    {
        std::cout << "--" << std::endl;
        obj_ = obj;
        std::cout << "--" << std::endl;
    }

    void setObjectByMoving(Object&& obj)  // move or copy
    {
        std::cout << "--" << std::endl;
        obj_ = std::move(obj);
        std::cout << "--" << std::endl;
    }    


private:
    Object obj_;   
};


int main(int argc, char const *argv[])
{
    {
        ObjectHolder objectHolder;
        std::cout << "---" << std::endl;
        objectHolder.setObjectByValue(Object());
        std::cout << "---" << std::endl;
    }
    std::cout << "\n\n\n" << std::endl;
    {
        ObjectHolder objectHolder;
        std::cout << "---" << std::endl;
        objectHolder.setObjectByMoving(Object());
        std::cout << "---" << std::endl;
    }
    std::cout << "\n\n\n" << std::endl;    
    return 0;
}
