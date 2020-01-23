#include "PrecompiledHeader.h"

#include "MultipleSceneModule.h"

void MultipleSceneModule::Update()
{
    if (m_nextScene != nullptr)
    {
        m_currScene = std::move(m_nextScene);
        m_currScene->Initialize();
    }
    
    if (m_currScene != nullptr)
    {
        m_currScene->Update();
    }

    for (auto& object : m_globalObjectVector)
    {
        object->Update();
    }
}

void MultipleSceneModule::AddGlobalObject(const std::shared_ptr<tgon::GameObject>& object)
{
    m_globalObjectMap.emplace(object->GetName(), object);
    m_globalObjectVector.push_back(object);
}

void MultipleSceneModule::AddGlobalObject(std::shared_ptr<tgon::GameObject>&& object)
{
    m_globalObjectMap.emplace(object->GetName(), object);
    m_globalObjectVector.push_back(std::move(object));
}

std::shared_ptr<tgon::GameObject> MultipleSceneModule::FindGlobalObject(const tgon::StringViewHash& objectName)
{
    auto iter = m_globalObjectMap.find(objectName);
    if (m_globalObjectMap.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}

bool MultipleSceneModule::RemoveGlobalObject(const tgon::StringViewHash& objectName)
{
    auto mapIter = m_globalObjectMap.find(objectName);
    if (mapIter == m_globalObjectMap.end())
    {
        return false;
    }

    m_globalObjectMap.erase(mapIter);
    m_globalObjectVector.erase(std::find_if(m_globalObjectVector.begin(), m_globalObjectVector.end(), [&](const std::shared_ptr<tgon::GameObject>& item)
    {
        return objectName == item->GetName();
    }));
 
    return true;
}

void MultipleSceneModule::RemoveAllGlobalObject()
{
    m_globalObjectVector.clear();
    m_globalObjectMap.clear();
}
