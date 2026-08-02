/*
 * SceneSettings.hpp
 *
 *  Created on: Dec 27, 2025
 *      Author: Timur
 */

#ifndef INC_SCENE_MANAGER_SCENE_SETTINGS_HPP_
#define INC_SCENE_MANAGER_SCENE_SETTINGS_HPP_

#include <Data/DataCell.hpp>
#include <SceneManager/SceneManager.hpp>

class FPSSetting : public IDataCell<bool>
{
protected:
	bool enabled = false;
public:
	virtual void Set(const bool& enable)
	{
		enabled = enable;
		SceneManager::Instance().EnableFPS(enable);
	}
	virtual bool Get(bool& value) const
	{
		return enabled;
	}
	virtual bool GetOrDefault() const 
	{
		return enabled;
	}
};

extern LETO_CORE_EXPORT FPSSetting EnableFPS_Setting;

#endif