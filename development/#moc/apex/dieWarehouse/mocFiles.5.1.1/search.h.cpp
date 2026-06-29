/****************************************************************************
** Meta object code from reading C++ file 'search.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/dieWarehouse/search.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'search.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_searchWidgetClass_t {
    QByteArrayData data[7];
    char stringdata[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_searchWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_searchWidgetClass_t qt_meta_stringdata_searchWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 21),
QT_MOC_LITERAL(2, 40, 0),
QT_MOC_LITERAL(3, 41, 24),
QT_MOC_LITERAL(4, 66, 16),
QT_MOC_LITERAL(5, 83, 8),
QT_MOC_LITERAL(6, 92, 10)
    },
    "searchWidgetClass\0filterTextChangedSlot\0"
    "\0listSelectionChangedSlot\0QListWidgetItem*\0"
    "selected\0deselected\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_searchWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a,
       3,    2,   27,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 4,    5,    6,

       0        // eod
};

void searchWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        searchWidgetClass *_t = static_cast<searchWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->filterTextChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->listSelectionChangedSlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject searchWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_searchWidgetClass.data,
      qt_meta_data_searchWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *searchWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *searchWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_searchWidgetClass.stringdata))
        return static_cast<void*>(const_cast< searchWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int searchWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
