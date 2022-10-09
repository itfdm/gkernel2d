#ifndef __GKERNEL_H_FILE_PARSER
#define __GKERNEL_H_FILE_PARSER

#include <vector>

#include "gkernel/structures/circuit.h"

/**
 * @brief Класс, для разбора входных данных и их перевода во внутреннее представление.
 *
 */
class FileParser {
public:
    FileParser() = delete;
    ~FileParser() = delete;

    /**
     * @brief Выполняет разбор входных данных.
     *
     * @param path путь до файла
     * @return std::vector<gkernel::Circuit> - множество контуров
     */
    static std::vector<gkernel::Circuit> parse(const std::string& path);
};

#endif /* __GKERNEL_H_FILE_PARSER */
