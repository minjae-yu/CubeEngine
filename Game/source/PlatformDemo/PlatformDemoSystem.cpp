//Author: DOYEONG LEE
//Project: CubeEngine
//File: PlatformDemoSystem.cpp
#include "PlatformDemo/PlatformDemoSystem.hpp"
#include "PlatformDemo/PPlayer.hpp"
#include "PlatformDemo/PEnemy.hpp"
#include "Engine.hpp"

#include <iostream>
#include <fstream>

#ifdef _DEBUG
#include "imgui.h"
#endif


#ifdef _DEBUG
const char* BackgroundTypeEnumToChar(BackgroundType type)
{
	switch (type)
	{
	case BackgroundType::NORMAL:
		return "NORMAL";
		break;
	case BackgroundType::VPARALLEX:
		return "VPARALLEX";
		break;
	}
	return "NORMAL";
}

BackgroundType BackgroundTypeCharToEnum(const char* type)
{
	if (type == "NORMAL")
	{
		return BackgroundType::NORMAL;
	}
	else if (type == "VPARALLEX")
	{
		return BackgroundType::VPARALLEX;
	}
	return BackgroundType::NONE;
}

const char* BackgroundSpriteNumToChar(int num)
{
	switch (num)
	{
	case 0:
		return "train_editor";
		break;
	case 1:
		return "building1";
		break;
	case 2:
		return "building2";
		break;
	case 3:
		return "building3";
		break;
	case 4:
		return "rail";
		break;
	case 5:
		return "trainSide";
		break;
	}
	return "NORMAL";
}

const char* ObjectTypeToChar(int num)
{
	switch (num)
	{
	case 0:
		return "Player";
		break;
	case 1:
		return "EnemyNormal";
		break;
	case 2:
		return "EnemyBig";
		break;
	}
	return "NORMAL";
}
#endif

void PDemoMapEditorDemo::LoadLevelData(const std::filesystem::path& filePath)
{
	std::ifstream inStream;
	inStream.open(filePath);

	if (inStream.is_open() == false)
	{
		std::cout << "Fail to Access file";
	}

	while (!inStream.eof())
	{
		float posX;
		float posY;
		float sizeX;
		float sizeY;

		std::string objectName;
		std::string spriteName;
		std::string objectType = "";

		inStream >> objectName;
		inStream >> posX;
		inStream >> posY;
		inStream >> sizeX;
		inStream >> sizeY;
		inStream >> objectType;


		if (objectType == "PLAYER")
		{
			if (isEditorMod == false)
			{
				Engine::Instance().GetObjectManager()->AddObject<PPlayer>(glm::vec3{ posX, posY, 0.f }, glm::vec3{ sizeX, sizeY, 0.f }, "Player", ObjectType::PLAYER);
			}
			else
			{
				PPlayer* temp = new PPlayer(glm::vec3{ posX, posY, 0.f }, glm::vec3{ sizeX, sizeY, 0.f }, "Player", ObjectType::PLAYER);
				objects.push_back(std::move(temp));
			}
		}
		else if (objectType == "ENEMY")
		{
			int eType;
			inStream >> eType;
			if (isEditorMod == false)
			{
				Engine::Instance().GetObjectManager()->AddObject<PEnemy>(glm::vec3{ posX, posY, 0.f }, glm::vec3{ sizeX, sizeY, 0.f }, "Enemy", static_cast<EnemyType>(eType));
			}
			else
			{
				PEnemy* temp = new PEnemy(glm::vec3{ posX, posY, 0.f }, glm::vec3{ sizeX, sizeY, 0.f }, "Enemy", static_cast<EnemyType>(eType));
				objects.push_back(std::move(temp));
			}
		}
		else if (objectType == "WALL")
		{
			if (isEditorMod == false)
			{
				Engine::Instance().GetObjectManager()->AddObject<Object>(glm::vec3{ posX, posY, 0.f }, glm::vec3{ sizeX, sizeY, 0.f }, "Wall", ObjectType::WALL);
				Engine::Instance().GetObjectManager()->GetLastObject()->AddComponent<Sprite>();
				Engine::Instance().GetObjectManager()->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 0.5f,0.5f,0.5f,1.f });

				Engine::Instance().GetObjectManager()->GetLastObject()->AddComponent<Physics2D>();
				Engine::Instance().GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygonAABB({ Engine::Instance().GetObjectManager()->GetLastObject()->GetSize().x / 2.f,  Engine::Instance().GetObjectManager()->GetLastObject()->GetSize().y / 2.f });
				Engine::Instance().GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
				Engine::Instance().GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->SetMass(1.f);
			}
			else
			{
				Object* temp = new Object(glm::vec3{ posX, posY,0.f }, glm::vec3{ sizeX, sizeY, 0.f }, "Wall", ObjectType::WALL);
				temp->AddComponent<Sprite>();
				temp->GetComponent<Sprite>()->AddQuad({ 0.f,1.f,0.f,0.25f });

				temp->AddComponent<Physics2D>();
				temp->GetComponent<Physics2D>()->AddCollidePolygonAABB({ temp->GetSize().x / 2.f,  temp->GetSize().y / 2.f });
				temp->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
				temp->GetComponent<Physics2D>()->SetMass(1.f);
				walls.push_back(std::move(temp));
			}
		}
		else if (objectType == "BACKGROUND")
		{
			float speedX;
			float speedY;
			float depth;
			std::string bType;
			bool isScrolled;
			bool isAnimation;

			inStream >> speedX;
			inStream >> speedY;
			inStream >> depth;
			inStream >> bType;
			inStream >> isScrolled;
			inStream >> isAnimation;
			inStream >> spriteName;

			if (isEditorMod == false)
			{
				if (bType == "NORMAL")
				{
					bgm->AddNormalBackground(spriteName, { posX, posY }, { sizeX, sizeY }, 0.f, { speedX, speedY }, { 0.f,0.f }, depth, isScrolled, isAnimation);
				}
				else if (bType == "VPARALLEX")
				{
					bgm->AddHorizonParallexBackground(spriteName, objectName, { posX, posY }, { sizeX, sizeY }, 0.f, speedX, depth, isAnimation);
				}
				bgm->AddSaveBackgroundList(spriteName, "none", BackgroundTypeCharToEnum(bType.c_str()), { posX, posY }, { sizeX, sizeY },
					0.f, { speedX, speedY }, { 0.f,0.f }, depth, false, isAnimation);
			}
			else
			{
				bgm->AddSaveBackgroundList(spriteName, "none", BackgroundTypeCharToEnum(bType.c_str()), { posX, posY }, { sizeX, sizeY },
					0.f, { speedX, speedY }, { 0.f,0.f }, depth, false, isAnimation);
			}
		}
	}
	inStream.close();
}

