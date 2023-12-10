#ifndef _COLLAPSE_WIDGET_HPP_
#define _COLLAPSE_WIDGET_HPP_

#include <QWidget>


class QParallelAnimationGroup;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QSpacerItem;


class CollapseWidget : public QWidget
{
	QWidget* m_pCap{ nullptr };
	QHBoxLayout* m_pCapLayout{ nullptr };
	QSpacerItem* pSpacerLeft{ nullptr };
	QLabel* m_pText{ nullptr };
	QSpacerItem* pSpacerRight{ nullptr };
	QToolButton* m_pCollapseButton{ nullptr };
	QVBoxLayout* m_pLayout{ nullptr };

	QWidget* m_pContent{ nullptr };
	QParallelAnimationGroup* m_pAnimator{ nullptr };

public:
	CollapseWidget(QWidget* parent);
	~CollapseWidget() override;

	void setText(const QString& text);
	void setContent(QWidget* content);

	void hideContent();
	void showContent();

};

#endif
