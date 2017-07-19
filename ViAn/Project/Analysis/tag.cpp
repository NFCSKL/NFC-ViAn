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
    add_interval(new POI(frame, frame));
}

void Tag::remove_frame(int frame) {
}
void Tag::add_interval(POI *an_interval){
    Analysis::add_interval(an_interval);
    merge_intervals();
}
/**
 * @brief Tag::merge_intervals
 * This function assumes m_intervals is sorted
 */
void Tag::merge_intervals(){
    std::set<POI*, poi_cmp> intervals = m_intervals;
    std::set<POI*, poi_cmp> res;
    auto it = intervals.begin();    
    POI* current = *it;
    it++;
    while (it != intervals.end()){
       if (current->getInterval().second +1 >= (*it)->getInterval().first){ // you might want to change it to >=
           POI* merged = new POI(current->getInterval().first, (*it)->getInterval().second);
           POI* temp = current;
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
