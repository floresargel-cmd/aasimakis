/****************************************************************************
** Meta object code from reading C++ file 'co.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/agingOvenApexCode/co.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'co.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_commandsGroupClass_t {
    QByteArrayData data[6];
    char stringdata[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_commandsGroupClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_commandsGroupClass_t qt_meta_stringdata_commandsGroupClass = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 22),
QT_MOC_LITERAL(2, 42, 0),
QT_MOC_LITERAL(3, 43, 24),
QT_MOC_LITERAL(4, 68, 23),
QT_MOC_LITERAL(5, 92, 25)
    },
    "commandsGroupClass\0loadOvenPushButtonSlot\0"
    "\0unloadOvenPushButtonSlot\0"
    "loadOven2PushButtonSlot\0"
    "unloadOven2PushButtonSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_commandsGroupClass[] = {

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
       1,    0,   34,    2, 0x0a,
       3,    0,   35,    2, 0x0a,
       4,    0,   36,    2, 0x0a,
       5,    0,   37,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void commandsGroupClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        commandsGroupClass *_t = static_cast<commandsGroupClass *>(_o);
        switch (_id) {
        case 0: _t->loadOvenPushButtonSlot(); break;
        case 1: _t->unloadOvenPushButtonSlot(); break;
        case 2: _t->loadOven2PushButtonSlot(); break;
        case 3: _t->unloadOven2PushButtonSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject commandsGroupClass::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_commandsGroupClass.data,
      qt_meta_data_commandsGroupClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *commandsGroupClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *commandsGroupClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_commandsGroupClass.stringdata))
        return static_cast<void*>(const_cast< commandsGroupClass*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int commandsGroupClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
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
