#############################################################################
##
## Copyright (C) 2017 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of PySide2.
##
## $QT_BEGIN_LICENSE:LGPL$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU Lesser General Public License Usage
## Alternatively, this file may be used under the terms of the GNU Lesser
## General Public License version 3 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL3 included in the
## packaging of this file. Please review the following information to
## ensure the GNU Lesser General Public License version 3 requirements
## will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 2.0 or (at your option) the GNU General
## Public license version 3 or any later version approved by the KDE Free
## Qt Foundation. The licenses are as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-2.0.html and
## https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################

import os, sys
import subprocess
from distutils.spawn import find_executable

class QtInfo(object):
    def __init__(self, qmake_command=None):
        if qmake_command:
            self._qmake_command = qmake_command
        else:
            self._qmake_command = [find_executable("qmake"),]
        self._dict = {}
        # bind all variables early at __init__ time.
        for thing in self.__class__.__dict__:
            getattr(self, thing)

    def getQMakeCommand(self):
        qmake_command_string = self._qmake_command[0]
        for entry in self._qmake_command[1:]:
            qmake_command_string += " %s" %(entry)
        return qmake_command_string

    def getVersion(self):
        return self.getProperty("QT_VERSION")

    def getBinsPath(self):
        return self.getProperty("QT_INSTALL_BINS")

    def getLibsPath(self):
        return self.getProperty("QT_INSTALL_LIBS")

    def getPluginsPath(self):
        return self.getProperty("QT_INSTALL_PLUGINS")

    def getImportsPath(self):
        return self.getProperty("QT_INSTALL_IMPORTS")

    def getTranslationsPath(self):
        return self.getProperty("QT_INSTALL_TRANSLATIONS")

    def getHeadersPath(self):
        return self.getProperty("QT_INSTALL_HEADERS")

    def getDocsPath(self):
        return self.getProperty("QT_INSTALL_DOCS")

    def _getProperty(self, prop_name):
        cmd = self._qmake_command + ["-query", prop_name]
        proc = subprocess.Popen(cmd, stdout = subprocess.PIPE, shell=False)
        prop = proc.communicate()[0]
        proc.wait()
        if proc.returncode != 0:
            return None
        if sys.version_info >= (3,):
            return str(prop, 'ascii').strip()
        return prop.strip()

    def getProperty(self, prop_name):
        if prop_name not in self._dict:
            self._dict[prop_name] = self._getProperty(prop_name)
        return self._dict[prop_name]

    version = property(getVersion)
    bins_dir = property(getBinsPath)
    libs_dir = property(getLibsPath)
    plugins_dir = property(getPluginsPath)
    qmake_command = property(getQMakeCommand)
    imports_dir = property(getImportsPath)
    translations_dir = property(getTranslationsPath)
    headers_dir = property(getHeadersPath)
    docs_dir = property(getDocsPath)
