#pragma once
#include "GameObject.h"
#include "DrawManager.h"
class GameObjectManager
{
private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<GameObject>> addBuffer;

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
            "T must derive from GameObject 訳:これGameObject継承してないよ...");

        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        drawMgr->Add(ptr); // DrawManagerに登録
        obj->Initialize(); // 初期化を呼ぶのはここでいいと思う.生成と初期化はセットなことが多いし.
        addBuffer.push_back(std::move(obj));

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
