#ifndef __GKERNEL_HPP_PARSER
#define __GKERNEL_HPP_PARSER

#include <vector>
#include "gkernel/containers.hpp"

namespace gkernel {
/**
 * @brief Класс для разбора входных данных и их перевода во внутреннее представление.
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
     * @return SegmentsSet - множество отрезков
     */
    static SegmentsSet parseSegmentsSet(const std::string& path);
    static std::vector<SegmentsSet> parseVectorOfSegmentsSet(const std::string& path);
};

} // namespace gkernel
#endif /* __GKERNEL_HPP_PARSER */
