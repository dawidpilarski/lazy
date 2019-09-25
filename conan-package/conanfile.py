import os

from conans import ConanFile, tools


class LazyConan(ConanFile):
    name = "lazy"
    version = "0.0.1"
    license = "BSD 3-Clause License"
    author = "Dawid Pilarski dawid.pilarski@panicsoftware.com"
    url = "https://github.com/dawidpilarski/lazy"
    description = "lazy type for lazy initialization"
    topics = ("C++17", "lazy-initialization", "header-only")
    no_copy_source = True
    # No settings/options are necessary, this is header only

    def source(self):
        self.run("git clone https://github.com/dawidpilarski/lazy.git")

    def package(self):
        self.copy("*.hpp", dst="include", src="lazy/src/")
