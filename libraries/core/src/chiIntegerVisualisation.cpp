#include <iostream>
#include <string>

#include "chiIntegerVisualisation.h"

// this function visualises the chi field in an integer matrix form (where chi is normalised between values 2 and 7), where 2 represents background medium and 7 represents the subsurface.
// 1 and 8 are acceptable out of bounds, while 0 and 9 imply a relatively significant out of bound estimation of chi

void chi_visualisation_in_integer_form(std::string filename, int nxt)
{
    std::string line;
    std::ifstream myfile(filename);

    if (!myfile.is_open())
    {
        std::cout << "Couldn't open file at " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    int x = 0;              //counts over the horizontal input
    int z = 0;              //counts over the vertical input
    int bg_character = 7;   // in the ascii image values close to this value represent oil or background
    int rock_character = 2; // same as bg_character, but rock
    int resolution = bg_character - rock_character;
    double contrast = 1.869132054239392993e-01;          // rock-oil contrast as found in temple.txt using c0 and c1
    double no_contrast = 0.0;                            // oil-oil non-contrast
    double contrast_difference = contrast - no_contrast; // just gives you back the contrast
    double value;                                        //value read in from file
    int dummy;                                           //value printed to screen

    while (std::getline(myfile, line))
    {
        value = stod(line);
        dummy = std::lround(((value) / contrast_difference * resolution) + rock_character); //cast value read to int printed by linear interpol
        if (dummy < 0)
        {
            dummy = 0;
        } // below bounds back to 0, signals potential problem
        else if (dummy > 9)
        {
            dummy = 9;
        } // above bounds back to 9, see above

        std::cout << dummy; //print integer

        //first increment x and then start printing the next line if nxt is reached
        x++;
        if (x == (nxt))
        {
            z++;
            x = 0;
            std::cout << std::endl;
        }
    }
    myfile.close();
}
