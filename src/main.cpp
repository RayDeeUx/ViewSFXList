#include <Geode/modify/AudioAssetsBrowser.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include "ErysSFXBrowser.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

class $modify(MyAudioAssetsBrowser, AudioAssetsBrowser) {
	bool init(gd::vector<int>& songIds, gd::vector<int>& sfxIds) {
		if (!AudioAssetsBrowser::init(songIds, sfxIds)) return false;
		if (!m_buttonMenu || static_cast<std::vector<int>>(sfxIds).empty()) return true;

		CCLabelBMFont* viewSFXLabel = CCLabelBMFont::create("View\nSFXs", "bigFont.fnt");
		viewSFXLabel->setAlignment(kCCTextAlignmentCenter);
		CircleButtonSprite* pinkSprite = CircleButtonSprite::create(viewSFXLabel, CircleBaseColor::Pink);
		pinkSprite->setScale(.6f);

		CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(pinkSprite, this, menu_selector(MyAudioAssetsBrowser::onViewErysSFXList));
		button->setID("view-sfx-list"_spr);
		button->setTag(9062025);

		LevelInfoLayer* lil = CCScene::get()->getChildByType<LevelInfoLayer>(0);
		if (lil && lil->m_level) button->setUserObject("level-name"_spr, CCString::create(lil->m_level->m_levelName));
		else if (auto* gjbgl = GJBaseGameLayer::get(); gjbgl) button->setUserObject("level-name"_spr, CCString::create(gjbgl->m_level->m_levelName));
		else button->setUserObject("level-name"_spr, CCString::create("this (unknown) level")); // may god help us all if it ever reaches this point

		m_buttonMenu->addChild(button);
		button->setPosition({194.f, 100.f});

		return true;
	}
	void onViewErysSFXList(CCObject* sender) {

		CCNode* senderAsNode = typeinfo_cast<CCNode*>(sender);
		if (!senderAsNode) return;
		if (senderAsNode->getID() != "view-sfx-list"_spr) return;

		CCObject* hopefullyMyUserObject = senderAsNode->getUserObject("level-name"_spr);
		if (!hopefullyMyUserObject || !typeinfo_cast<CCString*>(hopefullyMyUserObject)) return;

		Manager::get()->managerLevelName = static_cast<CCString*>(hopefullyMyUserObject)->getCString();
		Manager::get()->managerSFXIDs = static_cast<std::vector<int>>(m_sfxIds);
		ErysSFXBrowser::create()->show();
	}
};

class $modify(MyCustomSongWidget, CustomSongWidget) {
	static void onModify(auto& self) {
		(void) self.setHookPriority("CustomSongWidget::init", -3999);
	}
	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
		if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) return false;
		LevelEditorLayer* lel = LevelEditorLayer::get();
		if (!lel || !CCScene::get() || CCScene::get()->getChildByType<SetupSongTriggerPopup>(0) || !CCScene::get()->getChildByType<LevelSettingsLayer>(0)) return true;
		if (!m_buttonMenu || m_buttonMenu->getChildByID("dummy-info-button-for-milkcat"_spr)) return true;
		if (lel->m_level->m_sfxIDs.empty()) return true;
		// TODO: unk is probably an enum that might become a Geode addition!!! 1 == audio asset browser viewing level-specific assets
		if (unk != 0 || isMusicLibrary) return true;
		if (Loader::get()->isModLoaded("taswert.showaudioassets")) return true; // avoid double buttons. although HOLY SHIT the codebase in that mod is questionable as hell :sob:
		CCSprite* infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
		infoSprite->setScale(.65f);
		CCMenuItemSpriteExtra* sfxListBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(MyCustomSongWidget::onViewErysSFXList));
		sfxListBtn->setSizeMult(1.2f);
		sfxListBtn->m_scaleMultiplier = 1.26f;
		sfxListBtn->setPosition(-155.f, -130.f);
		sfxListBtn->setID("view-sfx-list"_spr);
		sfxListBtn->setUserObject("level-name"_spr, CCString::create(lel->m_level->m_levelName));
		m_buttonMenu->addChild(sfxListBtn);
		return true;
	}
	void onViewErysSFXList(CCObject* sender) {
		if (!GJBaseGameLayer::get() || !GJBaseGameLayer::get()->m_level || GJBaseGameLayer::get()->m_level->m_sfxIDs.empty()) return;
		CCNode* senderAsNode = typeinfo_cast<CCNode*>(sender);
		if (!senderAsNode) return;
		if (senderAsNode->getID() != "view-sfx-list"_spr) return;

		std::vector<int> songIDVector = {};
		for (auto songIDStr : utils::string::split(GJBaseGameLayer::get()->m_level->m_songIDs, ",")) {
			if (const int toPush = utils::numFromString<int>(songIDStr).unwrapOr(-1); toPush != -1) songIDVector.push_back(toPush);
		}

		gd::vector<int> songIDVectorGD = static_cast<gd::vector<int>>(songIDVector);

		std::vector<int> sfxIDVector = {};
		for (auto sfxIDStr : utils::string::split(GJBaseGameLayer::get()->m_level->m_sfxIDs, ",")) {
			if (const int toPush = utils::numFromString<int>(sfxIDStr).unwrapOr(-1); toPush != -1) sfxIDVector.push_back(toPush);
		}

		gd::vector<int> sfxIDVectorGD = static_cast<gd::vector<int>>(sfxIDVector);

		AudioAssetsBrowser::create(songIDVectorGD, sfxIDVectorGD)->show();
	}
};