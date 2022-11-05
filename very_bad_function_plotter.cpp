#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#define pixelArr std::vector<std::vector<std::vector<int>>>

int image_width = 1280, image_height = 720;

void ispis_slike(std::ostream& out, pixelArr){

    out << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int i = 0; i < image_height; ++i){
        for(int x = 0; x < image_width; ++x){
            out << 255 << ' ' << 255 << ' ' << 255 << std::endl;
        }
    }
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
        for(int x = 0; x < image_width; ++x){
            pix.at(i).at(x).resize(3);
            recolor_pixel(pix[i][x], r, g, b);
        }
    }
    
    return pix;
}

void draw_function( pixelArr& pix ){
    for(int x = 20; x < image_width - 20 && x < image_height - 20; ++x ){
        recolor_pixel( pix[x][x], 255 );
    }
}

void draw_sin(pixelArr& pix){
    for(int x = 20; x < image_width - 20 && x < image_height - 20; ++x ){
        recolor_pixel( pix[ (sin(x)+image_height/2)*1.25 ][x], 255 );
    }
}

void draw_plot(pixelArr& arr){
    int pola1 = image_height/2;
    int pola2 = image_width/2;
    for(int i = 20; i < image_width-20; ++i) recolor_pixel(arr.at(pola1).at(i));
    for(int i = 20; i < image_height-20; ++i) recolor_pixel(arr.at(i).at(pola2));

    for(int i = 0; i < 20; ++i){
        recolor_pixel( arr.at(pola1+i).at( i+20 ) );
        recolor_pixel( arr.at(pola1-i).at( i+20 ) );

        recolor_pixel( arr.at(i+20).at( pola2+i ) );
        recolor_pixel( arr.at(i+20).at( pola2-i ) );
    }
}

int main(){

    std::ofstream slika;

    pixelArr pikseli = make_array(255, 255, 255);

    draw_plot(pikseli);

    draw_function(pikseli);

    draw_sin(pikseli);

    slika.open("output.ppm");

    slika << "P3\n1280 720\n255\n";

    for(int i = 0; i < 720; ++i){
        for(int x = 1279; x >= 0; --x){
            slika << pikseli.at(i).at(x)[0] << ' ' << pikseli.at(i).at(x)[1] << ' ' <<  pikseli.at(i).at(x)[2] << '\n';
        }
    }

    slika.close();

    return 0;
}