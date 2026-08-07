#include "ISceneBuilder.hpp"

#include "IScene.hpp"
#include "ISceneManager.hpp"

ISceneBuilder::ISceneBuilder(ISceneManager *scene_manager)
    : scene_manager{ scene_manager }
{
}

IScene *ISceneBuilder::MainCreate(IAllocator &allocator)
{
    if (!scene) scene = Create(allocator);
    return scene;
}

void ISceneBuilder::MainDestroy(IAllocator &allocator)
{
    if (scene)
    {
        scene->~IScene();
        Destroy(allocator);
    }
    scene = nullptr;
}

void ISceneBuilder::Destroy(IAllocator &allocator)
{
    allocator.Clear();
}
