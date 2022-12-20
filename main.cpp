#include <iostream>
#include <unordered_map>
#include <list>
#include "image.hpp"
#include "dft.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void grey( slika*& k ){
    auto temp = new greyscale{k->to_greyscale()};
    delete k;
    k = temp;
}

cv::Mat fourier(cv::Mat& img){
    img.convertTo(img, CV_32FC1);
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    for(auto&& i : channels) cv::dft( i, i);
    cv::Mat four;
    cv::merge(channels, four);
    four.convertTo(four, CV_8UC3);
    return four;
}

cv::Mat inverse_fourier(cv::Mat& img){
    img.convertTo(img, CV_32FC1);
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    for(auto&& i : channels) cv::dft( i, i, cv::DFT_INVERSE );
    cv::Mat four;
    cv::merge(channels, four);
    four.convertTo(four, CV_8UC3);
    return four;
}

int main(){

    std::unordered_map<std::string, slika*> slike;
    std::list<std::string> keys;
    std::string input;
    // slika* working = nullptr;
    std::string working = "";
    auto it = slike.begin();
    char opcije{' '};

    std::cout << "Welcome to Simple Image Manipulation Program\n---------------------------------------------"
    << "\nt - list all loaded images" << "\nl - load a new image" << "\nw - select image to work on" 
    << "\nr/g/b - modify r/g/b" << "\ns - save selected image" << "\nc - modify contrast" << "\nq - quit"
    << "\nz - to greyscale" << "\ne - edge isolation"
    << "\n---------------------------------------------\n";

    for(;opcije!='q';){
        if(working == "") std::cout << "Warning, no image selected to work on!";
        std::cout << "\nSelect : ";
        std::cin >> opcije;
        switch (opcije){
        case 't':
            for(auto&& i : keys) std::cout << i << ' ';
            std::cout << '\n';
            break;
        case 'l':
            std::cout << "Name of the image to load: ";
            std::cin >> input;
            working = input;
            slike[working] = new image{working};
            keys.emplace_back(std::move(input));
            break;
        case 'w':
            std::cout << "Image name: ";
            std::cin >> input;
            it = slike.find(input);
            if( it == slike.end() ) std::cout << "Image not found";
            else working = input;
            break;
        case 'r':
            std::cin >> input;
            slike[working]->modify_red(std::stoi(input));
            break;
        case 'g':
            std::cin >> input;
            slike[working]->modify_green(std::stoi(input));
            break;
        case 'b':
            std::cin >> input;
            slike[working]->modify_blue(std::stoi(input));
            break;
        case 'c':
            std::cin >> input;
            slike[working]->modify_contrast(std::stoi(input));
            break;
        case 's':
            std::cout << "Output image name: "; std::cin >> input;
            slike[working]->save(input);
            break;
        case 'z':
            grey(slike[working]);
            break;
        case 'e':
            if(slike[working]->what() == "P3") grey(slike[working]);
            slike[working]->sobel();
            break;
        case 'q':
            std::cout << "Have a nice day :)\n";
            break;
        case '0':
            std::cout << slike[working]->what() << std::endl;
            break;
        default:
            opcije = ' ';
            std::cout << "Not an option";
            break;
        }
    }

    std::unordered_map<std::string, Mat> mats;

    int wd = 0, hg = 0, dp = 0;
    for (auto &&i : keys){
        auto k = slike[i]->data(hg, wd, dp);
        mats[i] = Mat( hg, wd, CV_8UC3, k );
    }

    for (auto it = keys.begin();;){
        cv::imshow(*it, mats[*it]);
        int key = cv::waitKey(0);
        if(key == 's') imwrite(*it+".png", mats[*it]);
        if(key == 'l'){
            ++it;
            if(it == keys.end()) it = keys.begin();
        }
        if(key == 'k'){
            if( it==keys.begin() ) it = keys.end();
            --it;
        }
        if(key == 'e'){
            cv::Mat grey;
            cv::cvtColor(mats[*it], grey, cv::COLOR_BGR2GRAY);
            cv::Canny(grey,grey,125,175);
            mats[*it] = grey;
        }
        if(key == 'f'){
            mats[*it] = fourier( mats[*it] );
            // fourier_transform( mats[*it] );
        }
        if(key == 'i'){
            mats[*it] = inverse_fourier( mats[*it] );
            // fourier_transform( mats[*it] );
        }
        if(key == 'q') break;
        cv::destroyAllWindows();
    }
   
    for(auto&& i : keys) delete slike[i];
}