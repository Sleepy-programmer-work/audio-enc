#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <string>
#include <vector>
#include <cstdint> // for uint8_t

class IO {
public:
    IO(const std::string& file_path); // Constructor
    ~IO();
    
    std::fstream& getFileStream();
    bool isOpen() const; // Check if file is open
    

    std::vector<uint8_t> readBinaryData(); // Read entire file
    std::vector<uint8_t> readBinaryData(size_t offset, size_t length); // Read with offset and length
    size_t getFileSize(); 
    
    bool writeBinaryData(const std::vector<uint8_t>& data); 
    bool writeBinaryData(const std::vector<uint8_t>& data, size_t offset); // Write with offset
    
    std::vector<uint8_t> readHeader(size_t header_size); 
    bool writeHeader(const std::vector<uint8_t>& header); 
    
    void reset(); // Reset file stream position to beginning 
    void close(); // Close file stream

private:
    std::fstream file_stream;
    std::string file_path;
};

#endif