void PDemoMapEditorDemo::SaveLevelData(const std::filesystem::path& outFilePath)
{
	std::ofstream saveLoad(outFilePath);
	for (auto& group : bgm->GetSaveBackgroundList())
	{
		for (auto& background : group.second)
		{
			float speedX;
			float speedY;
			float depth;
			std::string bType;
			bool isScrolled;
			bool isAnimation;

			saveLoad << group.first << ' ';
			saveLoad << background.position.x << ' ';
			saveLoad << background.position.y << ' ';
			saveLoad << background.size.x << ' ';
			saveLoad << background.size.y << ' ';
			saveLoad << "BACKGROUND" << ' ';
			saveLoad << background.speed.x << ' ';
			saveLoad << background.speed.y << ' ';
			saveLoad << background.depth << ' ';

			switch (background.type)
			{
			case BackgroundType::NORMAL:
				saveLoad << "NORMAL" << ' ';
				break;
			case BackgroundType::VPARALLEX:
				saveLoad << "VPARALLEX" << ' ';
				break;
			}
			saveLoad << background.isScrolled << ' ';
			saveLoad << background.isAnimation << ' ';
			saveLoad << background.spriteName << ' ';
			saveLoad << '\n';
		}
	}
	for (auto& target : walls)
	{
		saveLoad << target->GetName() << ' ';
		saveLoad << target->GetPosition().x << ' ';
		saveLoad << target->GetPosition().y << ' ';
		saveLoad << target->GetSize().x << ' ';
		saveLoad << target->GetSize().y << ' ';
		switch (target->GetObjectType())
		{
		case ObjectType::PLAYER:
			saveLoad << "PLAYER" << ' ';
			break;
		case ObjectType::WALL:
			saveLoad << "WALL" << ' ';
			break;
		}

		saveLoad << '\n';
	}
	for (auto& target : objects)
	{
		saveLoad << target->GetName() << ' ';
		saveLoad << target->GetPosition().x << ' ';
		saveLoad << target->GetPosition().y << ' ';
		saveLoad << target->GetSize().x << ' ';
		saveLoad << target->GetSize().y << ' ';
		switch (target->GetObjectType())
		{
		case ObjectType::PLAYER:
			saveLoad << "PLAYER" << ' ';
			break;
		case ObjectType::ENEMY:
			saveLoad << "ENEMY" << ' ';
			saveLoad << objectNum - 1 << ' ';
			break;
		}

		saveLoad << '\n';
	}


	saveLoad.close();
}

