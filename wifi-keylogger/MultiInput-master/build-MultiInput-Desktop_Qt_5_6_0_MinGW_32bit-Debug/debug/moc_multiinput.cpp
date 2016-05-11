/****************************************************************************
** Meta object code from reading C++ file 'multiinput.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt/multiinput.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiinput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultiInput_t {
    QByteArrayData data[8];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiInput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiInput_t qt_meta_stringdata_MultiInput = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MultiInput"
QT_MOC_LITERAL(1, 11, 12), // "displayError"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(4, 54, 11), // "socketError"
QT_MOC_LITERAL(5, 66, 10), // "readStream"
QT_MOC_LITERAL(6, 77, 15), // "socketConnected"
QT_MOC_LITERAL(7, 93, 24) // "on_connectButton_clicked"

    },
    "MultiInput\0displayError\0\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "readStream\0socketConnected\0"
    "on_connectButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiInput[] = {

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
       1,    1,   34,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,
       6,    0,   38,    2, 0x08 /* Private */,
       7,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MultiInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultiInput *_t = static_cast<MultiInput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 1: _t->readStream(); break;
        case 2: _t->socketConnected(); break;
        case 3: _t->on_connectButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject MultiInput::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MultiInput.data,
      qt_meta_data_MultiInput,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MultiInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiInput::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MultiInput.stringdata0))
        return static_cast<void*>(const_cast< MultiInput*>(this));
    return QWidget::qt_metacast(_clname);
}

int MultiInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
