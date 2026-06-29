/****************************************************************************
** Meta object code from reading C++ file 'expWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/sbWarehouse/expWdg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expWdg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_myListWidget_t {
    QByteArrayData data[8];
    char stringdata[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_myListWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_myListWidget_t qt_meta_stringdata_myListWidget = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 18),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 12),
QT_MOC_LITERAL(4, 46, 15),
QT_MOC_LITERAL(5, 62, 18),
QT_MOC_LITERAL(6, 81, 16),
QT_MOC_LITERAL(7, 98, 9)
    },
    "myListWidget\0itemsRemovedSignal\0\0"
    "QVector<int>\0itemRemovedTIds\0"
    "itemDropedOnSignal\0itemDroppedOnTId\0"
    "cassettes\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x05,
       5,    2,   27,    2, 0x05,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    6,    7,

       0        // eod
};

void myListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myListWidget *_t = static_cast<myListWidget *>(_o);
        switch (_id) {
        case 0: _t->itemsRemovedSignal((*reinterpret_cast< QVector<int>(*)>(_a[1]))); break;
        case 1: _t->itemDropedOnSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (myListWidget::*_t)(QVector<int> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&myListWidget::itemsRemovedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (myListWidget::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&myListWidget::itemDropedOnSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject myListWidget::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_myListWidget.data,
      qt_meta_data_myListWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *myListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myListWidget.stringdata))
        return static_cast<void*>(const_cast< myListWidget*>(this));
    return QListWidget::qt_metacast(_clname);
}

int myListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void myListWidget::itemsRemovedSignal(QVector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void myListWidget::itemDropedOnSignal(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_exportStationWidgetClass_t {
    QByteArrayData data[18];
    char stringdata[279];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_exportStationWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_exportStationWidgetClass_t qt_meta_stringdata_exportStationWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 24),
QT_MOC_LITERAL(1, 25, 17),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 17),
QT_MOC_LITERAL(4, 62, 31),
QT_MOC_LITERAL(5, 94, 14),
QT_MOC_LITERAL(6, 109, 16),
QT_MOC_LITERAL(7, 126, 15),
QT_MOC_LITERAL(8, 142, 18),
QT_MOC_LITERAL(9, 161, 11),
QT_MOC_LITERAL(10, 173, 13),
QT_MOC_LITERAL(11, 187, 1),
QT_MOC_LITERAL(12, 189, 15),
QT_MOC_LITERAL(13, 205, 12),
QT_MOC_LITERAL(14, 218, 15),
QT_MOC_LITERAL(15, 234, 16),
QT_MOC_LITERAL(16, 251, 16),
QT_MOC_LITERAL(17, 268, 9)
    },
    "exportStationWidgetClass\0listChangedSignal\0"
    "\0dbUpdateTimerSlot\0exportsListSelectionChangedSlot\0"
    "cassetteUpSlot\0cassetteDownSlot\0"
    "cassetteAddSlot\0cassetteRemoveSlot\0"
    "resizeEvent\0QResizeEvent*\0e\0itemRemovedSlot\0"
    "QVector<int>\0itemRemovedTIds\0"
    "itemDropedOnSlot\0itemDroppedOnTId\0"
    "cassettes\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_exportStationWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x0a,
       4,    0,   66,    2, 0x0a,
       5,    0,   67,    2, 0x0a,
       6,    0,   68,    2, 0x0a,
       7,    0,   69,    2, 0x0a,
       8,    0,   70,    2, 0x0a,
       9,    1,   71,    2, 0x0a,
      12,    1,   74,    2, 0x0a,
      15,    2,   77,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   16,   17,

       0        // eod
};

void exportStationWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        exportStationWidgetClass *_t = static_cast<exportStationWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->listChangedSignal(); break;
        case 1: _t->dbUpdateTimerSlot(); break;
        case 2: _t->exportsListSelectionChangedSlot(); break;
        case 3: _t->cassetteUpSlot(); break;
        case 4: _t->cassetteDownSlot(); break;
        case 5: _t->cassetteAddSlot(); break;
        case 6: _t->cassetteRemoveSlot(); break;
        case 7: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 8: _t->itemRemovedSlot((*reinterpret_cast< QVector<int>(*)>(_a[1]))); break;
        case 9: _t->itemDropedOnSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (exportStationWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&exportStationWidgetClass::listChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject exportStationWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_exportStationWidgetClass.data,
      qt_meta_data_exportStationWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *exportStationWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *exportStationWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_exportStationWidgetClass.stringdata))
        return static_cast<void*>(const_cast< exportStationWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int exportStationWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void exportStationWidgetClass::listChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
