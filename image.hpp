#pragma ONCE
#include <vector>
#include <iostream>
#include <fstream>

class greyscale;

class slika{
    protected:
        virtual int trunc(int);
        virtual void recolor_pixel( std::vector<int>&, int&, int, int) = 0;
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

    friend greyscale;

    private:
        int image_width = 0, image_height = 0, image_depth = 255;
        std::string type = "P3";
        std::vector<std::vector<std::vector<int>>> pix;
        void recolor_pixel( std::vector<int>&, int&, int, int);
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
        greyscale to_greyscale( image );
};

class greyscale : public slika{
    private:
        int image_width = 0, image_height = 0, image_depth = 255;
        std::string type = "P2";
        std::vector<std::vector<int>> pix;
        int trunc(int);
        void recolor_pixel( std::vector<int>&, int&, int, int);
        void rgb_modifier(int, int);
    public:
        greyscale(const image&);
        greyscale(std::string);
        void modify_red(int);
        void modify_green(int);
        void modify_blue(int);
        void modify_contrast(double);
        void modify_brightness(int);
        void gaussian_blur();
        void sobel();
        void save(std::string);
};