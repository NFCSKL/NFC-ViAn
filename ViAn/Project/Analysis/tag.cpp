#include "tag.h"

Tag::Tag()
{
    type = TAG;
}

/**
 * @brief Tag::add_frame
 * @param frame
 * Adds the newly tagged frame as a new poi
 * unless it's at the edge of a current poi or that frame already is tagged
 */
bool Tag::add_frame(int frame) {
    add_interval(new AnalysisInterval(frame, frame));
}

void Tag::remove_frame(int frame) {
}
void Tag::add_interval(AnalysisInterval *an_interval){
    BasicAnalysis::add_interval(an_interval);
    merge_intervals();
}
/**
 * @brief Tag::merge_intervals
 * This function assumes m_intervals is sorted
 */
void Tag::merge_intervals(){
    std::set<AnalysisInterval*, interval_cmp> intervals = m_intervals;
    std::set<AnalysisInterval*, interval_cmp> res;
    auto it = intervals.begin();
    AnalysisInterval* current = *it;
    it++;
    while (it != intervals.end()){
       if (current->get_interval().second +1 >= (*it)->get_interval().first){ // you might want to change it to >=
           auto m_end = std::max((*it)->get_interval().second, current->get_interval().second);
           auto m_start = current->get_interval().first;
           AnalysisInterval* merged = new AnalysisInterval(m_end,m_start);
           AnalysisInterval* temp = *it;
           current = merged;
           delete temp;
       } else {
           res.insert(current);
           current = *(it);
       }
       it++;
    }
    res.insert(current);
    m_intervals = res;
}
