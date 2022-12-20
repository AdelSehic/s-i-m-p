#include "image.hpp"
#include <typeinfo>
#include <cxxabi.h>

void image::recolor_pixel( std::vector<uint16_t>& pix, uint16_t& r, uint16_t g = 0, uint16_t b = 0){
    pix.at(0) = r;
    pix.at(1) = g;
    pix.at(2) = b;
}

uint16_t slika::trunc(uint16_t value){
    if(value>255) value = 255;
    if(value<0) value = 0;
    return value;
}

image::image(std::string ime){
    std::ifstream input;

    input.open(ime + ".ppm");

    uint16_t r, g, b;
    std::string type;

    input >> type >> image_width >> image_height >> image_depth;

    pix.resize(image_height);
    for(uint16_t i = 0; i < image_height; ++i){
        pix.at(i).resize(image_width);
        for(uint16_t x = 0; x < image_width; ++x)
            pix.at(i).at(x).resize(3); 
    }

    for(auto&& h : pix) for(auto&& w : h){
        input >> r >> g >> b;
        recolor_pixel(w,r,g,b);
    }

    input.close();
};

void image::save(std::string ime){
    std::ofstream output;
    output.open(ime + ".ppm");

    output << "P3\n" << image_width << ' ' << image_height << '\n' << image_depth << '\n';

    for(uint16_t i = 0; i < image_height; ++i){
        for(uint16_t x = 0; x < image_width; ++x){
            output << trunc(pix.at(i).at(x)[0]) << ' ';
            output << trunc(pix.at(i).at(x)[1]) << ' ';
            output << trunc(pix.at(i).at(x)[2]) << ' ';
        }
    }

    output.close();
};

void image::rgb_modifier(uint16_t choice, uint16_t modifier){
    for(auto&& h : pix) for(auto&& w : h) w[choice] += modifier;
}

void image::modify_red(uint16_t a){rgb_modifier(0, a);};
void image::modify_green(uint16_t a){rgb_modifier(1, a);}
void image::modify_blue(uint16_t a){rgb_modifier(2, a);}

void image::modify_contrast(double modifier){
    for(auto&& h : pix) for(auto&& w : h) for(uint16_t i = 0; i < 3; ++i) w[i] =  trunc(modifier*(w[i]-128)+128);
}

void image::modify_brightness(uint16_t modifier){
    for(auto&& h : pix) for(auto&& w : h) for(uint16_t i = 0; i < 3; ++i) trunc(w[i] +=  modifier);
}

void image::gaussian_blur(){
    auto temp = pix;

    double kernel[5][5] = {
        {0.25,0.5,1,0.5,0.25},
        {0.5,1,2,1,0.5},
        {1,2,4,2,1},
        {0.5,1,2,1,0.5},
        {0.25,0.5,1,0.5,0.25},
    };

    for(auto y = 3; y < image_height-3; ++y) for(auto x = 3; x < image_width-3; ++x) for(auto i = 0; i < 3; ++i){
        uint16_t zbir = 0;
        for(uint16_t ky = 0; ky < 5; ++ky){
            for(uint16_t kx = 0; kx < 5; ++kx){
                zbir += pix.at(y-2+ky).at(x+kx-2)[i] * kernel[ky][kx];                
            }
        }
        zbir /= 25;
        temp[y][x][i] = zbir;
    }
    
    pix = std::move(temp);
}

void image::sobel(){

    auto temp = this->to_greyscale();

    
}

greyscale image::to_greyscale(){
    return greyscale{*this};
}

std::string image::what(){ return type; }

uint8_t* image::data(int& a, int& b, int& c){

    using namespace cv;
    using namespace std;

    a = image_height;
    b = image_width;
    c = image_depth;

    auto data = new uint8_t[image_width*image_height*3];

    size_t brojac = 0;
    for( auto i = pix.begin(); i != pix.end(); ++i ){
        for( auto j = i->begin(); j != i->end(); ++j ){
            data[brojac++] = j->at(2);
            data[brojac++] = j->at(1);
            data[brojac++] = j->at(0);
        }
    }

    // std::cout << *(typeid(data[0]).name()) << '\n';

    // uint8_t m = new Mat( image_height, image_width, CV_8UC3, data );

    return data;
}

// G R E Y S C A L E :


uint8_t* greyscale::data(int& a, int& b, int& c){

    using namespace cv;

    auto data = new uint8_t[image_width*image_height];

    size_t brojac = 0;
    for( auto i = pix.begin(); i != pix.end(); ++i ){
        for( auto j = i->begin(); j != i->end(); ++j ){
            data[brojac++] = *j;
        }
    }

    // Mat* m = new Mat( image_height, image_width, CV_8UC1, data );

    return data;
}

