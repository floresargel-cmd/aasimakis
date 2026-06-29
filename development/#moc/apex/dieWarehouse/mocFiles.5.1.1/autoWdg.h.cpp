/****************************************************************************
** Meta object code from reading C++ file 'autoWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/dieWarehouse/autoWdg.h"
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
    QByteArrayData data[9];
    char stringdata[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_autoWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_autoWidgetClass_t qt_meta_stringdata_autoWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 31),
QT_MOC_LITERAL(2, 48, 0),
QT_MOC_LITERAL(3, 49, 5),
QT_MOC_LITERAL(4, 55, 27),
QT_MOC_LITERAL(5, 83, 17),
QT_MOC_LITERAL(6, 101, 12),
QT_MOC_LITERAL(7, 114, 30),
QT_MOC_LITERAL(8, 145, 4)
    },
    "autoWidgetClass\0exportsCheckBtnStateChangedSlot\0"
    "\0state\0saveAndReturnBtnClickedSlot\0"
    "basketChangedSlot\0basketOnWork\0"
    "basketContentsStateChangedSlot\0isOk\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_autoWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a,
       4,    0,   37,    2, 0x0a,
       5,    1,   38,    2, 0x0a,
       7,    1,   41,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void autoWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        autoWidgetClass *_t = static_cast<autoWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->exportsCheckBtnStateChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->saveAndReturnBtnClickedSlot(); break;
        case 2: _t->basketChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->basketContentsStateChangedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
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
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
