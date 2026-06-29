/****************************************************************************
** Meta object code from reading C++ file 'oW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane1/oW.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_openGLWidgetClass_t {
    QByteArrayData data[11];
    char stringdata[313];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_openGLWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_openGLWidgetClass_t qt_meta_stringdata_openGLWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 32),
QT_MOC_LITERAL(2, 51, 0),
QT_MOC_LITERAL(3, 52, 13),
QT_MOC_LITERAL(4, 66, 45),
QT_MOC_LITERAL(5, 112, 46),
QT_MOC_LITERAL(6, 159, 7),
QT_MOC_LITERAL(7, 167, 30),
QT_MOC_LITERAL(8, 198, 43),
QT_MOC_LITERAL(9, 242, 44),
QT_MOC_LITERAL(10, 287, 24)
    },
    "openGLWidgetClass\0scrapBasketPositionChangedSignal\0"
    "\0basketTableId\0"
    "missionFromAfterStackerPositionFinishedSignal\0"
    "missionFromAfterStacker2PositionFinishedSignal\0"
    "clicked\0scrapBasketPositionChangedSlot\0"
    "missionFromAfterStackerPositionFinishedSlot\0"
    "missionFromAfterStacker2PositionFinishedSlot\0"
    "openGlPartChangedPosSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_openGLWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x05,
       4,    0,   57,    2, 0x05,
       5,    0,   58,    2, 0x05,
       6,    0,   59,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    1,   60,    2, 0x0a,
       8,    0,   63,    2, 0x0a,
       9,    0,   64,    2, 0x0a,
      10,    0,   65,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void openGLWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        openGLWidgetClass *_t = static_cast<openGLWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->scrapBasketPositionChangedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->missionFromAfterStackerPositionFinishedSignal(); break;
        case 2: _t->missionFromAfterStacker2PositionFinishedSignal(); break;
        case 3: _t->clicked(); break;
        case 4: _t->scrapBasketPositionChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->missionFromAfterStackerPositionFinishedSlot(); break;
        case 6: _t->missionFromAfterStacker2PositionFinishedSlot(); break;
        case 7: _t->openGlPartChangedPosSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (openGLWidgetClass::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&openGLWidgetClass::scrapBasketPositionChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (openGLWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&openGLWidgetClass::missionFromAfterStackerPositionFinishedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (openGLWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&openGLWidgetClass::missionFromAfterStacker2PositionFinishedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (openGLWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&openGLWidgetClass::clicked)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject openGLWidgetClass::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_openGLWidgetClass.data,
      qt_meta_data_openGLWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *openGLWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *openGLWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_openGLWidgetClass.stringdata))
        return static_cast<void*>(const_cast< openGLWidgetClass*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int openGLWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void openGLWidgetClass::scrapBasketPositionChangedSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void openGLWidgetClass::missionFromAfterStackerPositionFinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void openGLWidgetClass::missionFromAfterStacker2PositionFinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void openGLWidgetClass::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
