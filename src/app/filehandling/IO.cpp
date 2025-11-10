#include <iostream>
#include "IO.hpp"

IO::IO(const std::string& file_path) : file_path(file_path) {
    file_stream.open(file_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file_stream.is_open()) {
        std::cerr << "Unable to open file: " << file_path << std::endl;
    }
}

IO::~IO() {
    close(); // Ensure file is closed on destruction
}

std::fstream& IO::getFileStream() {
    return file_stream; // Return reference to the file stream
}

bool IO::isOpen() const {
    return file_stream.is_open();
}

// Read entire file
std::vector<uint8_t> IO::readBinaryData() {
    if (!isOpen()) {
        std::cerr << "File is not open!" << std::endl;
        return {};
    }

    // Get file size
    file_stream.seekg(0, std::ios::end); // Move to end
    size_t size = file_stream.tellg(); // Get position (size)
    file_stream.seekg(0, std::ios::beg); // Move back to beginning

    // Read entire file
    std::vector<uint8_t> buffer(size); // Allocate buffer
    file_stream.read(reinterpret_cast<char*>(buffer.data()), size); // Read data

    return buffer;
}


// Read with offset and length
std::vector<uint8_t> IO::readBinaryData(size_t offset, size_t length) {
    if (!isOpen()) {
        std::cerr << "File is not open!" << std::endl;
        return {};
    }

    file_stream.seekg(offset, std::ios::beg); 
    std::vector<uint8_t> buffer(length); 
    file_stream.read(reinterpret_cast<char*>(buffer.data()), length); 

    return buffer;
}

size_t IO::getFileSize() {
    if (!isOpen()) {
        return 0;
    }

    file_stream.seekg(0, std::ios::end);
    size_t size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);

    return size;
}

// Write entire data
bool IO::writeBinaryData(const std::vector<uint8_t>& data) {
    if (!isOpen()) {
        std::cerr << "File is not open!" << std::endl;
        return false;
    }

    file_stream.seekp(0, std::ios::beg);
    file_stream.write(reinterpret_cast<const char*>(data.data()), data.size());

    return file_stream.good();
}

// Write with offset
bool IO::writeBinaryData(const std::vector<uint8_t>& data, size_t offset) {
    if (!isOpen()) {
        std::cerr << "File is not open!" << std::endl;
        return false;
    }

    file_stream.seekp(offset, std::ios::beg);
    file_stream.write(reinterpret_cast<const char*>(data.data()), data.size());

    return file_stream.good();
}

// Audio-specific operations
std::vector<uint8_t> IO::readHeader(size_t header_size) {
    return readBinaryData(0, header_size);
}

// Write header at the beginning
bool IO::writeHeader(const std::vector<uint8_t>& header) {
    return writeBinaryData(header, 0);
}


void IO::reset() {
    if (isOpen()) {
        file_stream.clear();
        file_stream.seekg(0, std::ios::beg);
        file_stream.seekp(0, std::ios::beg);
    }
}

void IO::close() {
    if (file_stream.is_open()) {
        file_stream.close();
    }
}