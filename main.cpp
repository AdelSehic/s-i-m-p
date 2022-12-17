#include <iostream>
#include <unordered_map>
#include "image.hpp"

int main(){

    std::unordered_map<std::string, slika*> slike;
    std::string input;

    char opcije;

    std::cout << "Welcome to Simple Image Manipulation Program\n-----------------------------------------------\n\n";

    for(;std::cin;){
        switch (opcije){
        case 'l':
            std::cout << "Name of the image to load: ";
            std::cin >> input;
            slike[input] = new image{input};
            break;
        default:
            break;
        }
    }
    
    /*
    image q("apollo");

    slika* p = &q;

    p->modify_contrast(1.5);

    p->save("modified");

    p->gaussian_blur();

    p->save("gaus");

    p = new greyscale{image{"monument"}};

    p->sobel();

    p->save("monument_edges");

    delete p;

    return 0;
    */
}