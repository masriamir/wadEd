# cumin

A simple command-line utility to print `lump` information for a given `WAD` file.

## Building

Build the **Visual Studio** project to generate an `exe`.

## Usage

```
./wadEd.exe FILE.wad
```

## TODO

### Functionality
* Extract raw `lump` data
* View `lump` data in real-time
  * Graphics/sprites/palettes
  * Sounds/MIDI
  * Map `linedefs`, AKA **automap** screenshot
    * Generate table of of `sector`, `linedef`, `sidedef`, and `thing` data
    * Basic map editor **???**
* Convert and output `lump` binary data (graphics, sounds, etc.)

### Code

* C++11/14 *modernization*
  * Use of `Class`
  * `RAII`
  * `vector` and `span`
  * Remove *raw* pointers - use `unique_ptr` and `shared_ptr`
* Apply [CppCoreGuidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) as often as possible (also applies to above)
  * Use **compiler flags** `CppCoreGuidelines-*`
* Consider conversion to **CMake** project from **Sln**
* Finalize/confirm build configurations
