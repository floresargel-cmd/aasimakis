/****************************************************************************
** Meta object code from reading C++ file 'ls.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/stackingPlan/ls.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ls.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_layerOfSpacersClass_t {
    QByteArrayData data[12];
    char stringdata[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_layerOfSpacersClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_layerOfSpacersClass_t qt_meta_stringdata_layerOfSpacersClass = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 18),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 1),
QT_MOC_LITERAL(4, 42, 15),
QT_MOC_LITERAL(5, 58, 15),
QT_MOC_LITERAL(6, 74, 15),
QT_MOC_LITERAL(7, 90, 15),
QT_MOC_LITERAL(8, 106, 15),
QT_MOC_LITERAL(9, 122, 15),
QT_MOC_LITERAL(10, 138, 15),
QT_MOC_LITERAL(11, 154, 15)
    },
    "layerOfSpacersClass\0onCombCheckBoxSlot\0"
    "\0v\0sp1CheckBoxSlot\0sp2CheckBoxSlot\0"
    "sp3CheckBoxSlot\0sp4CheckBoxSlot\0"
    "sp5CheckBoxSlot\0sp6CheckBoxSlot\0"
    "sp7CheckBoxSlot\0sp8CheckBoxSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_layerOfSpacersClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a,
       4,    1,   62,    2, 0x0a,
       5,    1,   65,    2, 0x0a,
       6,    1,   68,    2, 0x0a,
       7,    1,   71,    2, 0x0a,
       8,    1,   74,    2, 0x0a,
       9,    1,   77,    2, 0x0a,
      10,    1,   80,    2, 0x0a,
      11,    1,   83,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void layerOfSpacersClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        layerOfSpacersClass *_t = static_cast<layerOfSpacersClass *>(_o);
        switch (_id) {
        case 0: _t->onCombCheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sp1CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sp2CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sp3CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->sp4CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->sp5CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->sp6CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->sp7CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->sp8CheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject layerOfSpacersClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_layerOfSpacersClass.data,
      qt_meta_data_layerOfSpacersClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *layerOfSpacersClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *layerOfSpacersClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_layerOfSpacersClass.stringdata))
        return static_cast<void*>(const_cast< layerOfSpacersClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int layerOfSpacersClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
