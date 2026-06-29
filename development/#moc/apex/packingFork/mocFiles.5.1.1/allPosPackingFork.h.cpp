/****************************************************************************
** Meta object code from reading C++ file 'allPosPackingFork.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/packingFork/allPosPackingFork.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'allPosPackingFork.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_allPosPackingForkClass_t {
    QByteArrayData data[9];
    char stringdata[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_allPosPackingForkClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_allPosPackingForkClass_t qt_meta_stringdata_allPosPackingForkClass = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 22),
QT_MOC_LITERAL(2, 46, 0),
QT_MOC_LITERAL(3, 47, 20),
QT_MOC_LITERAL(4, 68, 20),
QT_MOC_LITERAL(5, 89, 25),
QT_MOC_LITERAL(6, 115, 26),
QT_MOC_LITERAL(7, 142, 10),
QT_MOC_LITERAL(8, 153, 2)
    },
    "allPosPackingForkClass\0mainTimerTriggeredSlot\0"
    "\0posMouseReleasedSlot\0posPackingForkClass*\0"
    "QGraphicsSceneMouseEvent*\0"
    "graphicsViewKeyPressedSlot\0QKeyEvent*\0"
    "ev\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allPosPackingForkClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a,
       3,    2,   30,    2, 0x0a,
       6,    1,   35,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 5,    2,    2,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void allPosPackingForkClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allPosPackingForkClass *_t = static_cast<allPosPackingForkClass *>(_o);
        switch (_id) {
        case 0: _t->mainTimerTriggeredSlot(); break;
        case 1: _t->posMouseReleasedSlot((*reinterpret_cast< posPackingForkClass*(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        case 2: _t->graphicsViewKeyPressedSlot((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< posPackingForkClass* >(); break;
            }
            break;
        }
    }
}

const QMetaObject allPosPackingForkClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_allPosPackingForkClass.data,
      qt_meta_data_allPosPackingForkClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *allPosPackingForkClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allPosPackingForkClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_allPosPackingForkClass.stringdata))
        return static_cast<void*>(const_cast< allPosPackingForkClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int allPosPackingForkClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
