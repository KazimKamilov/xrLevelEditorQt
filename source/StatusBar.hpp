#ifndef _TOOLBAR_HPP_
#define _TOOLBAR_HPP_

#include <QStatusBar>

class QToolButton;
class QLabel;

class StatusBar final : public QStatusBar
{
	QWidget* m_pStatusWidget{ nullptr };
	QToolButton* m_pOptionsBtn{ nullptr };
	QToolButton* m_pOptionGrip{ nullptr };
	QToolButton* m_pMacroBtn{ nullptr };
	QToolButton* m_pMacroGrip{ nullptr };
	QToolButton* m_pLogBtn{ nullptr };
	QToolButton* m_pStatBtn{ nullptr };
	QToolButton* m_pBreakBtn{ nullptr };
	QLabel* m_pStatusLbl{ nullptr };
	QLabel* m_pStatusText{ nullptr };
	QLabel* m_pStatSelected{ nullptr };
	QLabel* m_pStatCoords{ nullptr };
	QLabel* m_pStatScreen{ nullptr };
	QLabel* m_pStatGrid{ nullptr };

public:
	StatusBar(QWidget* parent);
	~StatusBar() override;

	QToolButton* getOptionsBtn() const noexcept { return m_pOptionsBtn; }
	QToolButton* getOptionsGrip() const noexcept { return m_pOptionGrip; }
	QToolButton* getMacroBtn() const noexcept { return m_pMacroBtn; }
	QToolButton* getMacroGrip() const noexcept { return m_pMacroGrip; }
	QToolButton* getLogBtn() const noexcept { return m_pLogBtn; }
	QToolButton* getStatBtn() const noexcept { return m_pStatBtn; }
	QToolButton* getBreakBtn() const noexcept { return m_pBreakBtn; }

};

#endif