std::string greyscale::what(){ return type; }

greyscale::greyscale(const image& img ){

    type = "P2";
    image_depth = img.image_depth;
    image_width = img.image_width;
    image_height = img.image_height;

    pix.resize(image_height);
    for(auto i = 0; i <image_height;++i){ pix.at(i).resize(image_width); }

    for(uint16_t i = 0; i < image_height; ++i) for(uint16_t x = 0; x < image_width; ++x){
        pix.at(i).at(x) = trunc((img.pix.at(i).at(x)[0] + img.pix.at(i).at(x)[1] + img.pix.at(i).at(x)[2]) / 3);
    }

}

void greyscale::recolor_pixel( std::vector<uint16_t>& pix, uint16_t& a, uint16_t v, uint16_t throwaway){ a = v;}

uint16_t greyscale::trunc(uint16_t value){
    if(value>255) value = 255;
    if(value<0) value = 0;
    return value;
}

greyscale::greyscale(std::string ime){
    std::ifstream input;

    input.open(ime + ".pgm");

    uint16_t v;

    input >> type >> image_width >> image_height >> image_depth;

    pix.resize(image_height);
    for(auto i = 0; i <image_height;++i){ pix.at(i).resize(image_width); }

    for(auto&& h : pix) for(auto&& w : h){
        input >> v;
        recolor_pixel(pix.at(0),w,v,0);
    }

    input.close();
};

void greyscale::save(std::string ime){
    std::ofstream output;
    output.open(ime + ".pgm");

    output << "P2\n" << image_width << ' ' << image_height << ' ' << image_depth << '\n';

    for(uint16_t i = 0; i < image_height; ++i){
        for(uint16_t x = 0; x < image_width; ++x){
            output << pix.at(i).at(x) << ' ';
        }
    }

    output.close();
};

void greyscale::rgb_modifier(uint16_t choice, uint16_t modifier){
    std::cout << "Undefined for greyscale greyscales\n";
}

void greyscale::modify_red(uint16_t a){rgb_modifier(0, a);};
void greyscale::modify_green(uint16_t a){rgb_modifier(1, a);}
void greyscale::modify_blue(uint16_t a){rgb_modifier(2, a);}

void greyscale::modify_contrast(double modifier){
    for(auto&& h : pix) for(auto&& w : h) w =  trunc(modifier*(w-128)+128);
}

void greyscale::modify_brightness(uint16_t modifier){
    for(auto&& h : pix) for(auto&& w : h) trunc(w +=  modifier);
}

void greyscale::gaussian_blur(){
    auto temp = pix;

    double kernel[5][5] = {
        {0.25,0.5,1,0.5,0.25},
        {0.5,1,2,1,0.5},
        {1,2,4,2,1},
        {0.5,1,2,1,0.5},
        {0.25,0.5,1,0.5,0.25},
    };

    for(auto y = 3; y < image_height-3; ++y) for(auto x = 3; x < image_width-3; ++x){
        uint16_t zbir = 0;
        for(uint16_t ky = 0; ky < 5; ++ky){
            for(uint16_t kx = 0; kx < 5; ++kx){
                zbir += pix.at(y-2+ky).at(x+kx-2) * kernel[ky][kx];                
            }
        }
        zbir /= 25;
        temp[y][x] = zbir;
    }
    
    pix = std::move(temp);
}

void greyscale::sobel(){

    auto temp_x = pix;
    auto temp_y = pix;

    int kernelx[3][3] = {
        { 1, 0, -1 },
        { 2, 0, -2 },
        { 1, 0, -1 },
    };
    int kernely[3][3] = {   // significantly better-looking edge at lower resolutions
        { 1, 2, 1},
        { 0, 0, 0},
        {-1,-2,-1}
    };

    uint16_t zbir_x, zbir_y;
    for(auto y = 3; y < image_height-3; ++y) for(auto x = 3; x < image_width-3; ++x) {
        zbir_x = zbir_y = 0;
        for(uint16_t ky = 0; ky < 3; ++ky){
            for(uint16_t kx = 0; kx < 3; ++kx){
                zbir_x += pix.at(y-1+ky).at(x+kx-1) * kernelx[ky][kx];               
                zbir_y += pix.at(y-1+ky).at(x+kx-1) * kernely[ky][kx];
            }
        }
        temp_x[y][x] = zbir_x;
        temp_y[y][x] = zbir_y;
    }

    for(auto y = 0; y < image_height; ++y) for(auto x = 0; x < image_width; ++x)
        temp_x[y][x] = trunc(temp_x[y][x] + temp_y[y][x]) / 2;
    pix = temp_x;
}

greyscale greyscale::to_greyscale(){
    std::cout << "This image is already in greyscale format\n";
    return *this;
}