#ifdef _DEBUG
void PDemoMapEditorDemo::Init()
{
	//if (isEditorMod == true)
	//{
	target = new Target();
	target->rect = new Sprite();
	//target->rect->AddMeshWithTexture("", {0.f,1.f,0.f,1.f});
	target->rect->AddQuad({ 0.f,1.f,0.f,0.f });
	//}
}

void PDemoMapEditorDemo::Update(float dt)
{
	if (isEditorMod == true)
	{
		for (auto& o : walls)
		{
			o->GetComponent<Physics2D>()->Update(dt);
			o->GetComponent<Sprite>()->Update(dt);
		}
		for (auto& o : objects)
		{
			o->GetComponent<Physics2D>()->SetGravity(0.f);
			o->GetComponent<Physics2D>()->Update(dt);
			o->GetComponent<Sprite>()->Update(dt);
		}

		ImGui::Begin("MapEditor");
		//Button
		if (ImGui::Button("SaveFile"))
		{
			SaveLevelData("../Game/assets/PlatformDemo/Stage.txt");
		}


		if (ImGui::Button("ObjectCreator"))
		{
			mode = EditorMode::OBJCREATOR;
		}
		ImGui::SameLine();
		if (ImGui::Button("BackGroundCreator"))
		{
			target->rect->ChangeTexture(BackgroundSpriteNumToChar(backGSpriteNum));
			mode = EditorMode::BACKGCREATOR;
		}
		ImGui::SameLine();
		if (ImGui::Button("WallCreator"))
		{
			target->rect->ChangeTexture("");
			mode = EditorMode::WALLCREATOR;
		}
		ImGui::End();

		switch (mode)
		{
		case EditorMode::OBJCREATOR:
			target->rect->SetColor({ 1.f,1.f,1.f,1.f });
			ObjectCreator();
			break;
		case EditorMode::WALLCREATOR:
			target->rect->SetColor({ 0.f,1.f,0.f,1.f });
			WallCreator();
			break;
		case EditorMode::BACKGCREATOR:
			target->rect->SetColor({ 1.f,1.f,1.f,1.f });
			BackgroundCreator();
			break;
		}
	}
}
void PDemoMapEditorDemo::End()
{
	delete target->rect;
	delete target;
	for (auto* target : objects)
	{
		delete target;
	}
	for (auto* target : walls)
	{
		delete target;
	}
	objects.clear();
	walls.clear();
}
#endif

