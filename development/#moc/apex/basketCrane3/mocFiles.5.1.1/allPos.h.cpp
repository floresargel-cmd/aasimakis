/****************************************************************************
** Meta object code from reading C++ file 'allPos.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane3/allPos.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'allPos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_allPosClass_t {
    QByteArrayData data[13];
    char stringdata[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_allPosClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_allPosClass_t qt_meta_stringdata_allPosClass = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 23),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 13),
QT_MOC_LITERAL(4, 51, 20),
QT_MOC_LITERAL(5, 72, 9),
QT_MOC_LITERAL(6, 82, 25),
QT_MOC_LITERAL(7, 108, 21),
QT_MOC_LITERAL(8, 130, 3),
QT_MOC_LITERAL(9, 134, 26),
QT_MOC_LITERAL(10, 161, 10),
QT_MOC_LITERAL(11, 172, 2),
QT_MOC_LITERAL(12, 175, 21)
    },
    "allPosClass\0exportListChangedSignal\0"
    "\0mainTimerSlot\0posMouseReleasedSlot\0"
    "posClass*\0QGraphicsSceneMouseEvent*\0"
    "userChangedBasketSlot\0pos\0"
    "graphicsViewKeyPressedSlot\0QKeyEvent*\0"
    "ev\0exportListChangedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allPosClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x0a,
       4,    2,   46,    2, 0x0a,
       7,    1,   51,    2, 0x0a,
       9,    1,   54,    2, 0x0a,
      12,    0,   57,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 6,    2,    2,
    QMetaType::Void, 0x80000000 | 5,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void allPosClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allPosClass *_t = static_cast<allPosClass *>(_o);
        switch (_id) {
        case 0: _t->exportListChangedSignal(); break;
        case 1: _t->mainTimerSlot(); break;
        case 2: _t->posMouseReleasedSlot((*reinterpret_cast< posClass*(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        case 3: _t->userChangedBasketSlot((*reinterpret_cast< posClass*(*)>(_a[1]))); break;
        case 4: _t->graphicsViewKeyPressedSlot((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 5: _t->exportListChangedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< posClass* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< posClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (allPosClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPosClass::exportListChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject allPosClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_allPosClass.data,
      qt_meta_data_allPosClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *allPosClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allPosClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_allPosClass.stringdata))
        return static_cast<void*>(const_cast< allPosClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int allPosClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void allPosClass::exportListChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
