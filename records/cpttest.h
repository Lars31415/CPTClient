#ifndef CPTTEST_H
#define CPTTEST_H

#include "cptrecord.h"

#include <QFile>

struct Scan {
    int no;
    float d;
    float qc;
    float fs;
    float u;
    float fsm;
    float rf;
    float qt;
    float ftm;
    float rft;
    Scan()
        : no(0), d(-9999), qc(-9999), fs(-9999), fsm(-9999), u(-9999), rf(-9999),
        qt(-9999), ftm(-9999), rft(-9999) {}
};

typedef std::vector<Scan> ScanArray;

class CPTTest {
public:
    CPTTest();
    CPTTest(QString f);

    void load_file(QString fn);

    void clear();

    void add_line(QString s);

    QString cpt_name() const {
        if (rec100.get())
            return rec100->cpt_name();
        return "";
    }
    double max_depth() const
    {
        if (rec111.get())
            return rec111->MaxDepth();
        return 1;
    }
    ScanArray &data() { return scans; }

private:
    QString file_name;
    QStringList raw_data;
    ScanArray scans;
    size_t dev_idx;

    std::unique_ptr<Record100> rec100;
    std::unique_ptr<Record111> rec111;
    std::unique_ptr<Record500> rec500;
    std::unique_ptr<Record501> rec501;
    std::unique_ptr<Record502> rec502;
    std::unique_ptr<Record503> rec503;
    std::unique_ptr<Record504> rec504;
    std::unique_ptr<Record506> rec506;
    std::unique_ptr<Record101> rec101;
    std::unique_ptr<Record102> rec102;

    void scan_data();
    void add_record(QString s);
    void update_derived();
    Scan calc_scan();
    float qc(float vt) const;
    float fs(float vs, float vt) const;
    float u(float vp) const;
    float d(float c) const;
    float qt(Scan sc) const;
    float ft(Scan sc) const;

    typedef float (*val_fnc)(Scan sc);

    ScanArray::const_iterator find_depth(float d) const;
    float fs_d(float d) const;
    float u_d(float d) const;

    float find_value_d(float d, val_fnc) const;
};

#endif // CPTTEST_H
