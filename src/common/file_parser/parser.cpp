#include "parser.h"

#include <fstream>
#include <iostream>
#include <vector>

std::vector<gkernel::Circuit> FileParser::parse(const std::string& path) {
    std::ifstream input(path, std::ifstream::in);
    std::vector<gkernel::Circuit> circuitsList;
    if (!input.is_open()) {
        std::string error_message = "Cannot open file " + path + "\n";
        std::cerr << error_message << std::endl;
        throw std::runtime_error(error_message);
    }

    while (input.peek() != EOF) {
        // parse circuit identifier
        std::string bytes;
        while (input.peek() != ':') {
            bytes += static_cast<char>(input.get());
        }

        gkernel::identifier name = std::stoi(bytes);
        gkernel::Circuit item(name);

        // ignore :
        input.get();
        // ignore <space>
        input.get();

        // parse point coordinates
        gkernel::data_type x, y = 0;
        while (input.peek() != ';') {
            input >> x >> y;
            item.push({x, y});
        }

        // ignore ;
        input.get();

        // ignore new line symbols
        // ignore Windows line ending
        if (input.peek() == '\r') {
            input.get();
        }
        // ignore \n
        input.get();

        circuitsList.push_back(std::move(item));
    }

    input.close();
    return circuitsList;
}
