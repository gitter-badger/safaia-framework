#define RESET "\033[0m"
#define GREEN "\033[32m" // Green
#define YELLOW "\033[33m" // Yellow
#define RED "\033[1m\033[31m" // Bold Red

#include <iostream>
#include <string>
#include <regex>


class Log{
    int is_stdout = isatty(fileno(stdout));

public:

    bool ansi_color = true;

    Log(){
        // CLion still does not support ASCII color control, disable the feature by default
        if (std::regex_match(std::string(getenv("XPC_SERVICE_NAME")), std::regex("(.*)(CLion)(.*)")))
            ansi_color = false;
    }

    void info(std::string tag, std::string str){
        std::cout << (ansi_color && is_stdout ? GREEN : "") << "[Info] " << tag << ": "<< str << (ansi_color && is_stdout ? RESET : "") << std::endl;
    }
    void warning(std::string tag, std::string str){
        std::cout << (ansi_color && is_stdout ? YELLOW : "") <<"[Warning] " << tag << ": "<< str << (ansi_color && is_stdout ? RESET : "") << std::endl;
    }
    void error(std::string tag, std::string str){
        std::cerr << (ansi_color && is_stdout ? RED : "") << "[Error] " << tag << ": "<< str << (ansi_color && is_stdout ? RESET : "") << std::endl;
    }
};