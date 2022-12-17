#include <iostream>
#include <unordered_map>
#include <list>
#include <typeinfo>
#include "image.hpp"

void grey( slika*& k ){
    auto temp = new greyscale{k->to_greyscale()};
    delete k;
    k = temp;
}

int main(){

    std::unordered_map<std::string, slika*> slike;
    std::list<std::string> keys;
    std::string input;
    slika* working = nullptr;
    auto it = slike.begin();

    char opcije{' '};

    std::cout << "Welcome to Simple Image Manipulation Program\n---------------------------------------------"
    << "\nt - list all loaded images" << "\nl - load a new image" << "\nw - select image to work on" 
    << "\nr/g/b - modify r/g/b" << "\ns - save selected image" << "\nc - modify contrast" << "\nq - quit"
    << "\nz - to greyscale" << "\ne - edge isolation"
    << "\n---------------------------------------------\n";

    for(;opcije!='q';){
        if(!working) std::cout << "Warning, no image selected to work on!";
        std::cout << "\nSelect : ";
        std::cin >> opcije;
        switch (opcije){
        case 't':
            for(auto&& i : keys) std::cout << i << ' ';
            std::cout << '\n';
            break;
        case 'l':
            std::cout << "Name of the image to load: ";
            std::cin >> input;
            slike[input] = new image{input};
            keys.emplace_back(std::move(input));
            break;
        case 'w':
            std::cout << "Image name: ";
            std::cin >> input;
            it = slike.find(input);
            if( it == slike.end() ) std::cout << "Image not found";
            else working = it->second;
            break;
        case 'r':
            std::cin >> input;
            working->modify_red(std::stoi(input));
            break;
        case 'g':
            std::cin >> input;
            working->modify_green(std::stoi(input));
            break;
        case 'b':
            std::cin >> input;
            working->modify_blue(std::stoi(input));
            break;
        case 'c':
            std::cin >> input;
            working->modify_contrast(std::stoi(input));
            break;
        case 's':
            std::cout << "Output image name: "; std::cin >> input;
            working->save(input);
            break;
        case 'z':
            grey(working);
            break;
        case 'e':
            if(working->what() == "P3") grey(working);
            working->sobel();
            break;
        case 'q':
            std::cout << "Have a nice day :)\n";
            break;
        case '0':
            std::cout << working->what() << std::endl;
            break;
        default:
            opcije = ' ';
            std::cout << "Not an option";
            break;
        }
    }
}