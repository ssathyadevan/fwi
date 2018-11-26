#include "CsvReader.h"

void CSVRow::readNextRow(std::istream& str)
{
    std::string         line;
    std::getline(str, line);

    std::stringstream   lineStream(line);
    std::string         cell;

    m_data.clear();
    while(std::getline(lineStream, cell, ','))
    {
        m_data.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        m_data.push_back("");
    }
}
