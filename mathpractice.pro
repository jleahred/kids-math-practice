#-------------------------------------------------
#
# Project created by QtCreator 2011-09-24T18:37:32
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=gnu++0x


TARGET = mathpractice
TEMPLATE = app


SOURCES += main.cpp\
        math_practice.cpp \
    qoption_button.cpp \
    yaml-cpp-0.2.7/src/tag.cpp \
    yaml-cpp-0.2.7/src/stream.cpp \
    yaml-cpp-0.2.7/src/singledocparser.cpp \
    yaml-cpp-0.2.7/src/simplekey.cpp \
    yaml-cpp-0.2.7/src/scantoken.cpp \
    yaml-cpp-0.2.7/src/scantag.cpp \
    yaml-cpp-0.2.7/src/scanscalar.cpp \
    yaml-cpp-0.2.7/src/scanner.cpp \
    yaml-cpp-0.2.7/src/regex.cpp \
    yaml-cpp-0.2.7/src/parser.cpp \
    yaml-cpp-0.2.7/src/ostream.cpp \
    yaml-cpp-0.2.7/src/null.cpp \
    yaml-cpp-0.2.7/src/nodeownership.cpp \
    yaml-cpp-0.2.7/src/nodebuilder.cpp \
    yaml-cpp-0.2.7/src/node.cpp \
    yaml-cpp-0.2.7/src/iterator.cpp \
    yaml-cpp-0.2.7/src/exp.cpp \
    yaml-cpp-0.2.7/src/emitterutils.cpp \
    yaml-cpp-0.2.7/src/emitterstate.cpp \
    yaml-cpp-0.2.7/src/emitter.cpp \
    yaml-cpp-0.2.7/src/emitfromevents.cpp \
    yaml-cpp-0.2.7/src/directives.cpp \
    yaml-cpp-0.2.7/src/conversion.cpp \
    yaml-cpp-0.2.7/src/aliasmanager.cpp


HEADERS  += math_practice.h \
    qoption_button.h \
    yaml-cpp-0.2.7/include/yaml-cpp/yaml.h \
    yaml-cpp-0.2.7/include/yaml-cpp/traits.h \
    yaml-cpp-0.2.7/include/yaml-cpp/stlnode.h \
    yaml-cpp-0.2.7/include/yaml-cpp/stlemitter.h \
    yaml-cpp-0.2.7/include/yaml-cpp/parser.h \
    yaml-cpp-0.2.7/include/yaml-cpp/ostream.h \
    yaml-cpp-0.2.7/include/yaml-cpp/null.h \
    yaml-cpp-0.2.7/include/yaml-cpp/noncopyable.h \
    yaml-cpp-0.2.7/include/yaml-cpp/nodeutil.h \
    yaml-cpp-0.2.7/include/yaml-cpp/nodereadimpl.h \
    yaml-cpp-0.2.7/include/yaml-cpp/nodeimpl.h \
    yaml-cpp-0.2.7/include/yaml-cpp/node.h \
    yaml-cpp-0.2.7/include/yaml-cpp/mark.h \
    yaml-cpp-0.2.7/include/yaml-cpp/ltnode.h \
    yaml-cpp-0.2.7/include/yaml-cpp/iterator.h \
    yaml-cpp-0.2.7/include/yaml-cpp/exceptions.h \
    yaml-cpp-0.2.7/include/yaml-cpp/eventhandler.h \
    yaml-cpp-0.2.7/include/yaml-cpp/emittermanip.h \
    yaml-cpp-0.2.7/include/yaml-cpp/emitter.h \
    yaml-cpp-0.2.7/include/yaml-cpp/emitfromevents.h \
    yaml-cpp-0.2.7/include/yaml-cpp/dll.h \
    yaml-cpp-0.2.7/include/yaml-cpp/conversion.h \
    yaml-cpp-0.2.7/include/yaml-cpp/anchor.h \
    yaml-cpp-0.2.7/include/yaml-cpp/aliasmanager.h \
    yaml-cpp-0.2.7/src/token.h \
    yaml-cpp-0.2.7/src/tag.h \
    yaml-cpp-0.2.7/src/stringsource.h \
    yaml-cpp-0.2.7/src/streamcharsource.h \
    yaml-cpp-0.2.7/src/stream.h \
    yaml-cpp-0.2.7/src/singledocparser.h \
    yaml-cpp-0.2.7/src/setting.h \
    yaml-cpp-0.2.7/src/scantag.h \
    yaml-cpp-0.2.7/src/scanscalar.h \
    yaml-cpp-0.2.7/src/scanner.h \
    yaml-cpp-0.2.7/src/regeximpl.h \
    yaml-cpp-0.2.7/src/regex.h \
    yaml-cpp-0.2.7/src/ptr_vector.h \
    yaml-cpp-0.2.7/src/ptr_stack.h \
    yaml-cpp-0.2.7/src/nodeownership.h \
    yaml-cpp-0.2.7/src/nodebuilder.h \
    yaml-cpp-0.2.7/src/iterpriv.h \
    yaml-cpp-0.2.7/src/indentation.h \
    yaml-cpp-0.2.7/src/exp.h \
    yaml-cpp-0.2.7/src/emitterutils.h \
    yaml-cpp-0.2.7/src/emitterstate.h \
    yaml-cpp-0.2.7/src/directives.h \
    yaml-cpp-0.2.7/src/collectionstack.h



FORMS    += math_practice.ui


INCLUDEPATH +=  yaml-cpp-0.2.7/include/yaml-cpp/ \
                yaml-cpp-0.2.7/include
