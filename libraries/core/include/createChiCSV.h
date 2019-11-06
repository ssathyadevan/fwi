#pragma once

#include <string>
#include "genericInput.h"
#include <streambuf>
// creates a csv file with the chi values from the file input_filename and save them onto a file called output_filename
// these csv files could be later used to post-process the chi data as required
void create_csv_files_for_chi(std::string inputFilePath, GenericInput input, std::string postfix);

