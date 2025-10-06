#ifndef CPTRECORD_H
#define CPTRECORD_H

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QStringList>

class CPTRecord {
public:
    CPTRecord(QString s = "");

    int no() const;

protected:
    QStringList elements;
};

class Record5xx : public CPTRecord {
public:
    Record5xx() : CPTRecord() {}
    Record5xx(const CPTRecord &r) : CPTRecord(r){};

    double Constant() const { return elements[3].toDouble(); }

    Record5xx &operator=(const CPTRecord &r) {
        CPTRecord::operator=(r);
        return *this;
    };
};

class Record50x : public Record5xx {
public:
    Record50x() : Record5xx() {}
    Record50x(const CPTRecord &r) : Record5xx(r){};

    double Area() const { return elements[10].toDouble(); }

    Record50x &operator=(const CPTRecord &r) {
        Record5xx::operator=(r);
        return *this;
    };
};

class Record100 : public CPTRecord {
public:
    Record100() : CPTRecord() {}
    Record100(const CPTRecord &r) : CPTRecord(r){};

    QString cpt_name() const { return elements[1]; }
    double RefDist() const { return elements[7].toDouble(); }

    Record100 &operator=(const CPTRecord &r) {
        CPTRecord::operator=(r);
        return *this;
    };
};

class Record111 : public CPTRecord
{
public:
    Record111()
        : CPTRecord()
    {}
    Record111(const CPTRecord &r)
        : CPTRecord(r) {};

    double MaxDepth() const { return elements[2].toDouble(); }

    Record111 &operator=(const CPTRecord &r)
    {
        CPTRecord::operator=(r);
        return *this;
    };
};

class Record500 : public Record50x
{
public:
    Record500() : Record50x() {}
    Record500(const CPTRecord &r) : Record50x(r){};

    double Alpha() const {
        if (elements.size() < 13)
            return 0.25;
        return elements[12].toDouble();
    }

    Record500 &operator=(const CPTRecord &r) {
        Record50x::operator=(r);
        return *this;
    };
};

class Record501 : public Record50x {
public:
    Record501() : Record50x() {}
    Record501(const CPTRecord &r) : Record50x(r){};

    double Dist() const { return elements[11].toDouble(); }

    Record501 &operator=(const CPTRecord &r) {
        Record50x::operator=(r);
        return *this;
    };
};

class Record502 : public Record5xx {
public:
    Record502() : Record5xx() {}
    Record502(const CPTRecord &r) : Record5xx(r){};

    Record502 &operator=(const CPTRecord &r) {
        Record5xx::operator=(r);
        return *this;
    };
};

class Record503 : public Record5xx {
public:
    Record503() : Record5xx() {}
    Record503(const CPTRecord &r) : Record5xx(r){};

    Record503 &operator=(const CPTRecord &r) {
        Record5xx::operator=(r);
        return *this;
    };
};

class Record504 : public Record5xx {
public:
    Record504() : Record5xx() {}
    Record504(const CPTRecord &r) : Record5xx(r){};

    Record504 &operator=(const CPTRecord &r) {
        Record5xx::operator=(r);
        return *this;
    };
};

class Record506 : public Record5xx {
public:
    Record506() : Record5xx() {}
    Record506(const CPTRecord &r) : Record5xx(r){};

    Record506 &operator=(const CPTRecord &r) {
        Record5xx::operator=(r);
        return *this;
    };
};

class Record101 : public CPTRecord {
public:
    Record101() : CPTRecord() {}
    Record101(const CPTRecord &r) : CPTRecord(r){};

    int scan_no() const { return elements[1].toInt(); }
    QDateTime time() const {
        QString s = elements[2];
        s[10] = 'T';
        return QDateTime::fromString(s, Qt::ISODateWithMs);
    }
    long counts() const { return elements[3].toLong(); }
    double tip_v() const { return elements[4].toDouble(); }
    double sleeve_v() const { return elements[5].toDouble(); }
    double pore_v() const { return elements[6].toDouble(); }

    Record101 &operator=(const CPTRecord &r) {
        CPTRecord::operator=(r);
        return *this;
    };
};

class Record102 : public CPTRecord {
public:
    Record102() : CPTRecord() {}
    Record102(const CPTRecord &r) : CPTRecord(r){};

    QDateTime time() const {
        QString s = elements[1];
        s[10] = 'T';
        return QDateTime::fromString(s, Qt::ISODateWithMs);
    }
    long counts() const { return elements[6].toLong(); }
    double tip_v() const { return elements[7].toDouble(); }
    double sleeve_v() const { return elements[8].toDouble(); }
    double pore_v() const { return elements[9].toDouble(); }

    Record102 &operator=(const CPTRecord &r) {
        CPTRecord::operator=(r);
        return *this;
    };
};

#endif // CPTRECORD_H
