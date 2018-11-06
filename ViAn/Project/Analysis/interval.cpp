#include "interval.h"

#include <QDebug>

Interval::Interval(std::string name) {
    m_name = name;
}

Interval::~Interval() {

}

void Interval::add_area(int start, int end) {
    m_unsaved_changes = true;
    m_area_list.push_back(std::make_pair(start, end));
}

void Interval::add_area(std::pair<int, int> interval) {
    qDebug() << "add area" << interval.first << interval.second;
    m_unsaved_changes = true;
    m_area_list.push_back(interval);
}

//void Tag::add_interval(AnalysisInterval *an_interval){
//    BasicAnalysis::add_interval(an_interval);
//    merge_intervals();
//}

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
