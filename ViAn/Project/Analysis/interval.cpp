#include "interval.h"

#include <QDebug>
#include <QJsonArray>

Interval::Interval(QString name) {
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
        // Merge overlapping intervals.
        // Starting time of new merged interval is minimum of starting time
        // of both overlapping intervals.
        int start = std::min(interval.first, (*it).first);
        int end = interval.second;

        // Traverse the set while intervals are overlapping
        while (it != m_area_list.end() && overlap) {
            // Ending time of new merged interval is maximum of ending time
            // of both overlapping intervals
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
            return;
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

ANALYSIS_TYPE Interval::get_type() const {
    return INTERVAL;
}

void Interval::write(QJsonObject &json) {
    json["name"] = m_name;
    QJsonArray interval_areas;
    for (auto areas : m_area_list) {
        QJsonObject area;
        area["start"] = areas.first;
        area["end"] = areas.second;
        interval_areas.push_back(area);
    }
    json["areas"] = interval_areas;
    m_unsaved_changes = false;
}

void Interval::read(const QJsonObject &json) {
    std::vector<std::pair<int, int>> new_list;
    m_name = json["name"].toString();
    QJsonArray json_intervals = json["areas"].toArray();
    for (int i = 0; i < json_intervals.size(); ++i) {
        QJsonObject json_frame = json_intervals[i].toObject();
        int start = json_frame["start"].toInt();
        int end = json_frame["end"].toInt();
        std::pair<int, int> pair = std::make_pair(start, end);
        new_list.push_back(pair);
    }
    m_area_list = new_list;
    m_unsaved_changes = false;
}
