#include <vector>
#include <iostream>
#include <fstream>

class image{
    private:
        int image_width = 0, image_height = 0, image_depth = 255;
        std::vector<std::vector<std::vector<int>>> pix;
        int trunc(int);
    public:
        image();
        void modify_red(int);
        void modify_green(int);
        void modify_blue(int);

        void save(std::string);
};