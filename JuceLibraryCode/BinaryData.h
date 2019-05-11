/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   downIcon_svg;
    const int            downIcon_svgSize = 2249;

    extern const char*   leftIcon_svg;
    const int            leftIcon_svgSize = 2447;

    extern const char*   musicMid_svg;
    const int            musicMid_svgSize = 24460;

    extern const char*   musicStart_svg;
    const int            musicStart_svgSize = 48707;

    extern const char*   musicStrip_svg;
    const int            musicStrip_svgSize = 151948;

    extern const char*   musicStrip_backup_svg;
    const int            musicStrip_backup_svgSize = 151948;

    extern const char*   rightIcon_svg;
    const int            rightIcon_svgSize = 2446;

    extern const char*   upIcon_svg;
    const int            upIcon_svgSize = 2226;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
