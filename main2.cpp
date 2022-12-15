#include <iostream>
#include "image.hpp"

int main(){

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
}