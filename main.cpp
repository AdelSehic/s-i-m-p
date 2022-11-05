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

            if ( pikseli.at(i).at(x)[0] > 255 ) output << 255 << ' ';
            else if ( pikseli.at(i).at(x)[0] < 0 ) output << 0 << ' ';
            else output << pikseli.at(i).at(x)[0] << ' ';

            if ( pikseli.at(i).at(x)[1] > 255 ) output << 255 << ' ';
            else if ( pikseli.at(i).at(x)[1] < 0 ) output << 0<< ' ';
            else output << pikseli.at(i).at(x)[1]<< ' ';

            if ( pikseli.at(i).at(x)[2] > 255 ) output << 255<< ' ';
            else if ( pikseli.at(i).at(x)[2] < 0 ) output << 0<< ' ';
            else output << pikseli.at(i).at(x)[2]<< ' ';

            // output << pikseli.at(i).at(x)[0] << ' ' << pikseli.at(i).at(x)[1] << ' ' <<  pikseli.at(i).at(x)[2] << '\n';
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

void modify_contrast(pixelArr& pix){
    double modifier;
    std::cout << "Modify by : "; std::cin >> modifier; std::cout << std::endl;
    for(auto&& h : pix) for(auto&& w : h) for(int i = 0; i < 3; ++i) w[i] +=  (w[i]-128)*modifier;
}

void modify_brightness(pixelArr& pix){
    double modifier;
    std::cout << "Modify by : "; std::cin >> modifier; std::cout << std::endl;
    for(auto&& h : pix) for(auto&& w : h) for(int i = 0; i < 3; ++i) w[i] +=  modifier;
}

void to_grayscale(pixelArr& pikseli){
    std::ofstream output;
    output.open("greyscale.pgm");

    output << "P2\n" << image_width << ' ' << image_height << '\n' << image_depth << '\n';

    for(int i = 0; i < image_height; ++i){
        for(int x = 0; x < image_width; ++x){
            int out = (pikseli.at(i).at(x)[0] + pikseli.at(i).at(x)[1] + pikseli.at(i).at(x)[2]) / 3;
            if(out > 255) output << 255 << ' ';
            else if(out < 0) output << 0 << ' ';
            else output << out << ' '; 
        }
    }

    output.close();
}

void gaussian_blur(pixelArr& pix){

}

int main(){

    pixelArr pikseli{load_image()};

    char choice = ' ';
    while(1){
        std::cout << "r/g/b - modify individual values by x\nc - modify contrast\nv - modify brightness\nq\nEnter your choice : ";
        std::cin>>choice;
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
                modify_contrast(pikseli);
                break;
            case 'v':
                modify_brightness(pikseli);
                break;
            case 'q':
                to_grayscale(pikseli);
                break;
            default:
                break;
        } else break;
        output_image(pikseli);
    }

    output_image(pikseli, "output");

    return 0;
}