#ifndef __GKERNEL_HPP_CONTAINERS
#define __GKERNEL_HPP_CONTAINERS

#include "objects.hpp"

namespace gkernel {

    class Converter;

    /**
     * @brief Родительский класс, содержащий общие методы для контейнеров отрезков.
     */
    class SegmentsSetCommon: public Labeling
    {
    protected:
        SegmentsSetCommon(): _segments({}) {}
        SegmentsSetCommon(const std::vector<Segment>& segments): _segments(segments.begin(), segments.end()) {
            for (size_t i = 0; i < _segments.size(); ++i) {
                _segments[i].id = i;
            }
        }
        SegmentsSetCommon(std::vector<Segment>&& segments): _segments(std::move(segments)) {
            for (size_t i = 0; i < _segments.size(); ++i) {
                _segments[i].id = i;
            }
        }

    public:
        /**
         * @brief Возвращает true, если содержимое контейнеров this и other идентично, иначе false.
         *
         * @param other Сравниваемый контейнер.
         * @return true Содержимое контейнеров одинаковое.
         * @return false Содержимое контейнеров разное.
         */
        bool operator==(const SegmentsSetCommon& other) const {
            return std::equal(this->_segments.begin(), this->_segments.end(), other._segments.begin());
        }
        /**
         * @brief Возвращает true, если содержимое контейнеров this и other не одинаковое, иначе false.
         *
         * @param other Сравниваемый контейнер.
         * @return true Содержимое контейнеров разное.
         * @return false Содержимое контейнеров одинаковое.
         */
        bool operator!=(const SegmentsSetCommon& other) const {
            return !(*this == other);
        }

        /**
         * @brief Возвращает значение метки для заданного отрезка.
         *
         * @param label Тип метки.
         * @param segment Отрезок, для которого мы хотим узнать значение метки типа label.
         * @return label_data_type Значение метки типа label для указанного отрезка segment.
         */
        label_data_type get_label_value(label_type label, const Segment& segment) const override {
            return _labels_data[get_offset(label) + segment.id];
        }

        /**
         * @brief Устанавливает типы меток для контейнера.
         *
         * @param label_types Типы меток.
         */
        void set_labels_types(const std::vector<label_type>& label_types) override;

        /**
         * @brief Устанавливает значение метки указанного типа для каждого объекта в контейнере.
         * В случае несовпадения количества элементов в контейнере и в label_data будет брошена ошибка.
         *
         * @param label Тип метки.
         * @param label_data Значение метки для объекта с индексом idx = label_data[idx]
         * @throw std::runtime_error если число объектов в контейнере не совпадает с числом элементов в label_data.
         */
        void set_label_values(label_type label, const std::vector<label_data_type>& label_data) override;

        /**
         * @brief Устанавливает значение метки указанного типа для заданного отрезка.
         *
         * @param label Тип метки.
         * @param segment Отрезок, для которого устанавливается значение метки.
         * @param label_value Значение метки.
         */
        void set_label_value(label_type label, const Segment& segment, label_data_type label_value) override {
            _labels_data[get_offset(label) + segment.id] = label_value;
        }

        /**
         * @brief Возвращает отрезок, хранящийся по индексу idx для чтения.
         *
         * @param idx Индекс отрезка.
         * @return const Segment& Отрезок, хранящийся по индексу idx.
         */
        const Segment& operator[](size_t idx) const {
            return _segments[idx];
        }

        /**
         * @brief Возвращает отрезок, хранящийся по индексу idx.
         *
         * @param idx Индекс отрезка.
         * @return const Segment& Отрезок, хранящийся по индексу idx.
         */
        Segment& operator[](size_t idx) {
            return _segments[idx];
        }

        /**
         * @brief Возвращает количество элементов в контейнере.
         *
         * @return size_t Количество элементов в контейнере.
         */
        size_t size() const {
            return _segments.size();
        }

