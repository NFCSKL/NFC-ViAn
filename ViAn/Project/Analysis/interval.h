#ifndef INTERVAL_H
#define INTERVAL_H

#include "basicanalysis.h"

#include <QJsonObject>

#include <vector>

class Interval : public BasicAnalysis
{
public:
    Interval(QString name = "");
    ~Interval() override;
    virtual ANALYSIS_TYPE get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject& json) override;

    void add_area(int start, int end);
    void add_area(std::pair<int, int> interval);
    void remove_area_by_frame(int frame);
    void remove_area(std::pair<int, int> interval);

    bool does_overlap(std::pair<int, int> a, std::pair<int, int> b);

    std::vector<std::pair<int, int>> m_area_list;
};

#endif // INTERVAL_H
