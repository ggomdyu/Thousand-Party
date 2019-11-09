#include "TGON.h"
#include "RenderTestScene.h"

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

void RenderTestScene::CreateFontObjects()
{
    FontFactory ff;
    std::shared_ptr<Font> font = ff.CreateFont(u8"Resource/Fonts/malgunbd.ttf");
    auto object = std::make_shared<GameObject>("font");
    object->GetTransform()->SetLocalScale( { 1.0f, 1.0f, 1.0f } );
    object->GetTransform()->SetLocalPosition( Vector3( 0.0f, 0.0f, 0.0f ) );
    auto spriteComponent = object->AddComponent<SpriteRendererComponent>();

    const wchar_t chArray[] = L"A이것은 폰트에요.";
    static auto textureAtlas = TextureAtlas::Create(I32Extent2D(512, 256), PixelFormat::RGBA8888, 20);
    for (auto ch : chArray)
    {
        const auto& glyphData = font->GetGlyphData(ch, 70);
        textureAtlas.Insert(glyphData.ch, ImageView(glyphData.bitmap.get(), glyphData.metrics.size, PixelFormat::RGBA8888));
    }

    spriteComponent->SetSprite(std::make_shared<UISprite>(textureAtlas.GetAtlasTexture()));
    this->AddObject(object);
    return;
    
    float accumulatedXPos = -100.0f;
    float accumulatedYPos = 0.0f;
    for (int i = 0; i < std::extent<decltype(chArray)>::value - 1; ++i)
    {
       int32_t fontSize = 70;
       auto& glyphData = font->GetGlyphData(chArray[i], fontSize);

       if (i != 0)
       {
           auto& prevGlyphData = font->GetGlyphData( chArray[i - 1], fontSize );
           accumulatedXPos -= (prevGlyphData.metrics.size.width - glyphData.metrics.size.width) / 2;

           auto kerning = font->GetKerning( chArray[i - 1], chArray[i], fontSize );
           accumulatedXPos += kerning.x;
       }

       float xPos = accumulatedXPos + glyphData.metrics.bearing.x;
       float yPos = accumulatedYPos - glyphData.metrics.size.height / 2 + glyphData.metrics.bearing.y;

       auto object = std::make_shared<GameObject>("introSprite1");
       object->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
       object->GetTransform()->SetLocalPosition(Vector3( xPos, yPos, 0.0f ));
       auto spriteComponent = object->AddComponent<SpriteRendererComponent>();
       auto texture = std::make_shared<Texture>(&glyphData.bitmap[0], glyphData.metrics.size, PixelFormat::RGBA8888, FilterMode::Point, WrapMode::Clamp, false, false);
       spriteComponent->SetSprite(std::make_shared<UISprite>(texture));

       this->AddObject(object);

       accumulatedXPos += glyphData.metrics.advance.x;
    }
}
