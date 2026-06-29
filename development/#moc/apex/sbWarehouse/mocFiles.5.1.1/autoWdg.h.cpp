/****************************************************************************
** Meta object code from reading C++ file 'autoWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/sbWarehouse/autoWdg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'autoWdg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_autoWidgetClass_t {
    QByteArrayData data[13];
    char stringdata[345];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_autoWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_autoWidgetClass_t qt_meta_stringdata_autoWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 27),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 5),
QT_MOC_LITERAL(4, 51, 34),
QT_MOC_LITERAL(5, 86, 33),
QT_MOC_LITERAL(6, 120, 41),
QT_MOC_LITERAL(7, 162, 25),
QT_MOC_LITERAL(8, 188, 29),
QT_MOC_LITERAL(9, 218, 29),
QT_MOC_LITERAL(10, 248, 25),
QT_MOC_LITERAL(11, 274, 38),
QT_MOC_LITERAL(12, 313, 30)
    },
    "autoWidgetClass\0autoWestImportsCheckBtnSlot\0"
    "\0state\0autoWestExportsEmptySbCheckBtnSlot\0"
    "autoWestExportsFullSbCheckBtnSlot\0"
    "autoWestExportsEmptyCassettesCheckBtnSlot\0"
    "tripleImportsCheckBtnSlot\0"
    "tripleExportsWestCheckBtnSlot\0"
    "tripleExportsEastCheckBtnSlot\0"
    "doubleImportsCheckBtnSlot\0"
    "doubleExportEmptyCassettesCheckBtnSlot\0"
    "doubleExportFullSBCheckBtnSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_autoWidgetClass[] = {

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
       1,    1,   64,    2, 0x0a,
       4,    1,   67,    2, 0x0a,
       5,    1,   70,    2, 0x0a,
       6,    1,   73,    2, 0x0a,
       7,    1,   76,    2, 0x0a,
       8,    1,   79,    2, 0x0a,
       9,    1,   82,    2, 0x0a,
      10,    1,   85,    2, 0x0a,
      11,    1,   88,    2, 0x0a,
      12,    1,   91,    2, 0x0a,

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
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void autoWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        autoWidgetClass *_t = static_cast<autoWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->autoWestImportsCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->autoWestExportsEmptySbCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->autoWestExportsFullSbCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->autoWestExportsEmptyCassettesCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->tripleImportsCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->tripleExportsWestCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->tripleExportsEastCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->doubleImportsCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->doubleExportEmptyCassettesCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->doubleExportFullSBCheckBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject autoWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_autoWidgetClass.data,
      qt_meta_data_autoWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *autoWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *autoWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_autoWidgetClass.stringdata))
        return static_cast<void*>(const_cast< autoWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int autoWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
