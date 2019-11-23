#include "TGON.h"
#include "RenderTestScene.h"

#include <climits>

using namespace tgon;

RenderTestScene::RenderTestScene()
{
}

void RenderTestScene::Initialize()
{
    auto graphicsModule = Application::GetEngine()->FindModule<GraphicsModule>();
    graphicsModule->GetGraphics().SetClearColor(Color4f(1.0f, 0.5f, 0.5f, 1.0f));
    
    this->CreateCameraObject();
    this->CreateFontObjects();

    auto object = std::make_shared<GameObject>("test");
    auto spriteComponent = object->AddComponent<SpriteRendererComponent>();
    auto sprite = std::make_shared<UISprite>(std::make_shared<Texture>("/Users/chajunho/Desktop/1.png", FilterMode::Bilinear, WrapMode::Clamp, false, false));
    sprite->SetPivot({0.5f, 0.5f});
    spriteComponent->SetSprite(sprite);
    this->AddObject(object);
}

void RenderTestScene::CreateCameraObject()
{
    auto camera = std::make_shared<GameObject>("camera1");
    
    auto rootWindowSize = Application::GetInstance().GetRootWindow()->GetClientSize();
    float halfWidth = static_cast<float>(rootWindowSize.width) * 0.5f;
    float halfHeight = static_cast<float>(rootWindowSize.height) * 0.5f;
    camera->AddComponent<CameraComponent>(FRect(-halfWidth, -halfHeight, rootWindowSize.width, rootWindowSize.height), -1.0f, 1024.0f);
    this->AddGlobalObject(camera);
}

static std::vector<std::tuple<std::shared_ptr<GameObject>, float, float, Vector3>> temp;

void RenderTestScene::Update()
{
    SuperType::Update();
    
//    for (int i = 0; i < temp.size(); ++i)
//    {
//        auto obj = std::get<0>(temp[i]);
//        std::get<1>(temp[i]) += 0.05f;
//        std::get<2>(temp[i]) += 0.05f;
//        Vector3 origin = std::get<3>(temp[i]);
//        obj->GetTransform()->SetLocalPosition(Vector3(origin.x + std::cos(std::get<1>(temp[i])) * 10.0f, origin.y + std::sin(std::get<2>(temp[i])) * 10.0f));
//    }
}

void RenderTestScene::CreateFontObjects()
{
    auto assetModule = Application::GetEngine()->FindModule<AssetModule>();
    std::shared_ptr<Font> font = assetModule->GetFont(u8"Resource/Fonts/malgunbd.ttf");

    const char chArray[] = u8"static auto textureAtlas = TextureAtlas::Create(I32Extent2D(1024, 1024), PixelFormat::RGBA8888, 2); uiText.SetRect(I32Rect(-100, 100, 200, 200)) ";
    static auto textureAtlas = TextureAtlas::Create(I32Extent2D(1024, 1024), PixelFormat::RGBA8888, 2);
    for (auto ch : Encoding::UTF8().GetChars((const std::byte*)&chArray[0], strlen(chArray)))
    {
        const auto& glyphData = font->GetGlyphData(ch, 30);
        textureAtlas.Insert(glyphData.ch, &glyphData.bitmap[0], glyphData.metrics.size);
    }
    
    UIText uiText;
    uiText.SetText(chArray);
    uiText.SetFont(u8"Resource/Fonts/malgunbd.ttf");
    uiText.SetFontSize(30);
    uiText.SetRect(I32Rect(-100, 100, 200, 200));
    uiText.SetTextAlignment(TextAlignment::UpperLeft);
    
    auto fontTexture = textureAtlas.GetAtlasTexture();
    for (auto& characterInfo : uiText.GetCharacterInfos())
    {
        auto object = std::make_shared<GameObject>("introSprite1");
        object->GetTransform()->SetLocalScale({1.0f, 1.0f, 1.0f});
        object->GetTransform()->SetLocalPosition(Vector3(characterInfo.rect.x, characterInfo.rect.y, 0.0f));
        auto spriteComponent = object->AddComponent<SpriteRendererComponent>();
        auto sprite = std::make_shared<UISprite>(fontTexture);
        spriteComponent->SetSprite(sprite);
        sprite->SetTextureRect(textureAtlas.GetTextureRect(characterInfo.character));
        sprite->SetBlendColor(Color4f(0.0f, 1.0f, 1.0f, 1.0f));
        sprite->SetPivot({0.0f, 0.0f});

        this->AddObject(object);
        
        float a = Random().NextDouble(0.0, 3.14159265358);
        temp.push_back({object, a, a, object->GetTransform()->GetLocalPosition()});
    }
}
