#ifdef _WIN32

#include "ff_win.hpp"
#include "disk_img_win.h"

#include "ff.h"

#include <iostream>
#include <fstream>

#include <VirtualConsole/VirtualConsole.hpp>

// Создаёт пустой файл-образ, если его нет
static void ensure_disk_image(const char* path, uint64_t size_mb = 64) {
    fatfs_image_path = path;
    // Проверяем существование файла
    std::ifstream check(path);
    if (check.is_open()) {
        check.close();
        return; // Образ уже существует
    }

    // Создаём новый файл заданного размера
    std::ofstream ofs(path, std::ios::binary | std::ios::out);
    if (!ofs.is_open()) {
        throw std::runtime_error("Cannot create disk image: " + std::string(path));
    }

    uint64_t bytes = size_mb * 1024ULL * 1024ULL;
    if (bytes > 0) {
        // Ленивое выделение (sparse file на NTFS/ext4)
        ofs.seekp(static_cast<std::streamoff>(bytes - 1));
        ofs.put(0);
    }
    // ofs закроется автоматически при выходе из области видимости
}

// Инициализирует FatFS с авто-форматированием
FATFS* fatfs_image_initialize(const char* image_path) {
    ensure_disk_image(image_path, 64);
    
    static FATFS fs_obj;
    FRESULT res;
    
    // 1-я попытка монтирования
    res = f_mount(&fs_obj, "0:", 1);
    
    // Если файловая система не найдена — форматируем
    if (res == FR_NO_FILESYSTEM || res == FR_MKFS_ABORTED) {
        VirtualConsole::Instance().Print("Formatting disk with f_mkfs()", BlueColor);
        
        // Рабочий буфер: минимум FF_MAX_SS байт
        uint8_t work_buf[FF_MAX_SS];
        
        // FatFS R0.14+ API:
        static const MKFS_PARM opt = {FM_FAT32, 0, 0, 0, 0};
        res = f_mkfs("0:", &opt, work_buf, sizeof(work_buf));
        // Для R0.13: f_mkfs(0, FM_FAT, 0, work_buf, sizeof(work_buf));
        
        if (res != FR_OK) {
            VirtualConsole::Instance().Printf("f_mkfs failed: %d", RedColor, (int)res);
            return nullptr;
        }
        
        // После форматирования — перемонтируем
        res = f_mount(&fs_obj, "0:", 1);
    }
    
    if (res != FR_OK) {
        VirtualConsole::Instance().Printf("f_mount failed: %d", RedColor, (int)res);
        return nullptr;
    }
    
    VirtualConsole::Instance().Print("FatFS ready on drive 0:", GreenColor);
    return &fs_obj;
}

#endif