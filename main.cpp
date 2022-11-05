#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

#define pixelArr std::vector<std::vector<std::vector<int>>>

int image_width = 720, image_height = 1280, image_depth = 255;


void recolor_pixel( std::vector<int>& pix, int r = 0, int g = 0, int b = 0){
    pix.at(0) = r;
    pix.at(1) = g;
    pix.at(2) = b;
}

pixelArr make_array(int r = 255, int g = 255, int b = 255){

    pixelArr pix(image_height);

    for(int i = 0; i < image_height; ++i){
        pix.at(i).resize(image_width);
        for(int x = 0; x < image_width; ++x){
            pix.at(i).at(x).resize(3);
            // recolor_pixel(pix[i][x], r, g, b);
        }
    }

    return pix;
}

pixelArr load_image(){

    std::ifstream input;
    input.open("bird.ppm");
    int r, g, b;
    std::string type;

    input >> type >> image_width >> image_height >> image_depth;

    auto pix = make_array();

    for(auto&& h : pix) for(auto&& w : h){
        input >> r >> g >> b;
        recolor_pixel(w, r, g, b);
    }

    input.close();
    return pix;
}

void output_image(const pixelArr& pikseli){

    std::string outName;
    std::cout << "Enter output image name: ";
    std::cin >> outName;

    std::ofstream output;
    output.open(outName + ".ppm");

    output << "P3\n" << image_width << ' ' << image_height << '\n' << image_depth << '\n';

    for(int i = 0; i < image_height; ++i){
        for(int x = 0; x < image_width; ++x){
            output << pikseli.at(i).at(x)[0] << ' ' << pikseli.at(i).at(x)[1] << ' ' <<  pikseli.at(i).at(x)[2] << '\n';
        }
    }

    output.close();
}

void blue_filter(pixelArr& pix){
    for(auto&& h : pix) for(auto&& w : h){
        w[2] += 50;
        if ( w[2] > 255 ) w[2] = 255;
    }
}

void increase_contrast(pixelArr& pix){
    for(auto&& h : pix) for(auto&& w : h){
        for(int i = 0; i < 3; ++i){
            w[i] +=  (w[i]-128)*0.2;
            if ( w[i] > 255 ) w[i] = 255;
            if ( w[i] < 0 ) w[i] = 0;
        }
    }
}

int main(){

    pixelArr pikseli{load_image()};

    // blue_filter(pikseli);

    increase_contrast(pikseli);

    output_image(pikseli);

    return 0;
}