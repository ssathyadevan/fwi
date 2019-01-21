#ifndef CREATE_CSV_FILES_FOR_CHI_H
#define CREATE_CSV_FILES_FOR_CHI_H
#include "variableStructure.h"

// creates a csv file with the chi values from the file input_filename and save them onto a file called output_filename
// these csv files could be later used to post-process the chi data as required
void create_csv_files_for_chi(string inputFilePath, Input input, string postfix);

#endif // CREATE_CSV_FILES_FOR_CHI_H
