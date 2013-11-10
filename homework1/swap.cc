#include <iostream>

inline void swap(int& i, int& j){
    int temp = i;
    i = j;
    j = temp;
}

inline void swap(double& i, double& j){
    double temp = i;
    i = j;
    j = temp;
}

int main()
{
    int m = 5, n = 10;
    double x = 5.3, y = 10.6;
    std::cout << "inputs:" << m << ", " << n << std::endl;
    std::cout << "double inputs: " << x << ", " << y << std::endl;
    swap(m, n);
    std::cout << "int outputs: " << m << ", " << n << std::endl;
    swap(x, y);
    std::cout << "double outputs: " << x << ", " << y << std::endl;

    return 0;
}



