#ifndef ANALYSISINTERVAL_H
#define ANALYSISINTERVAL_H

// TODO Should switch?
#include "Filehandler/saveable.h"
//#include "Filehandler/writeable.h"

#include <QJsonObject>

class AnalysisInterval : public Writeable
{
    using interval = std::pair<int,int>;
protected:
    int m_start = -1;
    int m_end = -1;
public:
    AnalysisInterval(int start, int end);
    AnalysisInterval();
    virtual ~AnalysisInterval();
    bool in_interval(int frame) const;
    int get_start() const;
    int get_end() const;
    bool at_edge(int frame_num);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};

#endif // ANALYSISINTERVAL_H
