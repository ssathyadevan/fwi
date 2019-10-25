#include <fstream>
#include <iostream>

#include "createChiCSV.h"

// creates a csv file with the chi values from the file input_filename and save them onto a file called output_filename
// these csv files could be later used to post-process the chi data as required
void create_csv_files_for_chi(std::string inputFilePath, genericInput input, std::string postfix)
{
    std::string line;
    std::ifstream myfile(inputFilePath);

    if (!myfile.is_open())
    {
        std::cout << "Could not open file at " << inputFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ofstream output;
    std::string outputFilePath = input.outputLocation + postfix + input.runName + ".csv";
    output.open(outputFilePath); // open the file to write the chi values into

    if (!output.is_open())
    {
        std::cout << "Could not create file at " << outputFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    int x = 0;    //counts over the horizontal input
    int z = 0;    //counts over the vertical input
    double value; //double to read the chi value

    while (std::getline(myfile, line))
    {
        value = stod(line);
        if (x == input.ngrid[0] - 1)
        {
            output << value;
        }
        else
        {
            output << value << ",";
        }
        //first increment x and then start printing the next line if nxt is reached
        x++;
        if (x == (input.ngrid[0]))
        {
            z++;
            x = 0;
            output << std::endl;
        }
    }
    myfile.close();
    output.close();
}
