#ifndef __GKERNEL_H_OUTPUT_SERIALIZER
#define __GKERNEL_H_OUTPUT_SERIALIZER

#include <fstream>

#include "gkernel/circuit.h"
#include "gkernel/helpers.h"

/**
 * Класс, предназначенный для перевода данных из внутреннего представления в текстовый формат.
 *
 */
class OutputSerializer {
public:
    OutputSerializer() = delete;
    ~OutputSerializer() = delete;

    /**
     * @brief Сериализует набор контуров
     *
     * @param circuit области наложения
     * @param path путь до файла, в который будет сохранен результат
     * @param overwrite если true и файл существует, то он будет перезаписан
     */
    static void serializeCircuits(const std::vector<gkernel::Circuit>& circuit,
                                  const std::string& path,
                                  bool overwrite = true);
};

#endif /* __GKERNEL_H_OUTPUT_SERIALIZER */
