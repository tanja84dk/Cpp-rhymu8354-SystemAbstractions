# CMakeLists.txt for SystemAbstractions
#
# © 2018 by Richard Walters

cmake_minimum_required(VERSION 3.8)
project(SystemAbstractions)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Tanja84dkWarnings.cmake)

find_package(CPM REQUIRED)
find_package(Cpp-rhymu8354-StringExtensions REQUIRED)

set(Headers
    include/SystemAbstractions/Clipboard.hpp
    include/SystemAbstractions/CryptoRandom.hpp
    include/SystemAbstractions/DiagnosticsContext.hpp
    include/SystemAbstractions/DiagnosticsSender.hpp
    include/SystemAbstractions/DiagnosticsStreamReporter.hpp
    include/SystemAbstractions/DirectoryMonitor.hpp
    include/SystemAbstractions/DynamicLibrary.hpp
    include/SystemAbstractions/File.hpp
    include/SystemAbstractions/IFile.hpp
    include/SystemAbstractions/IFileCollection.hpp
    include/SystemAbstractions/IFileSystemEntry.hpp
    include/SystemAbstractions/INetworkConnection.hpp
    include/SystemAbstractions/NetworkConnection.hpp
    include/SystemAbstractions/NetworkEndpoint.hpp
    include/SystemAbstractions/Service.hpp
    include/SystemAbstractions/StringFile.hpp
    include/SystemAbstractions/Subprocess.hpp
    include/SystemAbstractions/TargetInfo.hpp
    include/SystemAbstractions/Time.hpp
)

set(Sources
    src/DataQueue.cpp
    src/DataQueue.hpp
    src/DiagnosticsContext.cpp
    src/DiagnosticsSender.cpp
    src/DiagnosticsStreamReporter.cpp
    src/File.cpp
    src/FileImpl.hpp
    src/NetworkConnection.cpp
    src/NetworkConnectionImpl.hpp
    src/NetworkEndpoint.cpp
    src/NetworkEndpointImpl.hpp
    src/StringFile.cpp
    src/SubprocessInternal.hpp
)

if(MSVC)
    list(APPEND Sources
        src/Win32/ClipboardWin32.cpp
        src/Win32/CryptoRandomWin32.cpp
        src/Win32/DirectoryMonitorWin32.cpp
        src/Win32/DynamicLibraryWin32.cpp
        src/Win32/FileWin32.cpp
        src/Win32/NetworkConnectionWin32.cpp
        src/Win32/NetworkConnectionWin32.hpp
        src/Win32/NetworkEndpointWin32.cpp
        src/Win32/NetworkEndpointWin32.hpp
        src/Win32/ServiceWin32.cpp
        src/Win32/SubprocessWin32.cpp
        src/Win32/TargetInfoWin32.cpp
        src/Win32/TimeWin32.cpp
    )
    set(TargetFolders Win32)
    add_definitions(
        -DUNICODE -D_UNICODE
        -D_CRT_SECURE_NO_WARNINGS
    )
elseif(APPLE)
    list(APPEND Sources
        src/Mach/ClipboardMach.mm
        src/Mach/DirectoryMonitorMach.cpp
        src/Mach/DynamicLibraryMach.cpp
        src/Mach/FileMach.cpp
        src/Mach/ServiceMach.cpp
        src/Mach/SubprocessMach.cpp
        src/Mach/TargetInfoMach.cpp
        src/Mach/TimeMach.cpp
    )
elseif(UNIX)
    list(APPEND Sources
        src/Linux/ClipboardLinux.cpp
        src/Linux/DirectoryMonitorLinux.cpp
        src/Linux/DynamicLibraryLinux.cpp
        src/Linux/FileLinux.cpp
        src/Linux/ServiceLinux.cpp
        src/Linux/SubprocessLinux.cpp
        src/Linux/TargetInfoLinux.cpp
        src/Linux/TimeLinux.cpp
    )
endif()

if(UNIX)
    list(APPEND Sources
        src/Posix/CryptoRandomPosix.cpp
        src/Posix/DynamicLibraryImpl.hpp
        src/Posix/DynamicLibraryPosix.cpp
        src/Posix/FilePosix.cpp
        src/Posix/FilePosix.hpp
        src/Posix/NetworkConnectionPosix.cpp
        src/Posix/NetworkConnectionPosix.hpp
        src/Posix/NetworkEndpointPosix.cpp
        src/Posix/NetworkEndpointPosix.hpp
        src/Posix/PipeSignal.cpp
        src/Posix/PipeSignal.hpp
        src/Posix/SubprocessPosix.cpp
        src/Posix/TimePosix.cpp
    )
endif(UNIX)

add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})

add_library(libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(rhymu8354::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_14
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
    FOLDER Libraries
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    rhymu8354::StringExtensions
)

if(APPLE)
    find_library(AppKit AppKit)
    find_library(CF CoreFoundation)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${CF} ${AppKit})
endif(APPLE)
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        pthread
        dl
    )
endif(UNIX)

target_compile_definitions(${PROJECT_NAME} PRIVATE CLIPBOARD_REVEAL_OS_API)

if(BUILD_TESTING)
	add_subdirectory(test)
endif()