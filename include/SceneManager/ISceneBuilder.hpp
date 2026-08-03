/**
 * @file ISceneBuilder.hpp
 * @date May 21, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SCENE_MANAGER_I_SCENE_BUILDER_HPP_
#define INC_SCENE_MANAGER_I_SCENE_BUILDER_HPP_

#include <SceneManager/IScene.hpp>
#include <Data/IAllocator.hpp>

#include <stdint.h>

/**
 * @brief Строитель-синглтон сцен (в перспективе и других объектов)
 */
class ISceneBuilder
{
public:
    virtual ~ISceneBuilder() = default;
    
    /**
     * @brief Получить созданный объект
     */
    IScene* GetObject() { return scene; }

    /**
     * @brief Создать объект
     * 
     * При многократном вызове будет создан лишь один объект, в остальных случаях 
     * будет возвращена ссылка на первый созданный объект
     */
    IScene* MainCreate(IAllocator& allocator)
    {
        if (!scene) scene = Create(allocator);
        return scene;
    }
    
    /**
     * @brief Очистить память после уничтожения объекта
     */
    void MainDestroy(IAllocator& allocator)
    {
        if (scene)
        {
            scene->~IScene();
            Destroy(allocator);
        }
        scene = nullptr;
    }

    /**
     * @brief Установить ID на предыдущей сцены
     */
    void SetPrevScene(uint32_t prev) { prev_scene = prev; }

    /**
     * @brief Получить ID предыдущей сцены
     */
    bool GetPrevScene(uint32_t& prev) { prev = prev_scene; return prev_scene; }

protected:
    /**
     * @brief Создать объект
     */
    virtual IScene* Create(IAllocator& allocator) = 0;

    /**
     * @brief Очистить память после уничтожения объекта
     * 
     * Используется, например, для очистки статического аллокатора
     */
    virtual void Destroy(IAllocator& allocator)
    {
        allocator.Clear();
    }

private:
    IScene* scene{};
    uint32_t prev_scene{};
};

// Как идея! Для создания функции отложенной инициализации

//template <typename Scene, typename ID, typename... Args>
//Scene* AddScene(ID id, Args... args)
//{
//    static_assert(std::is_base_of<IScene, Scene>::value);
//    Scene* scene = CommonAllocator.Make<Scene>(std::forward<Args>(args)...);
//    AddScene((uint32_t) id, scene);
//    return scene;
//}

template <typename Scene>
class ISceneNoArgBuilder : public ISceneBuilder
{
    static_assert(std::is_base_of<IScene, Scene>::value);
protected:
    IScene* Create(IAllocator& allocator) override
    {
        return allocator.Make<Scene>();
    }
};

#define SCENE_NO_ARGS_BUILDER(Class) using Builder = ISceneNoArgBuilder<Class>;

#endif