    private:
        size_t get_offset(label_type label) const;

    protected:
        std::vector<Segment> _segments;

        friend class CircuitsSet;
        friend class Converter;
    };

    /**
     * @brief Набор отрезков, контейнер, хранящий объекты класса Segment.
     *
     */
    class SegmentsSet: public SegmentsSetCommon
    {
    public:
        /**
         * @brief Конструктор класса.
         *
         */
        SegmentsSet(): SegmentsSetCommon() {}

        /**
         * @brief Создаёт новый объект SegmentsSet. Контейнер заполняется копиями из segments.
         *
         * @param segments Вектор объектов Segment, которые будет содержать SegmentSet.
         */
        SegmentsSet(const std::vector<Segment>& segments): SegmentsSetCommon(segments) {}

        /**
         * @brief Добавляет новый элемент Segment в конец контейнера, если метки для контейнера ещё не заданы, иначе при попытке добавления будет выброшено std::runtime_error.
         * @param segment Добавляемый элемент.
         * @throw std::runtime_error при добавлении нового элемента, после задания меток.
         */
        virtual void emplace_back(const Segment& segment) {
            if (!_label_types.empty()) {
                throw std::runtime_error("Unable to add elements after labels initialization.");
            }
            _segments.emplace_back(segment);
            _segments.back().id = _segments.size() - 1;
        }
    };

    /**
     * @brief Цепь, контейнер, хранящий объекты класса Segment.
     * @details Цепь представляет из себя последовательность непересекающихся отрезков, в котором конец каждого отрезка совпадает с началом последующего.
     *
     */
    class VertexChain: public SegmentsSet, public Validator
    {
    public:
        /**
         * @brief Конструктор класса.
         *
         */
        VertexChain(): SegmentsSet() {}
        /**
         * @brief Создаёт новый объект VertexChain. Контейнер заполняется копиями из segments.
         *
         * @param segments Вектор объектов Segment, которые будет содержать SegmentSet. Последовательность отрезков в segments должна удовлетворять правилу построения цепи.
         * @param validation настройка проверки правила построения цепи для отрезков вектора segments (по умолчанию true)
         * @n true - включить проверку
         * @n false - выключить проверку
         */
        VertexChain(const std::vector<Segment>& segments, bool validation = true): SegmentsSet(segments), Validator(validation) {
            if (_validation) {
                validate();
            }
        }

        /**
         * @brief Добавляет объект segment в конец контейнера, если метки для контейнера ещё не заданы, иначе при попытке добавления будет выброшено std::runtime_error.
         *
         * @param segment Добавляемый элемент.
         * @throw std::runtime_error если validation = true и добавляемый отрезок нарушает правило построения цепи.
         */
        void emplace_back(const Segment& segment) override {
            if (_validation && !_segments.empty()) {
                if (_segments.back().end() != segment.start()) {
                    throw std::runtime_error("Validation failed. Invalid emplaced segment.");
                }
            }
            SegmentsSet::emplace_back(segment);
        }

    private:
        void validate() const override {
            for (size_t i = 0; i < _segments.size() - 1; ++i) {
                if (_segments[i].end() != _segments[i + 1].start()) {
                    throw std::runtime_error("Validation failed. Segment set is not a chain.");
                }
            }
        }
    };

    /**
     * @brief Контур, контейнер, хранящий объекты класса Segment.
     * @details Контур — цепь, в которой начальная точка первого отрезка совпадает с конечной точкой последнего отрезка.
     */
    class Circuit: public SegmentsSetCommon, public Validator
    {
    public:
        /**
         * @brief Конструктор класса.
         *
         */
        Circuit(): SegmentsSetCommon() {}
        /**
         * @brief Конструктор копирования.
         *
         * @param circuit Объект копирования.
         */
        Circuit(const Circuit& circuit): SegmentsSetCommon(circuit._segments) {}

