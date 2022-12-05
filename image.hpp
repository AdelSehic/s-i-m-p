#pragma ONCE
#include <vector>
#include <iostream>
#include <fstream>

class slika{
    protected:
        virtual int trunc(int) ;
        virtual void recolor_pixel( std::vector<int>&, int, int, int) = 0;
        virtual void rgb_modifier(int, int) = 0;
    public:
        virtual void modify_red(int) = 0;
        virtual void modify_green(int) = 0;
        virtual void modify_blue(int) = 0;
        virtual void modify_contrast(double) = 0;
        virtual void modify_brightness(int) = 0;
        virtual void gaussian_blur() = 0;
        virtual void sobel() = 0;
        virtual void save(std::string) = 0;
};

class image : public slika{
    private:
        int image_width = 0, image_height = 0, image_depth = 255;
        std::string type = "P3";
        std::vector<std::vector<std::vector<int>>> pix;
        virtual void recolor_pixel( std::vector<int>&, int, int, int);
        virtual void rgb_modifier(int, int);
    public:
        image(std::string);
        virtual void modify_red(int);
        virtual void modify_green(int);
        virtual void modify_blue(int);
        virtual void modify_contrast(double);
        virtual void modify_brightness(int);
        virtual void gaussian_blur();
        virtual void sobel();
        virtual void save(std::string);
};

// class greyscale : public slika{
//     private:
//         int image_width = 0, image_height = 0, image_depth = 255;
//         std::string type = "P2";
//         std::vector<std::vector<int>> pix;
//         virtual int trunc(int);
//         void recolor_pixel( std::vector<int>&, int);
//         void rgb_modifier(int, int);
//     public:
//         greyscale(std::string);
//         virtual void modify_red(int);
//         virtual void modify_green(int);
//         virtual void modify_blue(int);
//         virtual void modify_contrast(double);
//         virtual void modify_brightness(int);
//         virtual void gaussian_blur();
//         virtual void sobel();
//         virtual void save(std::string);
// };