#include "image.hpp"

void image::recolor_pixel( std::vector<int>& pix, int r = 0, int g = 0, int b = 0){
    pix.at(0) = r;
    pix.at(1) = g;
    pix.at(2) = b;
}

int slika::trunc(int value){
    if(value>255) value = 255;
    if(value<0) value = 0;
    return value;
}

image::image(std::string ime){
    std::ifstream input;

    input.open(ime + ".ppm");

    int r, g, b;
    std::string type;

    input >> type >> image_width >> image_height >> image_depth;

    pix.resize(image_height);
    for(int i = 0; i < image_height; ++i){
        pix.at(i).resize(image_width);
        for(int x = 0; x < image_width; ++x)
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

    for(int i = 0; i < image_height; ++i){
        for(int x = 0; x < image_width; ++x){
            output << trunc(pix.at(i).at(x)[0]) << ' ';
            output << trunc(pix.at(i).at(x)[1]) << ' ';
            output << trunc(pix.at(i).at(x)[2]) << ' ';
        }
    }

    output.close();
};

void image::rgb_modifier(int choice, int modifier){
    for(auto&& h : pix) for(auto&& w : h) w[choice] += modifier;
}

void image::modify_red(int a){rgb_modifier(0, a);};
void image::modify_green(int a){rgb_modifier(1, a);}
void image::modify_blue(int a){rgb_modifier(2, a);}

void image::modify_contrast(double modifier){
    for(auto&& h : pix) for(auto&& w : h) for(int i = 0; i < 3; ++i) w[i] =  trunc(modifier*(w[i]-128)+128);
}

void image::modify_brightness(int modifier){
    for(auto&& h : pix) for(auto&& w : h) for(int i = 0; i < 3; ++i) w[i] +=  modifier;
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
        int zbir = 0;
        for(int ky = 0; ky < 5; ++ky){
            for(int kx = 0; kx < 5; ++kx){
                zbir += pix.at(y-2+ky).at(x+kx-2)[i] * kernel[ky][kx];                
            }
        }
        zbir /= 25;
        temp[y][x][i] = zbir;
    }
    
    pix = std::move(temp);
}

void image::sobel(){

    auto temp_x = pix;
    auto temp_y = pix;

    for(int i = 0; i < image_height; ++i) for(int x = 0; x < image_width; ++x){
        temp_x[i][x][0] = temp_y[i][x][0] = (pix.at(i).at(x)[0] + pix.at(i).at(x)[1] + pix.at(i).at(x)[2]) / 3;
    }

    int kernelx[3][3] = {
        { 1, 0, -1 },
        {2, 0, -2},
        { 1, 0, -1 },
    };
    int kernely[3][3] = {   // significantly better-looking edge at lower resolutions
        { 1, 2, 1},
        {  0,    0,   0 },
        {-1,-2,-1}
    };

    int zbir_x, zbir_y;
    for(auto y = 3; y < image_height-3; ++y) for(auto x = 3; x < image_width-3; ++x) {
        zbir_x = zbir_y = 0;
        for(int ky = 0; ky < 3; ++ky){
            for(int kx = 0; kx < 3; ++kx){
                zbir_x += pix.at(y-1+ky).at(x+kx-1)[0] * kernelx[ky][kx];               
                zbir_y += pix.at(y-1+ky).at(x+kx-1)[0] * kernely[ky][kx];
            }
        }
        temp_x[y][x][0] = zbir_x;
        temp_y[y][x][0] = zbir_y;
    }

    for(auto y = 0; y < image_height; ++y) for(auto x = 0; x < image_width; ++x)
        temp_x[y][x][0] = trunc(temp_x[y][x][0] + temp_y[y][x][0]) / 2;
    pix = temp_x;
}