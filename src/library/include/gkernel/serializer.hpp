#ifndef __GKERNEL_HPP_OUTPUT_SERIALIZER
#define __GKERNEL_HPP_OUTPUT_SERIALIZER

#include <fstream>

#include "gkernel/containers.hpp"

namespace gkernel {
/**
 * @brief Класс, предназначенный для перевода данных из внутреннего представления в текстовый формат.
 *
 */
class OutputSerializer {
public:
    OutputSerializer() = delete;
    ~OutputSerializer() = delete;

    /**
     * @brief Сериализует набор отрезков в файл.
     *
     * @param segments_set набор отрезков
     * @param path путь до файла, в который будет сохранен результат
     */
    static void serializeSegmentsSet(const SegmentsSet& segments_set, const std::string& path);
    static void serializeVectorOfSegmentsSet(const std::vector<SegmentsSet>& vector_of_segments_set, const std::string& path);
};

} // namespace gkernel
#endif /* __GKERNEL_HPP_OUTPUT_SERIALIZER */
