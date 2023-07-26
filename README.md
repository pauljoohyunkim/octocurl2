# octocurl2
C++ implementation of a simple bulk downloader wrapping around curl

## Usage
```
Usage: octocurl2 [options] url[::output filename] [url[::output filename]] ...
Usage options:
  -s [ --sort ]         sort queue after prefetching file size.
  -n [ --nwork ] arg    number of workers (default: 4)
Generic options:
  -h [ --help ]         produce help message
  -v [ --version ]      print version
```
* --sort option can be effective if there are many files of different sizes, as it will attempt to download larger files first.
* --nwork sets the number of concurrent threads (AKA workers) for download.
Note that arbitrarily increasing this **does not necessarily improve** the speed as it is likely that *the bottleneck is the bandwidth*.

### Examples
* Downloading two files
```
octocurl2 http://somewebsite/file1.txt http://somewebsite/file2.txt
```
* Downloading two files with output name specified for the first one (Delimit with ::)
```
octocurl2 http://somewebsite/file1.txt::test1.txt http://somewebsite/file2.txt
```
* Downloading from a text file full of links in each line, with 10 workers, sorted.
```
octocurl2 -s -n 10 $(cat links.txt)
```

## Dependency, Compilation, and Installation
By default, we use *meson* as our build system, so we obviously need that.
Also make sure you have the following dependencies
(if you don't know, when you try to run the commands, you will see it, so don't worry!)
* boost (command line parsing requires this)
* libcurl (obviously!!!)
* threads

You can build and install by the following commands:
```
# For system-wide installation
meson setup --buildtype release build
cd build
ninja
sudo ninja install

# For local user installation
meson setup --prefix=~/.local --buildtype release build
cd build
ninja
ninja install

# For debug build
meson setup build
cd build
ninja
```