/****************************************************************************
** Meta object code from reading C++ file 'craneM.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/packetCrane/craneM.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'craneM.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_packetCraneManualClass_t {
    QByteArrayData data[12];
    char stringdata[254];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_packetCraneManualClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_packetCraneManualClass_t qt_meta_stringdata_packetCraneManualClass = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 26),
QT_MOC_LITERAL(2, 50, 0),
QT_MOC_LITERAL(3, 51, 22),
QT_MOC_LITERAL(4, 74, 19),
QT_MOC_LITERAL(5, 94, 19),
QT_MOC_LITERAL(6, 114, 20),
QT_MOC_LITERAL(7, 135, 20),
QT_MOC_LITERAL(8, 156, 29),
QT_MOC_LITERAL(9, 186, 24),
QT_MOC_LITERAL(10, 211, 26),
QT_MOC_LITERAL(11, 238, 14)
    },
    "packetCraneManualClass\0"
    "onShowConveyorsManualsSlot\0\0"
    "continueMissionBtnSlot\0closeMissionBtnSlot\0"
    "abortMissionBtnSlot\0manualMissionBtnSlot\0"
    "addOilPushButtonSlot\0sendCorrectionsPushButtonSlot\0"
    "showAlarmsHistoryBtnSlot\0"
    "showWarningsHistoryBtnSlot\0alarmTimerSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_packetCraneManualClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a,
       3,    0,   65,    2, 0x0a,
       4,    0,   66,    2, 0x0a,
       5,    0,   67,    2, 0x0a,
       6,    0,   68,    2, 0x0a,
       7,    0,   69,    2, 0x0a,
       8,    0,   70,    2, 0x0a,
       9,    0,   71,    2, 0x0a,
      10,    0,   72,    2, 0x0a,
      11,    0,   73,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void packetCraneManualClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        packetCraneManualClass *_t = static_cast<packetCraneManualClass *>(_o);
        switch (_id) {
        case 0: _t->onShowConveyorsManualsSlot(); break;
        case 1: _t->continueMissionBtnSlot(); break;
        case 2: _t->closeMissionBtnSlot(); break;
        case 3: _t->abortMissionBtnSlot(); break;
        case 4: _t->manualMissionBtnSlot(); break;
        case 5: _t->addOilPushButtonSlot(); break;
        case 6: _t->sendCorrectionsPushButtonSlot(); break;
        case 7: _t->showAlarmsHistoryBtnSlot(); break;
        case 8: _t->showWarningsHistoryBtnSlot(); break;
        case 9: _t->alarmTimerSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject packetCraneManualClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_packetCraneManualClass.data,
      qt_meta_data_packetCraneManualClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *packetCraneManualClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *packetCraneManualClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_packetCraneManualClass.stringdata))
        return static_cast<void*>(const_cast< packetCraneManualClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int packetCraneManualClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
