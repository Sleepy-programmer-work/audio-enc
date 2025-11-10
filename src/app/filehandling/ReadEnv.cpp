#include "ReadEnv.hpp"
#include "IO.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

ReadEnv::ReadEnv() : env_path(".env"), default_key("AudioEncryptDefault2024") {
    // Constructor - you can customize default key here
}

std::string ReadEnv::getKey() {
    std::string key = readKeyFromFile();
    
    if (key.empty()) {
        std::cerr << "Warning: No key found in .env file. Using default key.\n";
        return default_key;
    }
    
    if (!isValidKey(key)) {
        std::cerr << "Warning: Invalid key in .env file. Using default key.\n";
        return default_key;
    }
    
    return key;
}

bool ReadEnv::keyExists() {
    IO io(env_path);
    return io.isOpen();
}

std::string ReadEnv::readKeyFromFile() {
    try {
        IO io(env_path);
        
        if (!io.isOpen()) {
            std::cerr << "Warning: Could not open .env file.\n";
            return "";
        }
        
        // Read entire file content
        std::vector<uint8_t> data = io.readBinaryData();
        
        if (data.empty()) {
            std::cerr << "Warning: .env file is empty.\n";
            return "";
        }
        
        // Convert bytes to string
        std::string content(data.begin(), data.end());
        
        // Sanitize the key (remove whitespace, newlines, etc.)
        return sanitizeKey(content);
        
    } catch (const std::exception& e) {
        std::cerr << "Error reading .env: " << e.what() << std::endl;
        return "";
    }
}

std::string ReadEnv::sanitizeKey(const std::string& key) {
    std::string sanitized;
    
    // Remove all whitespace, newlines, carriage returns, tabs
    for (char c : key) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            sanitized += c;
        }
    }
    
    // Handle KEY=value format (if user writes KEY=mykey123)
    size_t equals_pos = sanitized.find('=');
    if (equals_pos != std::string::npos) {
        sanitized = sanitized.substr(equals_pos + 1);
    }
    
    return sanitized;
}

bool ReadEnv::isValidKey(const std::string& key) {
    // Key validation rules
    if (key.empty()) {
        return false;
    }
    
    // Minimum length (e.g., at least 8 characters for security)
    if (key.length() < 8) {
        std::cerr << "Warning: Key too short (minimum 8 characters).\n";
        return false;
    }
    
    // Maximum length (avoid extremely long keys)
    if (key.length() > 256) {
        std::cerr << "Warning: Key too long (maximum 256 characters).\n";
        return false;
    }
    
    // Check for printable ASCII characters only
    for (char c : key) {
        if (!std::isprint(static_cast<unsigned char>(c))) {
            std::cerr << "Warning: Key contains non-printable characters.\n";
            return false;
        }
    }
    
    return true;
}