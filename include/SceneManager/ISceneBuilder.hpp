/**
 * @file ISceneBuilder.hpp
 * @date May 21, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SCENE_MANAGER_I_SCENE_BUILDER_HPP_
#define INC_SCENE_MANAGER_I_SCENE_BUILDER_HPP_

#include "LibrariesExport.h"

#include <Data/IAllocator.hpp>

#include <cstdint>

class ISceneManager;
class IScene;

/**
 * @brief Строитель-синглтон сцен (в перспективе и других объектов)
 */
class LETO_CORE_EXPORT ISceneBuilder
{
public:
    ISceneBuilder(ISceneManager* scene_manager);
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
    IScene* MainCreate(IAllocator& allocator);
    
    /**
     * @brief Очистить память после уничтожения объекта
     */
    void MainDestroy(IAllocator& allocator);

    /**
     * @brief Установить ID на предыдущей сцены
     */
    void SetPrevScene(uint32_t prev) { prev_scene = prev; }

    /**
     * @brief Получить ID предыдущей сцены
     */
    bool GetPrevScene(uint32_t& prev) { prev = prev_scene; return prev_scene; }

protected:
    ISceneManager* const scene_manager;

    /**
     * @brief Создать объект
     */
    virtual IScene* Create(IAllocator& allocator) = 0;

    /**
     * @brief Очистить память после уничтожения объекта
     * 
     * Используется, например, для очистки статического аллокатора
     */
    virtual void Destroy(IAllocator& allocator);

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

// ==========================================================================================

template <typename Scene>
class ISceneNoArgBuilder : public ISceneBuilder
{
    static_assert(std::is_base_of<IScene, Scene>::value);
public:
    ISceneNoArgBuilder(ISceneManager* manager) : ISceneBuilder(manager)
    { }

protected:
    IScene* Create(IAllocator& allocator) override
    {
        return allocator.Make<Scene>(this->scene_manager);
    }
};

#define SCENE_NO_ARGS_BUILDER(Class) using Builder = ISceneNoArgBuilder<Class>;

// ==========================================================================================

template <typename Scene, typename ArgType>
class ISceneOneArgBuilder : public ISceneBuilder
{
    static_assert(std::is_base_of<IScene, Scene>::value);
public:
    ISceneOneArgBuilder(ISceneManager* manager, ArgType arg) : ISceneBuilder(manager), arg{ arg }
    { }

protected:
    ArgType arg;

    IScene* Create(IAllocator& allocator) override
    {
        return allocator.Make<Scene>(this->scene_manager, arg);
    }
};

#define SCENE_ONE_ARG_BUILDER(Class, ArgType) using Builder = ISceneOneArgBuilder<Class, ArgType>;

// ==========================================================================================

template <typename Scene, typename Arg1Type, typename Arg2Type>
class ISceneTwoArgsBuilder : public ISceneBuilder
{
    static_assert(std::is_base_of<IScene, Scene>::value);
public:
    ISceneTwoArgsBuilder(ISceneManager* manager, Arg1Type arg1, Arg2Type arg2) 
        : ISceneBuilder(manager), arg1{ arg1 }, arg2{ arg2 }
    { }

protected:
    Arg1Type arg1;
    Arg2Type arg2;

    IScene* Create(IAllocator& allocator) override
    {
        return allocator.Make<Scene>(this->scene_manager, arg1, arg2);
    }
};

#define SCENE_TWO_ARGS_BUILDER(Class, Arg1Type, Arg2Type) using Builder = ISceneTwoArgsBuilder<Class, Arg1Type, Arg2Type>;

// ==========================================================================================

template <typename Scene, typename Arg1Type, typename Arg2Type, typename Arg3Type>
class ISceneThreeArgsBuilder : public ISceneBuilder
{
    static_assert(std::is_base_of<IScene, Scene>::value);
public:
    ISceneThreeArgsBuilder(ISceneManager* manager, Arg1Type arg1, Arg2Type arg2, Arg3Type arg3) 
        : ISceneBuilder(manager), arg1{ arg1 }, arg2{ arg2 }, arg3{ arg3 }
    { }

protected:
    Arg1Type arg1;
    Arg2Type arg2;
    Arg3Type arg3;

    IScene* Create(IAllocator& allocator) override
    {
        return allocator.Make<Scene>(this->scene_manager, arg1, arg2, arg3);
    }
};

#define SCENE_THREE_ARGS_BUILDER(Class, Arg1Type, Arg2Type, Arg3Type) using Builder = ISceneThreeArgsBuilder<Class, Arg1Type, Arg2Type, Arg3Type>;

// ==========================================================================================

#endif
