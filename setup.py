#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name="ftpparsemodule",
      version="0.91",
      description="Parsing of FTP server LIST responses",
      author="drt",
      author_email="drt@un.bewaff.net",
      url="http://c0re.jp/c0de/ftpparse/",
      ext_modules=[Extension("ftpparse", ["ftpparsemodule.c", "ftpparse.c"])]
      )


