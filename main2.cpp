#include <iostream>
#include "image.hpp"

int main(){

    image q("apollo");

    q.modify_contrast(1.5);

    q.save("modified");

    q.gaussian_blur();

    q.save("gaus");

    return 0;
}