void PDemoMapEditorDemo::ObjectCreator()
{
	float  targetP[2] = { target->pos.x, target->pos.y };
	float  targetScale[2] = { target->size.x, target->size.y };

	if (ImGui::BeginCombo("ObjectList", ObjectTypeToChar(objectNum)))
	{
		for (int i = 0; i < BackgroundType::NONE; i++)
		{
			if (ImGui::Selectable(ObjectTypeToChar(i), i))
			{
				objectNum = (i);
			}
			if (objectNum == (i))
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Begin("ObjectCreator");
	static char newName[256] = "none";
	ImGui::InputText("default", newName, 256);
	target->name = newName;

	ImGui::InputFloat2("Position", targetP);
	glm::vec2 mPos = Engine::Instance().GetInputManager()->GetMousePosition();
	glm::vec2 newPosition = { mPos.x - std::fmod(mPos.x, gridSize.x),  mPos.y - std::fmod(mPos.y, gridSize.y) };
	target->pos = newPosition;

	ImGui::InputFloat2("Scale", targetScale);
	glm::vec2  newSize = { targetScale[0], targetScale[1] };
	target->size = newSize;

	target->rect->UpdateModel({ target->pos.x, -target->pos.y, 0.f }, { newSize.x,newSize.y,0.f }, 0.f);

	int id = 0;
	for (auto& obj : objects)
	{
		if (!(obj->GetPosition().x + obj->GetSize().x / 2.f < mPos.x || mPos.y < obj->GetPosition().x - obj->GetSize().x / 2.f
			|| obj->GetPosition().y + obj->GetSize().y / 2.f < mPos.y || mPos.y < obj->GetPosition().y - obj->GetSize().y / 2.f) && Engine::GetInputManager()->IsMouseButtonPressedOnce(MOUSEBUTTON::RIGHT))
		{
			delete obj;
			objects.erase(objects.begin() + id);
			break;
		}
		id++;
	}
	if (Engine::GetInputManager()->IsMouseButtonPressedOnce(MOUSEBUTTON::MIDDLE))
	{
		switch (objectNum)
		{
		case 0:
			objects.push_back(new PPlayer({ target->pos.x, target->pos.y, 0.f }, { target->size.x, target->size.y, 0.f }, "Player"));
			break;
		case 1:
			objects.push_back(new PEnemy({ target->pos.x, target->pos.y, 0.f }, { target->size.x, target->size.y, 0.f }, "Enemy", EnemyType::NORMAL));
			break;
		case 2:
			objects.push_back(new PEnemy({ target->pos.x, target->pos.y, 0.f }, { target->size.x, target->size.y, 0.f }, "Enemy", EnemyType::BIG));
			break;
		}
	}
	ImGui::End();
}

void PDemoMapEditorDemo::BackgroundCreator()
{
	float  targetP[2] = { target->pos.x, target->pos.y };
	float  targetScale[2] = { target->size.x, target->size.y };
	float  targetSpeed[2] = { target->speed.x, target->speed.y };
	float targetDepth = target->depth;

	ImGui::Begin("BackgroundCreator");

	if (ImGui::BeginCombo("BackgroundtTypeList", BackgroundTypeEnumToChar(target->backgroundType)))
	{
		for (int i = 0; i < BackgroundType::NONE; i++)
		{
			if (ImGui::Selectable(BackgroundTypeEnumToChar(static_cast<BackgroundType>(i)), i))
			{
				target->backgroundType = static_cast<BackgroundType>(i);
			}
			if (target->backgroundType == static_cast<BackgroundType>(i))
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Sprite", BackgroundSpriteNumToChar(backGSpriteNum)))
	{
		for (int i = 0; i < 6; i++)
		{
			if (ImGui::Selectable(BackgroundSpriteNumToChar(i), i))
			{
				target->spriteName = BackgroundSpriteNumToChar(i);
				target->rect->ChangeTexture(BackgroundSpriteNumToChar(i));
			}
			if (target->spriteName == BackgroundSpriteNumToChar(i))
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::InputFloat2("Position", targetP);
	glm::vec2 mPos = Engine::Instance().GetInputManager()->GetMousePosition();
	glm::vec2 newPosition = { mPos.x - std::fmod(mPos.x, gridSize.x),  -(mPos.y - std::fmod(mPos.y, gridSize.y)) };
	target->pos = newPosition;

	ImGui::InputFloat2("Scale", targetScale);
	glm::vec2  newSize = { targetScale[0], targetScale[1] };
	target->size = newSize;

	ImGui::InputFloat2("Speed", targetSpeed);
	target->speed.x = targetSpeed[0];
	target->speed.y = targetSpeed[1];

	ImGui::InputFloat("Depth", &targetDepth, 0.1f, 1.0f);
	target->depth = targetDepth;


	for (auto& group : bgm->GetSaveBackgroundList())
	{
		for (int i = 0; i < group.second.size(); i++)
		{
			if (!(group.second.at(i).position.x + group.second.at(i).size.x < mPos.x || mPos.y < group.second.at(i).position.x - group.second.at(i).size.x
				|| group.second.at(i).position.y + group.second.at(i).size.y < mPos.y || mPos.y < group.second.at(i).position.y - group.second.at(i).size.y) && Engine::GetInputManager()->IsMouseButtonPressedOnce(MOUSEBUTTON::RIGHT))
			{
				delete group.second.at(i).sprite;
				group.second.erase(group.second.begin() + i);
				break;
			}
		}
	}
	if (Engine::GetInputManager()->IsMouseButtonPressedOnce(MOUSEBUTTON::MIDDLE))
	{
		bgm->AddSaveBackgroundList(target->spriteName, "none", target->backgroundType, target->pos, target->size,
			0.f, target->speed, { 0.f,0.f }, target->depth, false, target->isAnimation);
	}

	ImGui::End();
}

void PDemoMapEditorDemo::WallCreator()
{
	//ImGuiIO& io = ImGui::GetIO();
	//ImVec2 cursor_pos = io.MousePos;
	//bool cursor_over_imgui_window = ImGui::IsMouseHoveringAnyWindow();
	//if()
	{
		float distance = glm::distance(target->startPos, target->endPos);
		glm::vec2 midPoint = { (target->startPos.x + target->endPos.x) / 2.f, (target->startPos.y + target->endPos.y) / 2.f };
		if (isWallSetting == false)
		{
			glm::vec2 mPos = Engine::Instance().GetInputManager()->GetMousePosition();
			target->startPos = { mPos.x - std::fmod(mPos.x, gridSize.x),  mPos.y - std::fmod(mPos.y, gridSize.y) };
			target->rect->UpdateModel({ target->startPos.x, -target->startPos.y, 0.f }, { 4.f,4.f,0.f }, 0.f);
		}
		else
		{
			glm::vec2 mPos = Engine::Instance().GetInputManager()->GetMousePosition();
			target->endPos = { mPos.x - std::fmod(mPos.x, gridSize.x),  mPos.y - std::fmod(mPos.y, gridSize.y) };
			target->rect->UpdateModel({ midPoint.x, -midPoint.y, 0.f }, { abs(target->endPos.x - target->startPos.x) , abs(target->endPos.y - target->startPos.y),0.f }, 0.f);
		}
		target->rect->UpdateProjection();
		target->rect->UpdateView();

		if (Engine::Instance().GetInputManager()->IsMouseButtonPressedOnce(MOUSEBUTTON::MIDDLE))
		{
			if (isWallSetting == true)
			{
				Object* temp = new Object(glm::vec3{ midPoint.x, -midPoint.y,0.f }, glm::vec3{ abs(target->endPos.x - target->startPos.x) , abs(target->endPos.y - target->startPos.y),0.f }, "Wall", ObjectType::WALL);
				temp->AddComponent<Sprite>();
				temp->GetComponent<Sprite>()->AddQuad({ 0.f,1.f,0.f,0.25f });

				temp->AddComponent<Physics2D>();
				temp->GetComponent<Physics2D>()->AddCollidePolygonAABB({ temp->GetSize().x / 2.f,  temp->GetSize().y / 2.f });
				temp->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
				temp->GetComponent<Physics2D>()->SetMass(1.f);
				walls.push_back(std::move(temp));
				isWallSetting = false;
			}
			else
			{
				isWallSetting = true;
			}
		}
		else if (Engine::Instance().GetInputManager()->IsMouseButtonPressedOnce(MOUSEBUTTON::RIGHT))
		{
			if (isWallSetting == true)
			{
				isWallSetting = false;
			}
		}
	}
}

void PlatformDemoSystem::Init()
{
	mapEditor = new PDemoMapEditorDemo();
	backGroundManager = new BackgroundManager();
#ifdef _DEBUG
	mapEditor->SetBackgroundManager(backGroundManager);
#endif
}

void PlatformDemoSystem::Update(float dt)
{
	glm::vec2 viewSize = Engine::GetCameraManager()->GetViewSize();
	glm::vec2 center = Engine::GetCameraManager()->GetCenter();
	healthBar->UpdateModel({ (-viewSize.x / 2.f + 320.f) + center.x - (320.f - (320.f * (1.f / maxHp * hp)) / 2.f) , (viewSize.y / 2.f - 128.f) + center.y, 0.f }, { 320.f * (1.f / maxHp * hp), 64.f, 0.f }, 0.f);
	healthBar->UpdateProjection();
	healthBar->UpdateView();

	backGroundManager->Update(dt);
	//#ifdef _DEBUG
	//	mapEditor->Update(dt);
	//#endif
}

void PlatformDemoSystem::End()
{
	delete healthBar;
	healthBar = nullptr;
	delete backGroundManager;
#ifdef _DEBUG
	mapEditor->End();
	delete mapEditor;
#endif
}
void PlatformDemoSystem::InitHealthBar()
{
#ifdef _DEBUG
	mapEditor->Init();
#endif
	healthBar = new Sprite();
	healthBar->AddQuad({ 0.f,1.f,0.f,1.f });
}

#ifdef _DEBUG
void PlatformDemoSystem::UpdateMapEditor(float dt)
{
	mapEditor->Update(dt);
}
#endif
