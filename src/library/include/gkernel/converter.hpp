#include "geom_structures_base.hpp"

namespace gkernel {

    class Converter {
        Converter() = delete;

    public:
        static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& segments);
        static SegmentsLayer convertToSegmentsLayer(const CircuitsSet& circuits);

        static CircuitsLayer convertToCircuitsLayer(const CircuitsSet& circuits);

        static CircuitsSet convertToCircuitsSet(const CircuitsLayer& circuits);
        static SegmentsSet convertToSegmentsSet(const SegmentsLayer& segments);
    };

} // namespace gkernel
