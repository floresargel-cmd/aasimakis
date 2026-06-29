/****************************************************************************
** Meta object code from reading C++ file 'ap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/packetCrane/ap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_allPosisionsClass_t {
    QByteArrayData data[9];
    char stringdata[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_allPosisionsClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_allPosisionsClass_t qt_meta_stringdata_allPosisionsClass = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 29),
QT_MOC_LITERAL(2, 48, 0),
QT_MOC_LITERAL(3, 49, 20),
QT_MOC_LITERAL(4, 70, 9),
QT_MOC_LITERAL(5, 80, 25),
QT_MOC_LITERAL(6, 106, 2),
QT_MOC_LITERAL(7, 109, 13),
QT_MOC_LITERAL(8, 123, 27)
    },
    "allPosisionsClass\0mousePressedOnPacketPosSignal\0"
    "\0packetPositionClass*\0packetPos\0"
    "QGraphicsSceneMouseEvent*\0ev\0mainTimerSlot\0"
    "mousePressedOnPacketPosSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allPosisionsClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    0,   34,    2, 0x0a,
       8,    2,   35,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

       0        // eod
};

void allPosisionsClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allPosisionsClass *_t = static_cast<allPosisionsClass *>(_o);
        switch (_id) {
        case 0: _t->mousePressedOnPacketPosSignal((*reinterpret_cast< packetPositionClass*(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        case 1: _t->mainTimerSlot(); break;
        case 2: _t->mousePressedOnPacketPosSlot((*reinterpret_cast< packetPositionClass*(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< packetPositionClass* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< packetPositionClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (allPosisionsClass::*_t)(packetPositionClass * , QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPosisionsClass::mousePressedOnPacketPosSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject allPosisionsClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_allPosisionsClass.data,
      qt_meta_data_allPosisionsClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *allPosisionsClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allPosisionsClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_allPosisionsClass.stringdata))
        return static_cast<void*>(const_cast< allPosisionsClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int allPosisionsClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void allPosisionsClass::mousePressedOnPacketPosSignal(packetPositionClass * _t1, QGraphicsSceneMouseEvent * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
