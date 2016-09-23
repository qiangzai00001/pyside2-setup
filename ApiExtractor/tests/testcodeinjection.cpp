/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of PySide2.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "testcodeinjection.h"
#include <QFileInfo>
#include <QDir>
#include <QtTest/QTest>
#include "testutil.h"

void TestCodeInjections::testReadFileUtf8()
{
    const char* cppCode ="struct A {};";
    int argc = 0;
    char *argv[] = {NULL};
    QCoreApplication app(argc, argv);
    QString filePath = QDir::currentPath();
    QString xmlCode = QLatin1String("\
    <typesystem package=\"Foo\"> \
        <value-type name='A'> \
            <conversion-rule file='") +filePath+ QLatin1String("/utf8code.txt'/>\
            <inject-code class='target' file='") + filePath + QLatin1String("/utf8code.txt' />\
        </value-type>\
        <value-type name='A::B'/> \
    </typesystem>");
    TestUtil t(cppCode, xmlCode.toLocal8Bit().constData());
    AbstractMetaClassList classes = t.builder()->classes();
    AbstractMetaClass* classA = classes.findClass(QLatin1String("A"));
    QCOMPARE(classA->typeEntry()->codeSnips().count(), 1);
    QString code = classA->typeEntry()->codeSnips().first().code();
    QString utf8Data = QString::fromUtf8("\xC3\xA1\xC3\xA9\xC3\xAD\xC3\xB3\xC3\xBA");
    QVERIFY(code.indexOf(utf8Data) != -1);
    code = classA->typeEntry()->conversionRule();
    QVERIFY(code.indexOf(utf8Data) != -1);
}

void TestCodeInjections::testInjectWithValidApiVersion()
{
    const char* cppCode ="struct A {};";
    const char* xmlCode = "\
    <typesystem package='Foo'> \
        <value-type name='A'> \
            <inject-code class='target' since='1.0'>\
                test Inject code\
            </inject-code>\
        </value-type>\
    </typesystem>";

    TestUtil t(cppCode, xmlCode, true, "1.0");

    AbstractMetaClassList classes = t.builder()->classes();
    AbstractMetaClass* classA = classes.findClass(QLatin1String("A"));
    QCOMPARE(classA->typeEntry()->codeSnips().count(), 1);
}

void TestCodeInjections::testInjectWithInvalidApiVersion()
{
    const char* cppCode ="struct A {};";
    const char* xmlCode  = "\
    <typesystem package=\"Foo\"> \
        <value-type name='A'> \
            <inject-code class='target' since='1.0'>\
                test Inject code\
            </inject-code>\
        </value-type>\
    </typesystem>";

    TestUtil t(cppCode, xmlCode, true, "0.1");

    AbstractMetaClassList classes = t.builder()->classes();
    AbstractMetaClass* classA = classes.findClass(QLatin1String("A"));
    QCOMPARE(classA->typeEntry()->codeSnips().count(), 0);
}



QTEST_APPLESS_MAIN(TestCodeInjections)

#include "testcodeinjection.moc"
