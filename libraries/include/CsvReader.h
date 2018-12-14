#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <sstream>

class CSVReader
{
    public:
        std::string const& operator[](std::size_t index) const;
        std::size_t size() const;
        void readNextRow(std::istream& str);

    private:
        std::vector<std::string>    m_data;
};

inline std::istream& operator>>(std::istream& str, CSVReader& data)
{
    data.readNextRow(str);
    return str;
}



#endif // CSVREADER_H
