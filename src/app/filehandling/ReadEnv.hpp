#ifndef READENV_HPP
#define READENV_HPP

#include <string>

class ReadEnv {
public:
    ReadEnv();
    std::string getKey();
    bool keyExists();
    
private:
    std::string env_path; // Path to the .env file
    std::string default_key; // Default key if none found
    
    std::string readKeyFromFile(); // Read key from .env file
    std::string sanitizeKey(const std::string& key); // Remove unwanted characters
    bool isValidKey(const std::string& key); // Validate key format
};

#endif