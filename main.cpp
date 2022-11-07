#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

#define pixelArr std::vector<std::vector<std::vector<int>>>

int image_width = 720, image_height = 1280, image_depth = 255;

int trunc(int value){
    if(value>255) value = 255;
    if(value<0) value = 0;
    return value;
}

void recolor_pixel( std::vector<int>& pix, int r = 0, int g = 0, int b = 0){
    pix.at(0) = r;
    pix.at(1) = g;
    pix.at(2) = b;
}

pixelArr make_array(int r = 255, int g = 255, int b = 255){

    pixelArr pix(image_height);

    for(int i = 0; i < image_height; ++i){
        pix.at(i).resize(image_width);
        for(int x = 0; x < image_width; ++x)
            pix.at(i).at(x).resize(3); 
    }

    return pix;
}

pixelArr load_image(){

    std::ifstream input;

    std::cout<< "Name of the picture to work on : ";
    std::string slika; std::cin >> slika;

    input.open(slika + ".ppm");
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

void output_image(const pixelArr& pikseli, std::string outName = "temp"){

    std::ofstream output;
    output.open(outName + ".ppm");

    output << "P3\n" << image_width << ' ' << image_height << '\n' << image_depth << '\n';

    for(int i = 0; i < image_height; ++i){
        for(int x = 0; x < image_width; ++x){
            output << trunc(pikseli.at(i).at(x)[0]) << ' ';
            output << trunc(pikseli.at(i).at(x)[1]) << ' ';
            output << trunc(pikseli.at(i).at(x)[2]) << ' ';
        }
    }

    output.close();
}

void rgb_modifier(pixelArr& pix, int choice){
    int modifier;
    std::cout << "Modify by : "; std::cin >> modifier; std::cout << std::endl;
    for(auto&& h : pix) for(auto&& w : h)
        w[choice] += modifier;
}

void modify_contrast(pixelArr& pix, double modifier){
    for(auto&& h : pix) for(auto&& w : h) for(int i = 0; i < 3; ++i) w[i] =  trunc(modifier*(w[i]-128)+128);
}

void modify_brightness(pixelArr& pix){
    double modifier;
    std::cout << "Modify by : "; std::cin >> modifier; std::cout << std::endl;
    for(auto&& h : pix) for(auto&& w : h) for(int i = 0; i < 3; ++i) w[i] +=  modifier;
}

void to_grayscale(pixelArr& pikseli, std::string ime = "greyscale.pgm"){
    std::ofstream output;
    output.open(ime);

    output << "P2\n" << image_width << ' ' << image_height << '\n' << image_depth << '\n';

    for(int i = 0; i < image_height; ++i) for(int x = 0; x < image_width; ++x){
        int out = (pikseli.at(i).at(x)[0] + pikseli.at(i).at(x)[1] + pikseli.at(i).at(x)[2]) / 3;
        output << trunc(out) << ' ';
    }

    output.close();
}

void gaussian_blur(pixelArr& pix){

    auto temp = make_array();

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
    
    output_image(temp, "gaus");
}/*
far from a perfect gaussian output since it
messes with brigtness of the image, and doesn't
cover literal edge cases
but it's good enough as a proof of concept
*/

void sobel(pixelArr& pix){

    auto temp_x = make_array();
    auto temp_y = make_array();

    for(int i = 0; i < image_height; ++i) for(int x = 0; x < image_width; ++x){
        temp_x[i][x][0] = temp_y[i][x][0] = (pix.at(i).at(x)[0] + pix.at(i).at(x)[1] + pix.at(i).at(x)[2]) / 3;
    }

    // float kernelx[3][3] = {
    //     { 0.5, 0, -0.5 },
    //     {1.25, 0, -1.25},
    //     { 0.5, 0, -0.5 },
    // };
    // float kernely[3][3] = {   // significantly better-looking edge at lower resolutions
    //     { 0.5, 1.25, 0.5},
    //     {  0,    0,   0 },
    //     {-0.5,-1.25,-0.5}
    // };

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

    std::ofstream output;
    output.open("edges.pgm");
    output << "P2\n" << image_width << ' ' << image_height << '\n' << image_depth << '\n';
    for(auto y = 0; y < image_height; ++y) for(auto x = 0; x < image_width; ++x)
        output << trunc(temp_x[y][x][0] + temp_y[y][x][0]) /2 << ' ';
    output.close();
    // to_grayscale(temp_x, "edges.pgm");
}

int main(){

    pixelArr pikseli{load_image()};

    char choice = ' ';
    while(1){
        std::cout << "r/g/b - modify individual values by x\nc - modify contrast\nv - modify brightness\nq - greyscale output\no - gaussian output\ne - edge isolation\nEnter your choice : ";
        std::cin>>choice;
        double input;
        if(std::cin) switch (choice){
            case 'r':
                rgb_modifier(pikseli, 0);
                break;
            case 'g':
                rgb_modifier(pikseli, 1);
                break;
            case 'b':
                rgb_modifier(pikseli, 2);
                break;
            case 'c':
                std::cout << "Modify by : "; std::cin >> input; std::cout << std::endl;
                modify_contrast(pikseli, input);
                break;
            case 'v':
                modify_brightness(pikseli);
                break;
            case 'q':
                to_grayscale(pikseli);
                break;
            case 'o':
                gaussian_blur(pikseli);
                break;
            case 'e':
                sobel(pikseli);
                break;
            default:
                break;
        } else break;
        output_image(pikseli);
    }

    output_image(pikseli, "output");

    return 0;
}