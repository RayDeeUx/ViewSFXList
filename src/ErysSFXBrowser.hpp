#pragma once

using namespace geode::prelude;

#define DISABLE_BUTTON(button) \
	if (button) { \
		button->m_pListener = nullptr; \
		button->m_pfnSelector = nullptr; \
		button->m_animationEnabled = false; \
		button->m_bEnabled = false; \
		button->m_baseScale = 0.f; \
		button->m_scaleMultiplier = 0.f; \
		button->m_fSizeMult = 0.f; \
		button->setOpacity(0); \
		button->setScale(0.f); \
		button->setContentSize({0.f, 0.f}); \
		button->setPosition(12211989.f, 12211989.f); \
	}

class ErysSFXBrowser final : public geode::Popup<>, public CustomSFXDelegate {
public:
	static ErysSFXBrowser* create();
	bool setup() override;
};