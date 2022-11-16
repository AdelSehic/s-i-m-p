#pragma ONCE
#include <vector>
#include <iostream>
#include <fstream>

class image{
    private:
        int image_width = 0, image_height = 0, image_depth = 255;
        std::string type = "P3";
        std::vector<std::vector<std::vector<int>>> pix;
        int trunc(int);
        void recolor_pixel( std::vector<int>&, int, int, int);
        void rgb_modifier(int, int);
    public:
        image(std::string);
        void modify_red(int);
        void modify_green(int);
        void modify_blue(int);
        void modify_contrast(double);
        void modify_brightness(int);
        void gaussian_blur();
        void sobel();
        void save(std::string);
};