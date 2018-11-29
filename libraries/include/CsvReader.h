#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <sstream>

class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str);

    private:
        std::vector<std::string>    m_data;
};

inline std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

#endif // CSVREADER_H
