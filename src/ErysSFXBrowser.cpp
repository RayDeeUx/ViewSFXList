#include "ErysSFXBrowser.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

ErysSFXBrowser* ErysSFXBrowser::create() {
	auto* ret = new ErysSFXBrowser();
	if (ret->initAnchored(380.f, 250.f, "GJ_square02.png")) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool ErysSFXBrowser::setup() {
	this->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCBool::create(true)); // fuck off, user95401.
	this->m_noElasticity = true;
	this->setTitle(fmt::format("SFX Assets in {}", Manager::get()->managerLevelName));
	this->m_title->setID("level-sfx-list-title"_spr);

	ScrollLayer* scrollLayer = geode::ScrollLayer::create({340.f, 200.f});
	scrollLayer->m_contentLayer->setLayout(
		ColumnLayout::create()
			->setAxisReverse(true)
			->setAxisAlignment(AxisAlignment::End)
			->setAutoGrowAxis(scrollLayer->getContentHeight())
			->setGap(18.f) // show gap of 60.f for non-compact mode
	);
	scrollLayer->m_contentLayer->setID("sfx-list-content-layer"_spr);
	scrollLayer->setID("sfx-list-scroll-layer"_spr);

	MusicDownloadManager* mdm = MusicDownloadManager::sharedState();
	mdm->parseSFXLibrary(); // this function call is apparently mandatory to have any hope of rendering SFX cells outside of LevelEditorLayer :P

	if (mdm->m_sfxObjectCount < 1) {
		log::error("Uh-oh! No SFXs were found!");
		Loader::get()->queueInMainThread([]() {
			Loader::get()->queueInMainThread([]() {
				FLAlertLayer::create("Oh no!", "Looks like someone's messed with the SFX library again. Try again later.", "I Understand")->show();
			});
		});
	}

	CCNodeRGBA* dummyNode = CCNodeRGBA::create();
	dummyNode->setID("top-padding"_spr);
	dummyNode->setContentSize({0.f, 0.f});
	dummyNode->setOpacity(0);
	scrollLayer->m_contentLayer->addChild(dummyNode);

	bool isEven = true;

	for (const int sfxID : Manager::get()->managerSFXIDs) {
		SFXInfoObject* sfxObject = mdm->getSFXObject(sfxID);
		if (!sfxObject) {
			log::error("no sfx object for ID {}", sfxID);
			continue;
		}

		CustomSFXWidget* sfxWidget = CustomSFXWidget::create(sfxObject, this, false, true, true, true, true);
		// bool p2 is for showing "Use" button
		// bool p3 is for showing "Playback" button
		// bool p4 is for showing "Download" button
		// bool p5 is for... hiding the background? for some reason
		// bool p6 is for compact mode (which also hides the bg????)
		if (!sfxWidget) {
			log::error("no sfx widget for ID {}", sfxID);
			continue;
		}

		mdm->addMusicDownloadDelegate(sfxWidget);

		sfxWidget->setUserObject("special-sfx-cell"_spr, CCBool::create(true));
		sfxWidget->setTag(9062025);

		DISABLE_BUTTON(sfxWidget->m_cancelButton)
		DISABLE_BUTTON(sfxWidget->m_selectButton)
		DISABLE_BUTTON(sfxWidget->m_deleteButton)

		if (sfxWidget->m_idLabel) sfxWidget->m_idLabel->setColor({255, 255, 255});

		CCLayerColor* background = isEven ? CCLayerColor::create({42, 55, 136, 255}) : CCLayerColor::create({34, 42, 119, 255});
		background->ignoreAnchorPointForPosition(false);
		background->setContentSize({340.f, 18.f});
		background->setPosition({0.f, 0.f});
		background->setZOrder(-1);

		sfxWidget->addChild(background);
		isEven = !isEven;

		scrollLayer->m_contentLayer->addChild(sfxWidget);
	}

	CCNodeRGBA* dummyNodeAgain = CCNodeRGBA::create();
	dummyNodeAgain->setID("bottom-padding"_spr);
	dummyNodeAgain->setContentSize({0.f, 0.f});
	dummyNodeAgain->setOpacity(0);
	scrollLayer->m_contentLayer->addChild(dummyNodeAgain);

	scrollLayer->m_contentLayer->updateLayout();
	scrollLayer->scrollToTop();

	scrollLayer->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChildAtPosition(scrollLayer, Anchor::Center, {0.f, -10.f});
	scrollLayer->m_contentLayer->setPositionX(scrollLayer->getContentWidth() / 2.f);

	ListBorders* listBorder = ListBorders::create();
	listBorder->setSpriteFrames("GJ_commentTop2_001.png", "GJ_commentSide2_001.png");
	listBorder->setContentSize(scrollLayer->getContentSize());
	listBorder->ignoreAnchorPointForPosition(false);
	listBorder->setID("sfx-list-border"_spr);

	m_mainLayer->addChildAtPosition(listBorder, Anchor::Center, {0.f, -10.f});

	Scrollbar* scrollbar = Scrollbar::create(scrollLayer);
	this->m_mainLayer->addChildAtPosition(scrollbar, Anchor::Center, {177.5f, -10.f});
	scrollbar->setID("robtop-broke-your-touch-prio-so-ill-break-his-ego"_spr);

	this->m_buttonMenu->setID("close-menu"_spr);
	this->m_closeBtn->setID("close-button"_spr);
	this->m_mainLayer->setID("main-layer"_spr);
	this->m_bgSprite->setID("background"_spr);

	this->setID("ErysSFXBrowser"_spr);

	mdm->getCustomContentURL(); // i actually fucking hate myself. this was all i needed??@!@ BUT NOW TOUCHPRIO BREAKS?!?!?!?

	this->m_title->setPositionY(this->m_title->getPositionY() + 5.25f);
	CCLabelBMFont* useScrollbarLabel = CCLabelBMFont::create("(Please use the scrollbar!)", "chatFont.fnt");
	useScrollbarLabel->setScale(.5f);
	this->m_title->getParent()->addChild(useScrollbarLabel);
	useScrollbarLabel->setPosition(this->m_title->getPosition());
	useScrollbarLabel->setPositionY(useScrollbarLabel->getPositionY() - 13.25f);
	useScrollbarLabel->setID("scrollbar-warning"_spr);

	Loader::get()->queueInMainThread([]() {
		Manager::get()->managerLevelName = "";
		Manager::get()->managerSFXIDs = {};
	});

	return true;
}