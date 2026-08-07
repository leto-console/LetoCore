/**
 * @file ISceneManager.hpp
 * @date Aug 05, 2026
 */

#ifndef INC_SCENE_MANAGER_I_SCENE_MANAGER_HPP_
#define INC_SCENE_MANAGER_I_SCENE_MANAGER_HPP_

#include <SceneManager/ISceneBuilder.hpp>

#include <LetoABI/AppEvent.h>

#include <type_traits>
#include <utility>

#include <Graphics/IDrawer.hpp>

class ISceneManager : public IDrawer
{
protected:
    virtual IAllocator& GetSceneAllocator() = 0;

public:
	~ISceneManager() = default;

	virtual IAllocator& GetCommonAllocator() = 0;

	virtual void AddSceneBuilder(uint32_t ID, ISceneBuilder* builder) = 0;
	virtual void SwitchScene(uint32_t ID) = 0;

    /**
	 * @brief Возврат на предыдущую сцену
	 */
	virtual void Return() = 0;

    /// @brief Проверить, существует ли сцена с заданным идентификатор
	/// @param ID Идентификатор проверяемой сцены
	/// @return 
	virtual bool IsExists(uint32_t ID) const = 0;

    virtual uint32_t GetCurrentSceneID() const = 0;

    virtual void ClearScenes() = 0;
	virtual void Loop() override = 0;

	virtual void Draw(IScreen& screen) override = 0;

	virtual bool ProcessEvent(const AppEvent& event) = 0;

	template <typename Scene, typename ID, typename... Args>
	void AddSceneBuilder(ID id, Args... args)
	{
		static_assert(std::is_base_of<ISceneBuilder, typename Scene::Builder>::value);
		ISceneBuilder* builder =  GetSceneAllocator().Make<typename Scene::Builder>(this, std::forward<Args>(args)...);
		AddSceneBuilder((uint32_t) id, builder);
	}

	template <typename ID>
	void SwitchScene(ID id)
	{
		SwitchScene((uint32_t)id);
	}
};

#endif
