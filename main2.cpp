#include <iostream>
#include "image.hpp"

int main(){

    image q("apollo");

    slika* p = &q;

    p->modify_contrast(1.5);

    p->save("modified");

    p->gaussian_blur();

    p->save("gaus");

    return 0;
}