        /**
         * @brief Создаёт новый объект Circuit. Контейнер заполняется копиями из segments.
         *
         * @param segments Вектор объектов Segment, которые будет содержать Circuit. Последовательность отрезков в segments должна удовлетворять правилу построения контура.
         * @param validation настройка проверки правила построения контура для отрезков вектора segments (по умолчанию true)
         *                  @n true - включить проверку
         *                  @n false - выключить проверку
         */
        Circuit(const std::vector<Segment>& segments, bool validation = true): SegmentsSetCommon(segments), Validator(validation) {
            if (_validation) {
                validate();
            }
        }

    private:
        void validate() const override {
            for (size_t i = 0; i < _segments.size(); ++i) {
                if (_segments[i].end() != _segments[(i + 1) % _segments.size()].start()) {
                    throw std::runtime_error("Validation failed. Segment set is not a circuit.");
                }
            }
        }
    };

    /**
     * @brief Набор контуров, контейнер для объектов класса Circuits.
     *
     */
    class CircuitsSet: public Labeling
    {
    public:
        /**
         * @brief Конструктор класса.
         *
         */
        CircuitsSet(): _segments({}), _indices({}) {}

        /**
         * @brief Конструктор копирования.
         *
         * @param circuits Объект копирования.
         */
        CircuitsSet(const CircuitsSet& circuits): _segments(circuits._segments.begin(), circuits._segments.end()),
            _indices(circuits._indices.begin(), circuits._indices.end()) {
        }

        /**
         * @brief Создаёт новый объект CircuitsSet. Контейнер заполняется копиями из circuits.
         *
         * @param circuits Копируемое значение.
         */
        CircuitsSet(const std::vector<Circuit>& circuits) {
            _indices = { 0 };
            for (const auto& circuit : circuits) {
                _segments.insert(_segments.end(), circuit._segments.begin(), circuit._segments.end());
                _indices.emplace_back(_segments.size());
            }
        }

        label_data_type get_label_value(label_type label, const Segment& segment) const override { return 0; /*TODO*/ };
        void set_labels_types(const std::vector<label_type>& label_types) override {/*TODO*/ };
        void set_label_values(label_type label, const std::vector<label_data_type>& label_data) override {/*TODO*/ };
        void set_label_value(label_type label, const Segment& segment, label_data_type label_value) override {/*TODO*/ };

        /**
         * @brief Возвращает контур, хранящийся по индексу idx.
         *
         * @param idx индекс
         * @return Circuit контур, хранящийся по индексу idx.
         */
        Circuit get_circuit(size_t idx) const {
            std::vector<Segment> segments(_segments.begin() + _indices[idx], _segments.begin() + _indices[idx + 1]);
            return Circuit(segments, false);
        }

        /**
         * @brief Добавляет контур в конец контейнера, если метки для контейнера ещё не заданы, иначе при попытке добавления будет выброшено std::runtime_error.
         *
         * @param circuit Добавляемое значение.
         * @throw std::runtime_error при добавлении нового элемента, после задания меток.
         */
        void emplace_back(const Circuit& circuit) {
            if (!_label_types.empty()) {
                throw std::runtime_error("Unable to add elements after labels initialization.");
            }
            _segments.insert(_segments.end(), circuit._segments.begin(), circuit._segments.end());
            _indices.emplace_back(_segments.size());
        }

        /**
         * @brief Возвращает количество элементов в контейнере.
         *
         * @return size_t Количество элементов в контейнере.
         */
        size_t size() const {
            return _indices.size() - 1;
        }

    private:
        std::vector<Segment> _segments;
        std::vector<size_t> _indices; // circuit bounds (ex: if input circuits have size eq 2, 3, 4 respectively, then _indices={0, 2, 5, 9})
        friend class Converter;
    };

    using SegmentsLayer = const SegmentsSet;
    using CircuitsLayer = const CircuitsSet;

} // namespace gkernel
#endif /* __GKERNEL_HPP_CONTAINERS */
