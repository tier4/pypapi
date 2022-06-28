#!/usr/bin/env python
# encoding: UTF-8

import os
import subprocess

from setuptools import setup, find_packages
from setuptools.command.build_py import build_py


class CustomBuildPy(build_py):

    def run(self):
        os.environ["CFLAGS"] = "%s -fPIC -Werror=format-truncation=0" % os.environ.get("CFLAGS", "")
        subprocess.call("cd papi/src/ && ./configure --enable-perfevent-rdpmc=yes --with-debug=yes", shell=True)  # noqa
        subprocess.call("cd papi/src/ && make clean && make -j$(nproc)", shell=True)  # noqa
        subprocess.call("rm -f pypapi/libpapi.a && cp papi/src/libpapi.a pypapi", shell=True)  # noqa
        subprocess.call('cd pypapi && make -f Makefile', shell=True)  # noqa
        build_py.run(self)


long_description = ""
if os.path.isfile("README.rst"):
    long_description = open("README.rst", "r").read()
elif os.path.isfile("README.md"):
    long_description = open("README.md", "r").read()


setup(
    name="python_papiex",
    version="6.0.0.1",
    description="Python binding for the PAPI library",
    url="https://github.com/tier4/pypapi",
    license="WTFPL",

    long_description=long_description,
    keywords="papi perf performance",

    author="Fabien LOISON, Mathilde BOUTIGNY",
    # author_email="",

    packages=find_packages(),

    setup_requires=["cffi>=1.0.0"],
    install_requires=["cffi>=1.0.0"],

    cffi_modules=[
        "pypapi/papi_build.py:ffibuilder",
    ],

    cmdclass={
        "build_py": CustomBuildPy,
    },
)
