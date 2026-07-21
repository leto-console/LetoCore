/**
 * @file FatFsTask.hpp
 * @date May 31, 2026
 * @author Rakhimov T.
 */

#ifndef INC_FATFS_TASK_HPP_
#define INC_FATFS_TASK_HPP_

#include "LibrariesExport.h"

#include <TaskHandler/PriorityTask.hpp>
#include <SDCard/SDCard_ExtDevice.hpp>

/**
 * @brief Задача контроля файловой системы FatFS
 */
class LIBRARIES_EXPORT FatFsTask : public PriorityTask
{
protected:
    SDCard_ExtDevice* sdcard;
    
    /// Обновить состояние оборудования
    bool RefreshHardware();

    /// Обновить состояние примонтированности файловой системы
    void RefreshMount();

    bool Do() override;
public:
    FatFsTask(const StaticText32& name, uint32_t period_ms, uint8_t priority, SDCard_ExtDevice* sdcard);
};

#endif /*INC_FATFS_TASK_HPP_*/
