/**
 * @file UI_ExtDeviceStatus.hpp
 * @date Jul 15, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_SYSTEM_UI_EXT_DEVICE_STATUS_HPP_
#define INC_UI_SYSTEM_UI_EXT_DEVICE_STATUS_HPP_

#include "LibrariesExport.h"

#include <SceneManager/ISceneObject.hpp>
#include <ExtDevice/ExtDevice.hpp>

class LIBRARIES_EXPORT UI_ExtDeviceStatus : public ISceneObject
{
public:
    UI_ExtDeviceStatus() = default;
    UI_ExtDeviceStatus(ExtDevice* device);

    void SetDevice(ExtDevice* device);
    void SetFont(const IFont* font);

    void Draw(IScreen& screen, Point2_i offset = {}) override;
    bool ProcessInput(const AppEvent& event) override;

private:
    ExtDevice* device{};
    const IFont* font{};

    enum { MODES_COUNT = 3 };
    uint8_t mode{};
};

#endif
