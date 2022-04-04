#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
		//See BMFont Documentation for Binary Layout

		//Parse the Identification bytes (B,M,F)
	char fileId1 = pReader->Read<char>();
	char fileId2 = pReader->Read<char>();
	char fileId3 = pReader->Read<char>();

	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	if (fileId1 != 'B' || fileId2 != 'M' || fileId3 != 'F')
	{
		delete pReader;
		Logger::LogError(L"SpriteFontLoader::LoadContent > Failed to read the assetfile! \nPath: \'%s\'", loadInfo.assetSubPath);

		return nullptr;
	}

	//Parse the version (version 3 required)
	int version = pReader->Read<char>();
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	if (version != 3)
	{
		delete pReader;
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported! \nFile version: \'%i\'", version);

		return nullptr;
	}


	//Valid .fnt file
	SpriteFontDesc fontDesc{};
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId = pReader->Read<char>();
	int blockSize = pReader->Read<int>();
	
	//Retrieve the FontSize
	fontDesc.fontSize = pReader->Read<short>();
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);
	//Retrieve the FontName [SpriteFont::m_FontName]
	fontDesc.fontName = pReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<unsigned short>();
	fontDesc.textureHeight = pReader->Read<unsigned short>();
	//Retrieve PageCount
	int pageCount = pReader->Read<unsigned short>();
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	if (pageCount > 1)
	{
		delete pReader;
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont\n Only one texture per font allowed.");

		return nullptr;
	}

	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	//Retrieve the PageName (store Local)
	std::wstring pageName = pReader->ReadNullString();
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	if (pageName.empty())
	{
		delete pReader;
		Logger::LogError( L"SpriteFontLoader::LoadContent > SpriteFont\n inavlid font sprite (empty).");

		return nullptr;
	}

	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	std::wstring filepath = loadInfo.assetSubPath.substr(0, loadInfo.assetSubPath.rfind('/') + 1);
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	fontDesc.pTexture = ContentManager::Load<TextureData>(filepath + pageName);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	//Retrieve Character Count (see documentation)
	int charCount = blockSize / 20;
	//Retrieve Every Character, For every Character:
		for (wchar_t i = 0; i < charCount; ++i)
		{
			//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		
			auto charId = wchar_t(pReader->Read<unsigned int>());
	
			FontMetric fontMetric;

				
				//> Retrieve Xposition (store Local)
				auto xPos = pReader->Read<unsigned short>();
				//> Retrieve Yposition (store Local)
				auto yPos = pReader->Read<unsigned short>();
				//> Retrieve & Set Width [FontMetric::Width]
				fontMetric.width = pReader->Read<unsigned short>();
				//> Retrieve & Set Height [FontMetric::Height]
				fontMetric.height = pReader->Read<unsigned short>();
				//> Retrieve & Set OffsetX [FontMetric::OffsetX]
				fontMetric.offsetX = pReader->Read<short>();
				//> Retrieve & Set OffsetY [FontMetric::OffsetY]
				fontMetric.offsetY = pReader->Read<short>();
				//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
				fontMetric.advanceX = pReader->Read<short>();
				//> Retrieve & Set Page [FontMetric::Page]
				fontMetric.page = pReader->Read<char>();
				//> Retrieve Channel (BITFIELD!!!) 
				//	> See documentation for BitField meaning [FontMetrix::Channel]

				auto chnl = pReader->Read<char>();
				switch (chnl)
				{
				case 1:fontMetric.channel = 2; break;
				case 2:fontMetric.channel = 1; break;
				case 4:fontMetric.channel = 0; break;
				case 8:fontMetric.channel = 3; break;
				default: fontMetric.channel = 4; break;
				}
				//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
				XMFLOAT2 texture = { float(xPos) / fontDesc.textureWidth , float(yPos)/ fontDesc.textureHeight };
				fontMetric.texCoord = texture;
				fontDesc.metrics.emplace(charId, fontMetric);
		}
		delete pReader;
		return new SpriteFont(fontDesc);
	}
	
	


void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
