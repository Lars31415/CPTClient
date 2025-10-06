#include "cpttest.h"
#include <QDebug>

CPTTest::CPTTest() {}

CPTTest::CPTTest(QString fn) { load_file(fn); }

void CPTTest::clear() {
    file_name.clear();
    raw_data.clear();
    scans.clear();
    dev_idx = 0;

    rec500.reset(nullptr);
    rec501.reset(nullptr);
    rec502.reset(nullptr);
    rec503.reset(nullptr);
    rec504.reset(nullptr);
    rec506.reset(nullptr);
    rec101.reset(nullptr);
    rec102.reset(nullptr);
}

void CPTTest::load_file(QString fn) {
    clear();

    QFile cpt(fn);
    cpt.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!cpt.atEnd()) {
        raw_data.push_back(cpt.readLine());
    }
    scan_data();
}

void CPTTest::add_line(QString s) {
    add_record(s);
    update_derived();
}

void CPTTest::add_record(QString s) {
    CPTRecord rec(s);
    switch (rec.no()) {
    case 0:
        // raw_data.clear();
        break;
    case 100:
        rec100 = std::make_unique<Record100>(rec);
        break;
    case 500:
        rec500 = std::make_unique<Record500>(rec);
        break;
    case 501:
        rec501 = std::make_unique<Record501>(rec);
        break;
    case 502:
        rec502 = std::make_unique<Record502>(rec);
        break;
    case 503:
        rec503 = std::make_unique<Record503>(rec);
        break;
    case 504:
        rec504 = std::make_unique<Record504>(rec);
        break;
    case 506:
        rec506 = std::make_unique<Record506>(rec);
        break;
    case 101:
        if (!rec101.get())
            rec101 = std::make_unique<Record101>(rec);
        else
            *rec101 = rec;
        scans.push_back(calc_scan());
        break;
    case 102:
        rec102 = std::make_unique<Record102>(rec);
        break;
    case 111:
        rec111 = std::make_unique<Record111>(rec);
        break;
    case 902:
        scans.clear();
        dev_idx = 0;
        break;
    }
}

void CPTTest::update_derived() {
    if (rec501.get()) {
        ScanArray::iterator itrs = scans.begin() + dev_idx;
        for (; itrs != scans.end(); ++itrs) {
            itrs->fsm = fs_d(itrs->d + rec501->Dist() / 100.0);
            if (itrs->fsm > 0) {
                if (itrs->qc > 0)
                    itrs->rf = 100.0 * itrs->fsm / itrs->qc;
                itrs->ftm = itrs->fsm - 0.005 * itrs->u;
                if ((itrs->ftm > 0) && (itrs->qt > 0))
                    itrs->rft = 100.0 * itrs->ftm / itrs->qt;
                dev_idx = std::distance(scans.begin(), itrs);
            }
        }
    }
}

void CPTTest::scan_data() {
    QStringList::Iterator itr = raw_data.begin();
    for (; itr != raw_data.end(); ++itr) {
        add_record(*itr);
    }
    update_derived();
}

Scan CPTTest::calc_scan() {
    Scan sc;
    sc.no = rec101->scan_no();
    sc.d = d(rec101->counts());
    sc.qc = qc(rec101->tip_v());
    sc.fs = fs(rec101->sleeve_v(), rec101->tip_v());
    sc.u = u(rec101->pore_v());
    sc.qt = qt(sc);
    return sc;
}

float CPTTest::qc(float vt) const {
    float ret(-9999);
    if ((rec500.get()) && (rec102.get()) && (std::abs(rec500->Area()) > 1E-32))
        ret = rec500->Constant() * (vt - rec102->tip_v()) * 10.0 / rec500->Area();
    return ret;
}

float CPTTest::fs(float vs, float vt) const {
    float ret(-9999);
    if ((rec501.get()) && (rec102.get()) && (std::abs(rec501->Area()) > 1E-32)) {
        float v = vs - rec102->sleeve_v();
        if (rec502.get())
            v -= rec502->Constant() * (vt - rec102->tip_v());
        ret = rec501->Constant() * v * 10.0 / rec501->Area();
    }
    return ret;
}

float CPTTest::u(float vp) const {
    float ret(-9999);
    if ((rec503.get()) && (rec102.get()))
        ret = rec503->Constant() * (vp - rec102->pore_v());
    return ret;
}

float CPTTest::d(float c) const {
    float ret(-9999);
    if ((rec506.get()) && (rec102.get()) &&
            (std::abs(rec506->Constant()) > 1E-32))
        ret = (c - rec102->counts()) / rec506->Constant() - rec100->RefDist();
    return ret;
}

float CPTTest::qt(Scan sc) const {
    float ret(-9999);
    if ((sc.qc > 0) && (sc.u > 0))
        ret = sc.qc + (1 - rec500->Alpha()) * sc.u;
    return ret;
}

ScanArray::const_iterator CPTTest::find_depth(float d) const {
    auto const predicate = [&](auto const &element) {
        return ((element.d >= d) &&
                (std::abs(element.d - d) < .1));
    };
    ScanArray::const_iterator itr =
            std::find_if(scans.begin(), scans.end(), predicate);
    return itr;
}

float CPTTest::fs_d(float d) const {
    auto lambda_fs = [](Scan sc) -> auto {
        return sc.fs;
    };

    return find_value_d(d, lambda_fs);
}

float CPTTest::u_d(float d) const {
    auto lambda_u = [](Scan sc) -> auto {
        return sc.u;
    };

    return find_value_d(d, lambda_u);
}

float CPTTest::find_value_d(float d, val_fnc fnc) const {
    float ret(-9999);
    ScanArray::const_iterator itr = find_depth(d);
    if (itr == scans.end())
        return -9999;
    if ((itr->d == d) ||
            ((itr == scans.begin()) && (std::abs(itr->d - d) < 0.01)))
        return fnc(*itr);
    ScanArray::const_iterator itr2 = itr - 1;
    while ((itr2 != scans.begin()) && (itr2->d == itr->d))
        --itr2;
    if ((itr2 == scans.begin()) && (std::abs(itr2->d - d) < 0.01))
        return fnc(*itr2);
    float dd = itr->d - itr2->d;
    float dv = fnc(*itr) - fnc(*itr2);
    return fnc(*itr2) + (d - itr2->d) / dd * dv;
}
