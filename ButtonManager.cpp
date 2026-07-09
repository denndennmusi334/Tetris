#include "stdafx.h"
#include "ButtonManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"

Button& ButtonManager::AddButton(const ButtonData& data)
{
	Button* btn = GameObjectManager::GetInstance().Create<Button>(data);
    buttons.emplace_back(btn);
	return *btn;
}

bool ButtonManager::IsRelease(int id) const
{
    if (id < 0 || id >= (int)buttons.size()) return false;
    return buttons[id]->GetIsPush();
}

void ButtonManager::Draw()
{
}

void ButtonManager::Kill()
{
    buttons.erase(std::remove_if(buttons.begin(), buttons.end(),
        [](GameObject* obj) { return !obj || obj->IsDestroyed(); }),
        buttons.end());
}
void ButtonManager::LoadCSV(const TCHAR* path)
{
    buttons.clear();

    std::ifstream ifs(path);
    if (!ifs.is_open()) return;

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        ButtonData d;
        std::string text;
        int button_type, fill_flag;

        std::getline(ss, text, ',');
        d.text = std::basic_string<TCHAR>(text.begin(), text.end());

        std::string temp;
        std::getline(ss, temp, ','); d.pos.x = std::stof(temp);
        std::getline(ss, temp, ','); d.pos.y = std::stof(temp);
        std::getline(ss, temp, ','); d.width = std::stof(temp);
        std::getline(ss, temp, ','); d.height = std::stof(temp);

        std::getline(ss, temp, ','); d.id = std::stoi(temp);

        std::getline(ss, temp, ','); button_type = std::stoi(temp);
        d.button_type = (BUTTON_TYPE)button_type;

        std::getline(ss, temp, ','); d.fontSize = std::stoi(temp);
        std::getline(ss, temp, ','); d.neutral_color = std::stoul(temp);
        std::getline(ss, temp, ','); d.on_color = std::stoul(temp);
        std::getline(ss, temp, ','); d.text_color = std::stoul(temp);

        std::getline(ss, temp, ','); fill_flag = std::stoi(temp);
        d.fill_flag = (fill_flag != 0);

		int grH = std::stoi(temp);
        if (grH > 0){
            std::getline(ss, temp, ','); d.grH = ImageLoadManager::GetInstance().GetGraphHandle(MyStd::Cast<E_TEXTURE>(grH));
        }
        else {
			d.grH = -1;
        }
        AddButton(d);
    }
}

#if USE_BUTTON_EDIT
void ButtonManager::Update()
{
    auto& iMgr = InputManager::GetInstance();
    float mx = (float)iMgr.GetMouseX();
    float my = (float)iMgr.GetMouseY();

    if (iMgr.IsMouseTriggerLeft())
    {
        selected = nullptr;
        activeDir = ResizeDir::NONE;

        for (auto& b : buttons)
        {
			if (!b->IsActive()) continue;
            ResizeDir dir = b->GetResizeDir(mx, my);
            if (dir != ResizeDir::NONE)
            {
                selected = b;
                activeDir = dir;
                break;
            }

            if (b->HitCheck(mx, my))
            {
                selected = b;
                activeDir = ResizeDir::NONE;
                break;
            }
        }

        if (selected)
            selected->BeginEdit(activeDir, mx, my);
    }

    if (iMgr.IsMousePressLeft() && selected)
    {
        selected->Update(mx, my);
    }

    if (!iMgr.IsMousePressLeft() && selected)
    {
        selected->EndEdit();
        selected = nullptr;
    }
}

void ButtonManager::SaveCSV(const TCHAR* path)
{
    std::wofstream ofs(path);
    if (!ofs.is_open()) return;

    for (auto& b : buttons)
    {
        auto& d = b->GetData();
        ofs << std::wstring(d.text.begin(), d.text.end()) << ","  
            << d.pos.x << "," << d.pos.y << ","
			<< d.width << "," << d.height << "," << d.id << ","
            << (int)d.button_type << ","
            << d.fontSize << ","
            << d.neutral_color << ","
            << d.on_color << ","
            << d.text_color << ","
            << (d.fill_flag ? 1 : 0) << ","
            << d.grH
            << "\n";
    }
}
#else
void ButtonManager::Update()
{
}
#endif
