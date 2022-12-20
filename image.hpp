#pragma ONCE
#include <vector>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdint.h> 

class greyscale;

class slika{
    protected:
        virtual uint16_t trunc(uint16_t);
        virtual void recolor_pixel( std::vector<uint16_t>&, uint16_t&, uint16_t, uint16_t) = 0;
        virtual void rgb_modifier(uint16_t, uint16_t) = 0;
    public:
        virtual std::string what() = 0;
        virtual void modify_red(uint16_t) = 0;
        virtual void modify_green(uint16_t) = 0;
        virtual void modify_blue(uint16_t) = 0;
        virtual void modify_contrast(double) = 0;
        virtual void modify_brightness(uint16_t) = 0;
        virtual void gaussian_blur() = 0;
        virtual void sobel() = 0;
        virtual void save(std::string) = 0;
        virtual greyscale to_greyscale() = 0;
        virtual void to_mat() = 0;
};

class image : public slika{

    friend greyscale;

    private:
        uint16_t image_width = 0, image_height = 0, image_depth = 255;
        std::string type = "P3";
        std::vector<std::vector<std::vector<uint16_t>>> pix;
        void recolor_pixel( std::vector<uint16_t>&, uint16_t&, uint16_t, uint16_t);
        void rgb_modifier(uint16_t, uint16_t);
    public:
        std::string what();
        image(std::string);
        void modify_red(uint16_t);
        void modify_green(uint16_t);
        void modify_blue(uint16_t);
        void modify_contrast(double);
        void modify_brightness(uint16_t);
        void gaussian_blur();
        void sobel();
        void save(std::string);
        greyscale to_greyscale();
        void to_mat();
        
};

class greyscale : public slika{

    friend image;

    private:
        uint16_t image_width = 0, image_height = 0, image_depth = 255;
        std::string type = "P2";
        std::vector<std::vector<uint16_t>> pix;
        uint16_t trunc(uint16_t);
        void recolor_pixel( std::vector<uint16_t>&, uint16_t&, uint16_t, uint16_t);
        void rgb_modifier(uint16_t, uint16_t);
    public:
        std::string what();
        greyscale(const image&);
        greyscale(std::string);
        void modify_red(uint16_t);
        void modify_green(uint16_t);
        void modify_blue(uint16_t);
        void modify_contrast(double);
        void modify_brightness(uint16_t);
        void gaussian_blur();
        void sobel();
        void save(std::string);
        virtual greyscale to_greyscale();
        void to_mat();
};