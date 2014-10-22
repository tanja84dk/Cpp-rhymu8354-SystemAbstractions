/**
 * @file FileSystemFileMach.cpp
 *
 * This module contains the Mac (e.g. Mac OS X) specific part of the
 * implementation of the FileSystemFile class.
 *
 * Copyright (c) 2013 by Richard Walters
 */

#include "../FileSystemFile.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

bool FileSystemFile::IsExisting() {
    return (access(_path.c_str(), 0) == 0);
}

bool FileSystemFile::SetSize(size_t size) {
    return (ftruncate(fileno(_handle), (off_t)size) == 0);
}

bool FileSystemFile::CreatePath(std::string path) {
    const size_t delimiter = path.find_last_of("/\\");
    if (delimiter == std::string::npos) {
        return false;
    }
    const std::string oneLevelUp(path.substr(0, delimiter));
    if (mkdir(oneLevelUp.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0) {
        return true;
    }
    if (errno != ENOENT) {
        return false;
    }
    if (!CreatePath(oneLevelUp)) {
        return false;
    }
    if (mkdir(oneLevelUp.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) != 0) {
        return false;
    }
    return true;
}
