#!/usr/bin/env python

from distutils.core import setup, Extension

version = "0.93"

long_description = """
    ftpparse is a library for parsing FTP LIST responses.

    This Python module is completely based on Dan Bernstein's ftpparse
    library at http://cr.yp.to/ftpparse.html Mr. Bernstein requests you to
    inform him if you use his library in an commercial application.

    The Python wrapper was hacked by drt@un.bewaff.net - http://c0re.jp/

    You might be able to find more Information and updates at
    http://c0re.jp/c0de/ftpparse/
    """

setup(name="ftpparsemodule",
      version=version,
      description="Parsing of FTP server LIST responses",
      long_description=long_description,
      author="drt",
      author_email="drt@un.bewaff.net",
      url="http://c0re.jp/c0de/ftpparse/",
      ext_modules=[Extension("ftpparse", ["ftpparsemodule.c", "ftpparse.c"],
	                     define_macros=[('VERSION', '"%s"' % (version))])]
      )


