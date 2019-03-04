#include <fstream>

#include "createChiCSV.h"

// creates a csv file with the chi values from the file input_filename and save them onto a file called output_filename
// these csv files could be later used to post-process the chi data as required
void create_csv_files_for_chi(std::string inputFilePath, genericInput input, std::string postfix)
{

    std::string line;
    std::ifstream myfile (inputFilePath);
    int x = 0; //counts over the horizontal input
    int z = 0; //counts over the vertical input
    double value; //double to read the chi value
    std::ofstream output;
    output.open(input.outputLocation + postfix + input.runName + ".csv");// open the file to write the chi values into
    while (std::getline(myfile, line))
    {
        value = stod(line);
        if (x == input.ngrid[0]-1)     {output << value;}
        else                {output << value << ",";}
        //first increment x and then start printing the next line if nxt is reached
        x++;
        if (x == (input.ngrid[0])) { z++; x=0; output << std::endl;}
    }
    myfile.close();
    output.close();
}
