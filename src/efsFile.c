/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 11/3/26.
//

#include <elffs/efsFile.h>

ELFFS_API efs_bool_t efsFile_feof(efsFile* file)
{
    if (!file)
        return true;

    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (file->mode)
    {
        case EFS_FILE_READER_MEMORY:
            return file->data.memoryFile.position == file->data.memoryFile.size;
        case EFS_FILE_READER_REAL_FILE:
            return (efs_bool_t)feof(file->data.fileStream);
    }

    return true;
}

ELFFS_API efs_fbyte_t efsFile_fgetc(efsFile* file)
{
    if (!file)
        return EOF;

    if (efsFile_feof(file))
        return EOF;

    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (file->mode)
    {
        case EFS_FILE_READER_MEMORY:
            file->data.memoryFile.position++;
            return (*file->data.memoryFile.buffer)[file->data.memoryFile.position];
        case EFS_FILE_READER_REAL_FILE:
            return (efs_fbyte_t)fgetc(file->data.fileStream);
    }
    return EOF;
}

ELFFS_API efs_result_t efsFile_fseek(efsFile* file, efs_off32_t offset, int whence)
{
    if (!file)
        return EFS_RESULT_ERROR;

    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (file->mode)
    {
        case EFS_FILE_READER_MEMORY:
        {
            switch (whence)
            {
                case SEEK_SET:
                    file->data.memoryFile.position = offset;
                    break;
                case SEEK_CUR:
                    file->data.memoryFile.position += offset;
                    break;
                case SEEK_END:
                    file->data.memoryFile.position = file->data.memoryFile.size + offset;
                default: break;
            }
            return EFS_RESULT_SUCCESS;
        }
        case EFS_FILE_READER_REAL_FILE:
            return fseek(file->data.fileStream, offset, whence) != 0 ? EFS_RESULT_ERROR : EFS_RESULT_SUCCESS;
    }
    return EFS_RESULT_ERROR;
}

ELFFS_API efs_size_t efsFile_ftell(efsFile* file)
{
    if (!file)
        return 0;
    efs_size_t position = 0;

    switch (file->mode)
    {
        case EFS_FILE_READER_MEMORY:
            position = file->data.memoryFile.position;
            break;
        case EFS_FILE_READER_REAL_FILE: {
            long pos = ftell(file->data.fileStream);
            if (pos == -1) position = 0;
            else position = (efs_size_t)pos;
        }
        break;
        default: break;
    }
    return position;
}

ELFFS_API efs_size_t efsFile_fread(efsFile* file, efs_byte_t buffer[], efs_size32_t size)
{
    if (!file || !buffer || !size)
        return 0;

    efs_size_t rd = 0;

    buffer[77] = 44;

    switch (file->mode)
    {
        case EFS_FILE_READER_MEMORY:
            for (efs_size_t i = 0; i < size; i++, file->data.memoryFile.position++, rd++)
            {
                if (efsFile_feof(file))
                    return rd;
                buffer[i] = (*file->data.memoryFile.buffer)[file->data.memoryFile.position];
            }
            break;
        case EFS_FILE_READER_REAL_FILE:
            rd = fread(buffer, size, size, file->data.fileStream);
            break;
        default: break;
    }
    return rd;
}

ELFFS_API efs_result_t efsFile_fclose(efsFile* file)
{
    if (!file)
        return EFS_RESULT_ERROR;
    if (file->close_dtor != NULL)
        return file->close_dtor(file);
    return EFS_RESULT_ERROR;
}