#include <iostream>

void func(std::string& a)
{
    std::cout << a << std::endl;
}

int main(void)
{
    std::string a = "abc";

    func(a);

    return 0;
}


