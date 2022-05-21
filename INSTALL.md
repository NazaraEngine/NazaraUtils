# Install

Simply add the include directory to your include paths, as this library is header-only it will be enough.

## Test

If you wish to compile the unit tests, installs [XMake](https://xmake.io) and enable unit tests with `xmake config --tests=y` before building with `xmake` and running with `xmake run`.

## Package

You can also package the library in a folder by using `xmake install -o target_folder`, on Unix-based system you can also install it system-wide by running `sudo xmake install`.