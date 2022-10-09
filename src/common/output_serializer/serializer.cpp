#include "serializer.h"

void OutputSerializer::serializeCircuits(
    const std::vector<gkernel::Circuit>& circuit, const std::string& path,
    bool overwrite) {
    std::ofstream output;
    overwrite ? output.open(path, std::ofstream::out)
              : output.open(path, std::ofstream::app);

    if (!output.is_open()) {
        std::string error_message = "Cannot open file " + path + "\n";
#if GKERNEL_DEBUG
        std::cerr << error_message << std::endl;
#endif
        throw std::runtime_error(error_message);
    }

    for (auto item : circuit) {
        output << item.getID() << ": ";
        for (std::size_t i = 0; i < item.size(); ++i) {
            output << item[i].x << ' ' << item[i].y;
            output << ((i != item.size() - 1) ? " " : ";\n");
        }
    }

    output.close();
}
