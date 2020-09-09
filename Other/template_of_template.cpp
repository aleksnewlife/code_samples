// Object factory for pool

template<typename TObj>
GameObject* createObjectInPool(ObjectPool<GameObject>* pool)
{
    return (GameObject*)pool->addObject<TObj>();
}

using TCreator = std::function<GameObject* (ObjectPool<GameObject>*)>;
//typedef GameObject* (*TCreator)(ObjectPool<GameObject>*);


std::array<TCreator, 2> ObjectsFactory = { {
    createObjectInPool<GameObject>,
    createObjectInPool<GameObjectB>,
} };


////
