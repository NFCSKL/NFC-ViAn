#include "interval.h"

#include <QDebug>

Interval::Interval(std::string name) {
    m_name = name;
}

Interval::~Interval() {

}

void Interval::add_area(int start, int end) {
    add_area(std::make_pair(start, end));
}

/**
 * @brief Interval::add_area
 * Adds a new area to the interval list and handle each case
 * Cred: https://www.geeksforgeeks.org/insert-in-sorted-and-non-overlapping-interval-array/
 * @param interval
 */
void Interval::add_area(std::pair<int, int> interval) {
    m_unsaved_changes = true;
    std::vector<std::pair<int, int>> new_list;

    // Set is empty, simply add new interval
    if (m_area_list.empty()) {
        new_list.push_back(interval);
        m_area_list = new_list;
        return;
    }

    // Insert new interval first or last
    if (interval.second < m_area_list.front().first || interval.first > m_area_list.back().second) {
        if (interval.second < m_area_list.front().first) {
            new_list.push_back(interval);
        }

        for (auto pair : m_area_list) {
            new_list.push_back(pair);
        }

        if (interval.first > m_area_list.back().second) {
            new_list.push_back(interval);
        }
        m_area_list = new_list;
        return;
    }

    // New interval covers all. Clear and add new one
    if (interval.first <= m_area_list.front().first &&
            interval.second >= m_area_list.back().second) {
        new_list.push_back(interval);
        m_area_list = new_list;
        return;
    }

    // New interval overlap
    for (auto it = m_area_list.begin(); it != m_area_list.end(); ++it) {
        bool overlap = does_overlap(*it, interval);
        if (!overlap) {
            new_list.push_back(*it);

            // Check if new interval is between intervals
            if (it != m_area_list.end() &&
                    interval.first > (*it).second &&
                    interval.second < (*(it+1)).first)
            {
                new_list.push_back(interval);
            }
            continue;

        }
        int start = std::min(interval.first, (*it).first);
        int end = interval.second;

        while (it != m_area_list.end() && overlap) {
            end = std::max(interval.second, (*it).second);
            if (it == m_area_list.end()-1) {
                overlap = false;
            } else {
                overlap = does_overlap(*(it+1), interval);
            }
            it++;
        }

        it--;
        new_list.push_back(std::make_pair(start, end));
    }
    m_area_list = new_list;
}

void Interval::remove_area_by_frame(int frame) {
    for (auto it = m_area_list.begin(); it != m_area_list.end(); ++it) {
        if ((*it).first <= frame && (*it).second >= frame) {
            m_area_list.erase(it);
        }
    }
}

void Interval::remove_area(std::pair<int, int> interval) {
    auto p = std::find(m_area_list.begin(), m_area_list.end(), interval);
    m_area_list.erase(p);
}

bool Interval::does_overlap(std::pair<int, int> a, std::pair<int, int> b) {
    return (std::min(a.second, b.second) >= std::max(a.first, b.first));
}

//void Tag::remove_interval(AnalysisInterval *rm_interval) {
//    std::set<AnalysisInterval*, interval_cmp> intervals = m_intervals;
//    std::set<AnalysisInterval*, interval_cmp> res;
//    std::vector<AnalysisInterval*> temp_set;
//    auto it = intervals.begin();
//    while (it != intervals.end()) {
//        std::pair<int, int> overlap = get_overlap(*it, rm_interval);
//        if (is_interval(overlap.first, overlap.second)) {
//            std::pair<AnalysisInterval*, AnalysisInterval*> pair = remove_overlap(*it, overlap);
//            if (pair.first != nullptr) res.insert(pair.first);
//            if (pair.second != nullptr) res.insert(pair.second);
//            AnalysisInterval* temp = *it;
//            temp_set.push_back(temp);
//        } else {
//            res.insert(*it);
//        }
//        it++;
//    }
//    m_intervals = res;
//    for (auto it = temp_set.begin(); it != temp_set.end(); ++it) delete *it;
//    delete rm_interval;
//}

///**
// * @brief Tag::merge_intervals
// * This function assumes m_intervals is sorted
// */
//void Tag::merge_intervals(){
//    std::set<AnalysisInterval*, interval_cmp> intervals = m_intervals;
//    std::set<AnalysisInterval*, interval_cmp> res;
//    auto it = intervals.begin();
//    AnalysisInterval* current = *it;
//    it++;
//    while (it != intervals.end()){
//       if (current->get_end() +1 >= (*it)->get_start()){ // you might want to change it to >=
//           auto m_end = std::max((*it)->get_end(), current->get_end());
//           auto m_start = std::min((*it)->get_start(), current->get_start());
//           AnalysisInterval* merged = new AnalysisInterval(m_start, m_end);
//           AnalysisInterval* temp = *it;
//           current = merged;
//           delete temp;
//       } else {
//           res.insert(current);
//           current = *(it);
//       }
//       it++;
//    }
//    res.insert(current);
//    m_intervals = res;
//}

//bool Tag::is_interval(int start, int end) {
//    return start <= end && start >= 0 && end >= 0;
//}

//std::pair<int,int> Tag::get_overlap(AnalysisInterval* intval1, AnalysisInterval* intval2) {
//    int start = std::max(intval1->get_start(), intval2->get_start());
//    int end = std::min(intval1->get_end(), intval2->get_end());
//    std::pair<int,int> pair = std::make_pair(start, end);
//    return pair;
//}

//std::pair<AnalysisInterval*, AnalysisInterval*> Tag::remove_overlap(AnalysisInterval* interval, std::pair<int, int> overlap) {
//    std::pair<AnalysisInterval*, AnalysisInterval*> pair = std::make_pair(nullptr, nullptr);
//    int start = interval->get_start();
//    int end = interval->get_end();
//    if (interval->get_start() == overlap.first) {
//        if (interval->get_end() == overlap.second) return pair;
//        start = overlap.second +1;
//    } else {
//        end = overlap.first -1;
//    }
//    pair.first = new AnalysisInterval(start, end);
//    if (interval->get_end() != overlap.second) {
//        int new_start = overlap.second +1;
//        int new_end = interval->get_end();
//        pair.second = new AnalysisInterval(new_start, new_end);
//    }
//    return pair;
//}


ANALYSIS_TYPE Interval::get_type() const {
    return INTERVAL;
}

void Interval::write(QJsonObject &json) {

}

void Interval::read(const QJsonObject &json) {

}
