#ifndef __GKERNEL_HPP_AREA
#define __GKERNEL_HPP_AREA

#include "containers.hpp"
#include "gkernel/rbtree.hpp"

#include <functional>
#include <iostream>

namespace gkernel {

double get_sweeping_line_y(const Segment& segment, double x) {
    if (segment.start().x() < segment.end().x()) {
        double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
        double m = segment.start().y() - k * segment.start().x();
        return k * x + m;
    }
    else {
        double k = (segment.start().y() - segment.end().y()) / (segment.start().x() - segment.end().x());
        double m = segment.end().y() - k * segment.end().x();
        return k * x + m;
    }
}

class SegmentPair {
    public:
        SegmentPair() = delete;

static SegmentsSet SegmentPair::segmentlabel(const SegmentsSet& Segmentset) {
    SegmentsSet result;
    double x_sweeping_line = 0;

    auto compare_segments = [&x_sweeping_line](const Segment* first, const Segment* second) -> bool {
        double y1 = get_sweeping_line_y(*first, x_sweeping_line);
        double y2 = get_sweeping_line_y(*second, x_sweeping_line);
        if (y1 - y2 != 0) return y1 < y2;
        return first->id < second->id;
    };

    std::set<EventLabel> events;
    for (size_t idx=0; idx<Segmentset.size(); ++idx) {
        const Segment& segment = Segmentset[idx];
        if (segment.min().x() != segment.max().x()) {
            if (segment._begin_point.x() < segment._end_point.x()) {
                events.insert({segment, segment._begin_point.x(), event_status::start});
                events.insert({segment, segment._end_point.x(), event_status::end});
            }
            else {
                //const auto BufSegment = Segment({segment._end_point, segment._begin_point});
                events.insert({ segment, segment._end_point.x(), event_status::start});
                events.insert({ segment, segment._begin_point.x(), event_status::end});
            }
        }
    }

    using tree_type = RBTree<const Segment*, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);
    std::map<Segment, std::vector<label_data_type>> bufferlabel;
    std::vector<label_data_type> buf1;
    std::vector<label_data_type> buf2;
    std::vector<label_data_type> buf3;
    std::vector<const Segment*> temp_segments;
    temp_segments.reserve(Segmentset.size());

    while (events.size() > 0) {
        auto event = *events.begin();
        if (event.x != x_sweeping_line) {
            while (active_segments.size()) {
                auto it = active_segments.begin();
                temp_segments.push_back(*it);
                if (event.status == event_status::start) {
                    auto prev_segment = it;
                    int intermediate_result_prev = -1;
                    if (prev_segment != active_segments.begin()) {
                        --prev_segment;
                        intermediate_result_prev = (**prev_segment).id;
                    }

                    auto next_segment = it;
                    ++next_segment;
                    int intermediate_result_next = -1;
                    if (next_segment != active_segments.end()) {
                        intermediate_result_next = (**next_segment).id;
                    }
                    result.emplace_back(**it);
                    buf1.push_back(Segmentset.get_label_value(1, **it));
                    buf2.push_back(intermediate_result_prev);
                    buf3.push_back(intermediate_result_next);
                    //result.set_label_value(1, **it, Segmentset.get_label_value(1, **it));
                    //result.set_label_value(2, **it, intermediate_result_prev);
                    //result.set_label_value(3, **it, intermediate_result_next);

                }
                active_segments.erase(it);
            }
            x_sweeping_line = event.x;
            for (auto& elem : temp_segments) {
                active_segments.insert(elem);
            }
            temp_segments.clear();
        }

        tree_type::iterator internal_result;
        if (event.status == event_status::start) {
            internal_result = active_segments.insert(event.segment).first;
        } else {
            internal_result = active_segments.find(event.segment);
        }

        if (event.status == event_status::end) {
            active_segments.erase(event.segment);
        }

        events.erase(events.begin());
        }
    result.set_labels_types({ 1,2,3 });
    result.set_label_values(1, buf1);
    result.set_label_values(2, buf2);
    result.set_label_values(3, buf3);
    return result;
    }
    
private:
    enum event_status {
        start = 1,
        end = 0
    };
    struct EventLabel {
        EventLabel(const Segment& segment, data_type x, event_status status) : segment(&segment), x(x), status(status) {}

        bool operator<(const EventLabel& other) const {
            if (x != other.x) return x < other.x;
            if (status != other.status) return static_cast<int8_t>(status) > static_cast<int8_t>(other.status);
            return this->segment->id < other.segment->id;
        }

        const Segment* segment;
        data_type x;
        event_status status;
    };
};

}
#endif // __GKERNEL_HPP_AREA