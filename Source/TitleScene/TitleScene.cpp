#include "TGON.h"

#include "TitleScene.h"
#include "FireFly.h"
#include "NightSky.h"

using namespace tgon;

TitleScene::TitleScene()
{
}

void TitleScene::Update()
{
    SuperType::Update();

    this->OnHandleInput();

    auto blendColor = m_fadeInSprite->GetBlendColor();
    blendColor.a -= 1.0f * m_timeModule->GetTickTime();
    m_fadeInSprite->SetBlendColor(blendColor);
}

void TitleScene::Initialize()
{
    m_inputModule = Application::GetEngine()->FindModule<InputModule>();
    m_timeModule = Application::GetEngine()->FindModule<TimeModule>();
    
    this->InitializeGraphics();
    this->CreateGameObjects();
}

void TitleScene::InitializeGraphics()
{
    auto engine = Application::GetInstance().GetEngine();
    
    auto graphicsModule = engine->FindModule<GraphicsModule>();
    graphicsModule->GetGraphics().DisableDepthTest();
}

void TitleScene::CreateFontObjects()
{
    /*FontFactory ff;
    auto fontPath = Environment::GetFolderPath(Environment::SpecialFolder::Desktop).value() + "/Temporary/Thousand Party/Resource/Fonts/malgun.ttf";
    std::shared_ptr<Font> font = ff.CreateFont(fontPath.c_str());
    auto object = std::make_shared<GameObject>( "font", new Transform() );
    object->GetTransform()->SetLocalScale( { 1.0f, 1.0f, 1.0f } );
    object->GetTransform()->SetLocalPosition( Vector3( 0.0f, 0.0f, 0.0f ) );
    auto spriteComponent = object->AddComponent<SpriteRendererComponent>();

    const wchar_t chArray[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvrwxyz가나다라마바사아자차카타파하";
    static auto textureAtlas = TextureAtlas::Create(I32Extent2D(512, 512), PixelFormat::RGBA8888, 6);
    for (auto ch : chArray)
    {
        const auto& glyphData = font->GetGlyphData(ch, 35);
        textureAtlas.Insert(glyphData.ch, ImageView(glyphData.bitmap.get(), glyphData.metrics.size, PixelFormat::RGBA8888));
    }

    auto& imageRect = textureAtlas.GetTextureRect(u'A');
    decltype(auto) texture = textureAtlas.GetAtlasTexture();
    decltype(auto) sprite = std::make_shared<UISprite>(texture);
    sprite->SetTextureRect(FRect(imageRect.x, imageRect.y, imageRect.width, imageRect.height));
    spriteComponent->SetSprite(sprite);
    this->AddObject( object );*/
//    static TextureAtlas textureAtlas(I32Extent2D(512, 512), PixelFormat::RGBA8888, 6);
//
//    float accumulatedXPos = -100.0f;
//    float accumulatedYPos = 0.0f;
//    const wchar_t str[] = L"";
//    for (int i = 0; i < std::extent<decltype(str)>::value - 1; ++i)
//    {
//       int32_t fontSize = 30;
//       auto& glyphData = font->GetGlyphData(str[i], fontSize);
//
//       if (i != 0)
//       {
//           auto& prevGlyphData = font->GetGlyphData( str[i - 1], fontSize );
//           accumulatedXPos -= (prevGlyphData.metrics.size.width - glyphData.metrics.size.width) / 2;
//
//           auto kerning = font->GetKerning( str[i - 1], str[i], fontSize );
//           accumulatedXPos += kerning.x;
//       }
//
//       float xPos = accumulatedXPos + glyphData.metrics.bearing.x;
//       float yPos = accumulatedYPos - glyphData.metrics.size.height / 2 + glyphData.metrics.bearing.y;
//
//       auto object = std::make_shared<GameObject>("introSprite1", new Transform());
//       object->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
//       object->GetTransform()->SetLocalPosition(Vector3( xPos, yPos, 0.0f ));
//       auto spriteComponent = object->AddComponent<SpriteRendererComponent>();
//       auto texture = std::make_shared<Texture>(&glyphData.bitmap[0], glyphData.metrics.size, PixelFormat::RGBA8888, FilterMode::Point, WrapMode::Clamp, false, false);
//       spriteComponent->SetSprite(std::make_shared<UISprite>(texture));
//
//       this->AddObject(object);
//
//       accumulatedXPos += glyphData.metrics.advance.x;
//    }
}

void TitleScene::CreateUIObjects()
{
#if TGON_PLATFORM_WINDOWS
    std::string resourceFolderPath = u8"E:/Users/ggomdyu/Desktop/Programming/Git/GitHub/Thousand-Party/Resource";
#else
    std::string resourceFolderPath = u8"/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Resource";
#endif
    std::string texturePathList[] =
    {
        //resourceFolderPath + u8"/Backgrounds/TitleScene/star.png",
        //resourceFolderPath + u8"/Backgrounds/TitleScene/earth.png",
        //resourceFolderPath + u8"/Backgrounds/TitleScene/girl.png",
        //resourceFolderPath + u8"/UI/TitleScene/Logo.png",
        //resourceFolderPath + u8"../../Resource/UI/TitleScene/Press.png",
        resourceFolderPath + u8"/UI/Common/FadeInOut.png",
    };
    
    auto windowSize = Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = windowSize.width * 0.5f;
    float halfWindowHeight = windowSize.height * 0.5f;
    Vector3 texturePosList[] =
    {
        //Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 360.0f, 0.0f),
        //Vector3(-halfWindowWidth + 612.0f, halfWindowHeight - 388.0f, 0.0f),
        //Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 221.0f, 0.0f),
        //Vector3(-halfWindowWidth + 172.5f, halfWindowHeight - 92.5f, 0.0f),
        //Vector3(23.0f, 355.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
    };
    int32_t sortingLayerList[] = {/*0, 0, 2, 2, */3};

    auto nightSky = std::make_shared<NightSky>();
    nightSky->Initialize();
    this->AddObject(nightSky);

    auto assetModule = Application::GetEngine()->FindModule<AssetModule>();
    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto texture = assetModule->LoadTexture(texturePathList[i]);
        auto objectName = StringHash(*Path::GetFileNameWithoutExtension(texturePathList[i]));
        auto object = std::make_shared<GameObject>(objectName);
        object->GetTransform()->SetLocalPosition(texturePosList[i]);
        auto spriteComponent = object->AddComponent<SpriteRendererComponent>(std::make_shared<UISprite>(texture));
        spriteComponent->SetSortingLayer(sortingLayerList[i]);
        this->AddObject(object);
    }

    auto fadeInObject = this->FindObject("FadeInOut");
    m_fadeInSprite = fadeInObject->GetComponent<SpriteRendererComponent>()->GetSprite();   
}

void TitleScene::CreateFireFlyObjects()
{
    for (int i = 0; i < 5; ++i)
    {
        auto fireFly = std::make_shared<FireFly>(StringHash(std::to_string(i)));
        fireFly->Initialize();
        this->AddObject(std::move(fireFly));
    }
}

void TitleScene::CreateGameObjects()
{
    this->CreateFontObjects();
    this->CreateUIObjects();
    this->CreateFireFlyObjects();
}

void TitleScene::OnHandleInput()
{
}
