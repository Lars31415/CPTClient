#include "cptrecord.h"

CPTRecord::CPTRecord(QString s) {
    if (!s.isEmpty())
        elements = s.split(';');
}

int CPTRecord::no() const {
    int ret(-1);
    if (!elements.isEmpty()) {
        try {
            bool ok = false;
            ret = elements[0].mid(1).toInt(&ok);
            if (!ok)
                ret = -1;
        } catch (...) {
        }
    }
    return ret;
}
