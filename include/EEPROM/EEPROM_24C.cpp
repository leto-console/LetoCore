/*
 * EEPROM_24C.cpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#include "EEPROM/EEPROM_24C.hpp"

#include <string.h>
#include <stdexcept>
#include <cstdio>

// POSIX-заголовки для fsync/fileno (Linux, macOS, Android/Termux)
#if defined(__TERMUX__) && !defined(_WIN32) || defined(__LINUX__)
#include <unistd.h>      // fsync(), fileno()
#define USE_POSIX_FILE_SYNC
#endif

// Windows-заголовки
#if defined(_WIN32)
#include <io.h>          // _commit(), _fileno()
#define USE_WIN_FILE_SYNC
#endif

#define EEPROM_I2C_ADDR 0x50
#define EEPROM_I2C_TIMEOUT 300

#ifdef USE_HAL_DRIVER
// ========== STM32 HAL  ==========
EEPROM_24C::EEPROM_24C(I2C_HandleTypeDef* hi2c, uint32_t size_bytes) :
		hi2c{ hi2c }, size_bytes{ size_bytes }
{
	addr_size = (size_bytes == 32768 ? 2 : 1);
}
#else
// ========== Эмуляция через файл (Windows / Linux / Termux) ==========
EEPROM_24C::EEPROM_24C(uint32_t size_bytes, const std::string& filename) :
	filename{ filename }, size_bytes { size_bytes }
{
	addr_size = (size_bytes == 32768 ? 2 : 1);

	// Открытие или создание файла
	file_handle = fopen(filename.c_str(), "rb+");
	if (!file_handle) {
		// Файл не существует — создаём новый
		file_handle = fopen(filename.c_str(), "wb+");
		if (!file_handle) {
			throw std::runtime_error("Failed to create EEPROM file: " + filename);
		}
		// Заполняем нулями до нужного размера
		memory = new uint8_t[size_bytes]();
		if (fwrite(memory, 1, size_bytes, file_handle) != size_bytes) {
			fclose(file_handle);
			delete[] memory;
			throw std::runtime_error("Failed to initialize EEPROM file");
		}
		fflush(file_handle);
		SyncFile(); // ✅ Универсальная синхронизация
	}
	else {
		// Файл существует — проверяем размер
		fseek(file_handle, 0, SEEK_END);
		uint32_t file_size = static_cast<uint32_t>(ftell(file_handle));
		fseek(file_handle, 0, SEEK_SET);

		if (file_size < size_bytes) {
			// Дополняем файл до нужного размера
			memory = new uint8_t[size_bytes]();
			if (fread(memory, 1, file_size, file_handle) != file_size) {
				fclose(file_handle);
				delete[] memory;
				throw std::runtime_error("Failed to read existing EEPROM file");
			}
			fseek(file_handle, 0, SEEK_END);
			uint32_t remaining = size_bytes - file_size;
			if (fwrite(memory + file_size, 1, remaining, file_handle) != remaining) {
				fclose(file_handle);
				delete[] memory;
				throw std::runtime_error("Failed to extend EEPROM file");
			}
			fflush(file_handle);
			SyncFile();
			fseek(file_handle, 0, SEEK_SET);
		}
		else {
			// Читаем существующие данные
			memory = new uint8_t[size_bytes];
			if (fread(memory, 1, size_bytes, file_handle) != size_bytes) {
				fclose(file_handle);
				delete[] memory;
				throw std::runtime_error("Failed to read EEPROM file");
			}
		}
	}
}

EEPROM_24C::~EEPROM_24C()
{
	if (file_handle && memory) {
		// Сохраняем изменения перед закрытием
		fseek(file_handle, 0, SEEK_SET);
		fwrite(memory, 1, size_bytes, file_handle);
		fflush(file_handle);
		SyncFile(); // ✅ Гарантируем запись на диск
		delete[] memory;
		fclose(file_handle);
	}
}

// 🔹 Универсальный хелпер для синхронизации файла с диском
void EEPROM_24C::SyncFile() const
{
	if (!file_handle) return;
	
#ifdef USE_WIN_FILE_SYNC
	_commit(_fileno(file_handle));
#elif defined(USE_POSIX_FILE_SYNC)
	fsync(fileno(file_handle)); // POSIX-стандарт
	// На Android/Termux fsync() работает штатно
#endif
	// Если ни один макрос не определён — остаётся только fflush (лучше, чем ничего)
}
#endif // !USE_HAL_DRIVER

// ========== Общие методы ==========

uint32_t EEPROM_24C::GetSizeBytes() const
{
	return size_bytes;
}

#define MIN_MACRO(b, a) (((b) < (a)) ? (b) : (a))

void EEPROM_24C::Write(uint8_t page, uint16_t startByte, const void* data, uint16_t length)
{
#ifdef USE_HAL_DRIVER
	// === STM32 HAL код (без изменений) ===
	uint8_t i2cAddress = (EEPROM_I2C_ADDR | page) << 1;
	while (HAL_I2C_IsDeviceReady(hi2c, i2cAddress, 20, EEPROM_I2C_TIMEOUT) == HAL_BUSY) { }

	constexpr uint16_t PAGE_SIZE = 64;
	uint16_t written = 0;

	while (written < length) {
		uint16_t page_offset = (startByte + written) % PAGE_SIZE;
		uint16_t space_left  = PAGE_SIZE - page_offset;
		uint16_t chunk_size  = MIN_MACRO(length - written, space_left);

		if (HAL_I2C_Mem_Write(hi2c, i2cAddress, startByte + written,
		                      I2C_MEMADD_SIZE_16BIT,
		                      ((uint8_t*)data + written), chunk_size,
		                      EEPROM_I2C_TIMEOUT) != HAL_OK) {
			return;
		}
		HAL_Delay(60);
		written += chunk_size;
	}

#else
	// === Эмуляция: Windows / Linux / Termux ===
	if (!memory || !data) return;

	uint32_t addr = startByte;
	if (addr + length > size_bytes) {
		throw std::out_of_range("EEPROM write out of bounds");
	}

	// Копируем в кэш
	memcpy(memory + addr, data, length);

	// Синхронизируем с файлом
	fseek(file_handle, addr, SEEK_SET);
	fwrite(memory + addr, 1, length, file_handle);
	fflush(file_handle);
	SyncFile(); // ✅ Универсальная синхронизация
#endif
}

void EEPROM_24C::Read(uint8_t page, uint16_t startByte, void* data, uint16_t length) const
{
#ifdef USE_HAL_DRIVER
	// === STM32 HAL код (без изменений) ===
	uint8_t i2cAddress = (EEPROM_I2C_ADDR | page) << 1;
	while (HAL_I2C_IsDeviceReady(hi2c, i2cAddress, 20, EEPROM_I2C_TIMEOUT) == HAL_BUSY) { }

	if (HAL_I2C_Mem_Read(hi2c, i2cAddress, startByte, I2C_MEMADD_SIZE_16BIT,
	                     (uint8_t*)data, length, EEPROM_I2C_TIMEOUT) != HAL_OK) {
		return;
	}
#else
	// === Эмуляция: Windows / Linux / Termux ===
	if (!memory || !data) return;

	uint32_t addr = startByte;
	if (addr + length > size_bytes) {
		throw std::out_of_range("EEPROM read out of bounds");
	}
	memcpy(data, memory + addr, length);
#endif
}

