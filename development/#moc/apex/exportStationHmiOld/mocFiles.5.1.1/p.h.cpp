/****************************************************************************
** Meta object code from reading C++ file 'p.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/exportStationHmiOld/p.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_pincerClass_t {
    QByteArrayData data[19];
    char stringdata[363];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_pincerClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_pincerClass_t qt_meta_stringdata_pincerClass = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 18),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 23),
QT_MOC_LITERAL(4, 56, 21),
QT_MOC_LITERAL(5, 78, 21),
QT_MOC_LITERAL(6, 100, 14),
QT_MOC_LITERAL(7, 115, 3),
QT_MOC_LITERAL(8, 119, 23),
QT_MOC_LITERAL(9, 143, 28),
QT_MOC_LITERAL(10, 172, 28),
QT_MOC_LITERAL(11, 201, 28),
QT_MOC_LITERAL(12, 230, 19),
QT_MOC_LITERAL(13, 250, 7),
QT_MOC_LITERAL(14, 258, 19),
QT_MOC_LITERAL(15, 278, 26),
QT_MOC_LITERAL(16, 305, 32),
QT_MOC_LITERAL(17, 338, 17),
QT_MOC_LITERAL(18, 356, 5)
    },
    "pincerClass\0buttonReleasedSlot\0\0"
    "onExecuteBtnClickedSlot\0onClearBtnClickedSlot\0"
    "onPositionClickedSlot\0positionClass*\0"
    "pos\0openToReleaseBasketSlot\0"
    "moveFrom60to61PushbuttonSlot\0"
    "moveFrom54to55PushbuttonSlot\0"
    "moveFrom55to54PushbuttonSlot\0"
    "autoRadioButtonSlot\0checked\0"
    "setpRadioButtonSlot\0maintenanceRadioButtonSlot\0"
    "removeBasketBlinkingBtnTimerSlot\0"
    "splitterMovedSlot\0index\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pincerClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a,
       3,    0,   80,    2, 0x0a,
       4,    0,   81,    2, 0x0a,
       5,    1,   82,    2, 0x0a,
       8,    0,   85,    2, 0x0a,
       9,    0,   86,    2, 0x0a,
      10,    0,   87,    2, 0x0a,
      11,    0,   88,    2, 0x0a,
      12,    1,   89,    2, 0x0a,
      14,    1,   92,    2, 0x0a,
      15,    1,   95,    2, 0x0a,
      16,    0,   98,    2, 0x0a,
      17,    2,   99,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,   18,

       0        // eod
};

void pincerClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        pincerClass *_t = static_cast<pincerClass *>(_o);
        switch (_id) {
        case 0: _t->buttonReleasedSlot(); break;
        case 1: _t->onExecuteBtnClickedSlot(); break;
        case 2: _t->onClearBtnClickedSlot(); break;
        case 3: _t->onPositionClickedSlot((*reinterpret_cast< positionClass*(*)>(_a[1]))); break;
        case 4: _t->openToReleaseBasketSlot(); break;
        case 5: _t->moveFrom60to61PushbuttonSlot(); break;
        case 6: _t->moveFrom54to55PushbuttonSlot(); break;
        case 7: _t->moveFrom55to54PushbuttonSlot(); break;
        case 8: _t->autoRadioButtonSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setpRadioButtonSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->maintenanceRadioButtonSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->removeBasketBlinkingBtnTimerSlot(); break;
        case 12: _t->splitterMovedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< positionClass* >(); break;
            }
            break;
        }
    }
}

const QMetaObject pincerClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_pincerClass.data,
      qt_meta_data_pincerClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *pincerClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pincerClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pincerClass.stringdata))
        return static_cast<void*>(const_cast< pincerClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int pincerClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
