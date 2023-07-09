#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/converter.hpp"
#include "gkernel/parser.hpp"
#include "gkernel/area_analyzer.hpp"
#include "gkernel/serializer.hpp"
#include <chrono>

using namespace gkernel;

int main() {
    std::vector<SegmentsSet> segments_set_layer_first = FileParser::parseVectorOfSegmentsSet("./output_8.txt");
    std::vector<SegmentsSet> segments_set_layer_second = FileParser::parseVectorOfSegmentsSet("./output_13.txt");
    std::cout << "PARSED" << std::endl;
    std::cout << "first_size: " << segments_set_layer_first.size() << std::endl;
    std::cout << "second_size: " << segments_set_layer_second.size() << std::endl;

    std::vector<Circuit> circuits_first;
    std::vector<Circuit> circuits_second;
    for (const auto& segments_set : segments_set_layer_first) {
        std::vector<Segment> segments_vector;
        for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
            segments_vector.push_back(segments_set[idx]);
        }
        circuits_first.push_back(Circuit(segments_vector, false));
    }

    for (const auto& segments_set : segments_set_layer_second) {
        std::vector<Segment> segments_vector;
        for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
            segments_vector.push_back(segments_set[idx]);
        }
        circuits_second.push_back(Circuit(segments_vector, false));
    }

    CircuitsLayer first_layer(circuits_first);
    CircuitsLayer second_layer(circuits_second);
    std::cout << "CONVERTED" << std::endl;

    auto merged_layers = Converter::mergeCircuitsLayers(first_layer, second_layer);

    auto start = std::chrono::high_resolution_clock::now();
    auto segments_layer = Converter::convertToSegmentsLayer(merged_layers);

    SegmentsLayer filtered = AreaAnalyzer::markAreasAndFilter(segments_layer, [](const SegmentsLayer& segments, const Segment& segment) {
        return segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1 &&
                !(segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1) ||
               !(segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1) &&
                segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1;
    });

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    std::cout << "segments_layer size: " << segments_layer.size() << std::endl;
    // std::cout << "filtered_size: " << filtered.size() << std::endl;
    // OutputSerializer::serializeSegmentsSet(filtered, "./output.txt");
}
