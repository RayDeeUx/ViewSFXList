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
		if (Loader::get()->isModLoaded("spaghettdev.songpreview")) (void) self.setHookPriorityAfterPost("CustomSongWidget::updateWithMultiAssets", "spaghettdev.songpreview");
		else if (Loader::get()->isModLoaded("raydeeux.copysongid")) (void) self.setHookPriorityAfterPost("CustomSongWidget::updateWithMultiAssets", "raydeeux.copysongid");
		else (void) self.setHookPriority("CustomSongWidget::updateWithMultiAssets", -3999);
	}
	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
		if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) return false;
		if (!LevelEditorLayer::get() || CCScene::get()->getChildByType<SetupSongTriggerPopup>(0) || !this->getParent() || !this->getParent()->getParent() || !typeinfo_cast<CustomSongLayer*>(this->getParent()->getParent())) return true;
		if (!m_playbackBtn || !m_infoBtn) return true;
		m_infoBtn->setVisible(true);
		if (m_playbackBtn->isVisible() || showPlayMusic) m_infoBtn->setPositionX(-155.f);
		return true;
	}
	void updateWithMultiAssets(gd::string songList, gd::string sfxList, int p2) {
		CustomSongWidget::updateWithMultiAssets(songList, sfxList, p2);
		if (!LevelEditorLayer::get() || CCScene::get()->getChildByType<SetupSongTriggerPopup>(0) || !this->getParent() || !this->getParent()->getParent() || !typeinfo_cast<CustomSongLayer*>(this->getParent()->getParent())) return;
		if (!m_playbackBtn || !m_infoBtn) return;
		m_infoBtn->setVisible(true);
		if (m_playbackBtn->isVisible() || m_showPlayMusicBtn) m_infoBtn->setPositionX(-1552.f);
	}
};