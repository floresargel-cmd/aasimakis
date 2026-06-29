/****************************************************************************
** Meta object code from reading C++ file 'expW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/packetCrane/expW.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_exportsWidgetClass_t {
    QByteArrayData data[8];
    char stringdata[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_exportsWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_exportsWidgetClass_t qt_meta_stringdata_exportsWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 24),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 26),
QT_MOC_LITERAL(4, 72, 43),
QT_MOC_LITERAL(5, 116, 42),
QT_MOC_LITERAL(6, 159, 29),
QT_MOC_LITERAL(7, 189, 1)
    },
    "exportsWidgetClass\0addToExportsTableBtnSlot\0"
    "\0removeFromExportsTableSlot\0"
    "contentsTableWidgetItemSelectionChangedSlot\0"
    "exportsTableWidgetItemSelectionChangedSlot\0"
    "filterLineEditTextChangedSlot\0f\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_exportsWidgetClass[] = {

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
       1,    0,   39,    2, 0x0a,
       3,    0,   40,    2, 0x0a,
       4,    0,   41,    2, 0x0a,
       5,    0,   42,    2, 0x0a,
       6,    1,   43,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void exportsWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        exportsWidgetClass *_t = static_cast<exportsWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->addToExportsTableBtnSlot(); break;
        case 1: _t->removeFromExportsTableSlot(); break;
        case 2: _t->contentsTableWidgetItemSelectionChangedSlot(); break;
        case 3: _t->exportsTableWidgetItemSelectionChangedSlot(); break;
        case 4: _t->filterLineEditTextChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject exportsWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_exportsWidgetClass.data,
      qt_meta_data_exportsWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *exportsWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *exportsWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_exportsWidgetClass.stringdata))
        return static_cast<void*>(const_cast< exportsWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int exportsWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
