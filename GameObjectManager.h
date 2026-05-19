#pragma once
#include "GameObject.h"
#include "DrawManager.h"
class GameObjectManager
{
private:
    std::vector<std::unique_ptr<GameObject>> objects;

    GameObjectManager() = default;
	DrawManager* drawMgr = nullptr;
public:
    static GameObjectManager& GetInstance()
    {
        static GameObjectManager instance;
        return instance;
    }

    template<typename T, typename... Args>
    T* Create(Args&&... args)
    {
        static_assert(std::is_base_of<GameObject, T>::value,
            "T must derive from GameObject");

        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        objects.push_back(std::move(obj));
		drawMgr->Add(ptr); // DrawManagerに登録
        return ptr;
    }

    void Destroy(GameObject* obj);

	// 毎フレーム呼ぶ関数。破棄されたオブジェクトをリストから削除する.
    //必ずKILLの一番最後でよぶこと.
	void CheckDestroyedObjects()
	{
		objects.erase(std::remove_if(objects.begin(), objects.end(),
			[](const std::unique_ptr<GameObject>& obj) { return !obj || obj->IsDestroyed(); }),
			objects.end());
	}

	void SetDrawManager(DrawManager* mgr) { drawMgr = mgr; }

    void Update();
    void Clear();
};
