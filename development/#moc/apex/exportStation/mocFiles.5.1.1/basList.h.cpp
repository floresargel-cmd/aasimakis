/****************************************************************************
** Meta object code from reading C++ file 'basList.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/exportStation/basList.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'basList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_basketListClass_t {
    QByteArrayData data[10];
    char stringdata[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_basketListClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_basketListClass_t qt_meta_stringdata_basketListClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 20),
QT_MOC_LITERAL(2, 37, 0),
QT_MOC_LITERAL(3, 38, 16),
QT_MOC_LITERAL(4, 55, 8),
QT_MOC_LITERAL(5, 64, 10),
QT_MOC_LITERAL(6, 75, 12),
QT_MOC_LITERAL(7, 88, 14),
QT_MOC_LITERAL(8, 103, 16),
QT_MOC_LITERAL(9, 120, 13)
    },
    "basketListClass\0listSelectionChanged\0"
    "\0QListWidgetItem*\0selected\0deselected\0"
    "basketUpSlot\0basketDownSlot\0"
    "basketRemoveSlot\0basketAddSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_basketListClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x0a,
       6,    0,   44,    2, 0x0a,
       7,    0,   45,    2, 0x0a,
       8,    0,   46,    2, 0x0a,
       9,    0,   47,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void basketListClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        basketListClass *_t = static_cast<basketListClass *>(_o);
        switch (_id) {
        case 0: _t->listSelectionChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: _t->basketUpSlot(); break;
        case 2: _t->basketDownSlot(); break;
        case 3: _t->basketRemoveSlot(); break;
        case 4: _t->basketAddSlot(); break;
        default: ;
        }
    }
}

const QMetaObject basketListClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_basketListClass.data,
      qt_meta_data_basketListClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *basketListClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *basketListClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_basketListClass.stringdata))
        return static_cast<void*>(const_cast< basketListClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int basketListClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
