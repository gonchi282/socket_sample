#include <iostream>

template <typename TYPE> class Some
{
    public:
        Some(TYPE);

        public:
            static TYPE seiteki;
};

int main(void)
{
        Some<int> a(30);
            Some<int> b(50);

                std::cout << a.seiteki << std::endl;
                    std::cout << b.seiteki << std::endl;

                        return 0;
}

template<typename TYPE>
Some <TYPE>::Some(TYPE a)
{
        seiteki = a;
}

